
#include "tarvos.h"
#include "usart.h"
#include <stdint.h>
// Définition des variables



extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];

extern UART_HandleTypeDef hlpuart1;



HAL_StatusTypeDef SET_tcMODE(char mode){
	uint8_t transparent[] = {0x02, 0x04, 0x01, 0x00, 0x07};
	uint8_t command[] = {0x02, 0x04, 0x01, 0x10, 0x07};
	HAL_StatusTypeDef status=HAL_OK;

	if (strstr((char *)mode,"transparent")){
		if(HAL_UART_Transmit(&hlpuart1,transparent,sizeof(transparent),100)!=HAL_OK){

			status=HAL_ERROR;
		}

	}
	if (strstr((char *)mode,"command")){
		if(HAL_UART_Transmit(&hlpuart1,command,sizeof(command),100)!=HAL_OK){
			status=HAL_ERROR;
		}

	}
	return status;
}

HAL_StatusTypeDef INIT_TARVOS(UART_HandleTypeDef hlpuart1){
	HAL_StatusTypeDef status=SET_tcMODE("command");

	return status;
}

HAL_StatusTypeDef INIT_PERM_TARVOS(uint8_t Hadr, uint8_t Sadr) {
	 uint8_t commandsize=6;
	 uint8_t restartsize=4;
	 HAL_StatusTypeDef status=HAL_OK;

    uint8_t mode1[] = {0x02, 0x09, 0x02, 0x04, 0x01,0x00};

    uint8_t frequence[] = {0x02, 0x09, 0x02, 0x03, 0x82, 0x00}; // 869.5 MHz -> 130 = 0x82
    // On modifie la puissance (ici à 500mW max)
    uint8_t puissance[] = {0x02, 0x09, 0x02, 0x02, 0x1B, 0x00};

    // On modifie l'adresse de la source
    uint8_t Sadrr[] = {0x02, 0x09, 0x02, 0x0B, Hadr, 0x00};

    // On modifie l'adresse avec qui on veut communiquer
   	uint8_t send[] = {0x02, 0x09, 0x02, 0x08, Sadr, 0x00};

   	// On redémarre le module pour mettre à jour les paramètres
   	uint8_t restart[4] = {0x02, 0x05, 0x00, 0x07};

    mode1[5] = Get_CRC8(mode1, commandsize-1); // Ajout du CRC
    if(HAL_UART_Transmit(&hlpuart1,(uint8_t *)mode1, commandsize, 100)!=HAL_OK){

    	status=HAL_ERROR;
    }





    frequence[5] = Get_CRC8(frequence,  commandsize-1); // Ajout du CRC
    if( HAL_UART_Transmit(&hlpuart1, frequence, commandsize, 500)!=HAL_OK){

      	status=HAL_ERROR;
      }



    puissance[5] = Get_CRC8(puissance,  commandsize-1); // Ajout du CRC
    if( HAL_UART_Transmit(&hlpuart1, puissance, commandsize, 500)!=HAL_OK){

      	status=HAL_ERROR;
      }



    Sadrr[5] = Get_CRC8(Sadrr, commandsize-1); // Ajout du CRC
    if( HAL_UART_Transmit(&hlpuart1, Sadrr, commandsize, 500)!=HAL_OK){

      	status=HAL_ERROR;
      }



	send[5] = Get_CRC8(send,  commandsize-1); // Ajout du CRC
	  if(HAL_UART_Transmit(&hlpuart1, send, commandsize, 500)!=HAL_OK){

	    	status=HAL_ERROR;
	    }


	  if(HAL_UART_Transmit(&hlpuart1, restart, restartsize, 500)!=HAL_OK){

	    	status=HAL_ERROR;
	    }


    HAL_Delay(1000);
    return status;
}



uint8_t Get_CRC8(uint8_t * bufP, uint16_t len){

	uint8_t crc = 0x00;
	for (uint16_t i = 0; i < len; i++){
		crc ^= bufP[i];
	}
	return crc;
}



int SEND_DATA_NETW(uint8_t *data, uint8_t channel, uint8_t dest_adress, int length) {
	HAL_StatusTypeDef status=HAL_OK;
	uint8_t bufferindex=0;
	memset((uint8_t *) tarvos_TX_Buffer,256,'\0');

	if(length>=250){
		return HAL_ERROR;
	}

    // Initialisation de l'en-tête

	tarvos_TX_Buffer[bufferindex]= 0x02;
	bufferindex++;// Start byte
	tarvos_TX_Buffer[bufferindex]= 0x01;
	bufferindex++;// Command identifier
	tarvos_TX_Buffer[bufferindex]= length + 2;
	bufferindex++;// Longueur totale (length + channel + dest_adress)
	tarvos_TX_Buffer[bufferindex]= channel;
	bufferindex++;// Canal
	tarvos_TX_Buffer[bufferindex]= dest_adress; // Adresse de destination

	bufferindex++;

    // Copie des données dans la trame
    for (int i = 0; i < length; i++) {
    	tarvos_TX_Buffer[bufferindex]=data[i];
    	bufferindex++;

    }





    // Calcul du CRC (sur tout sauf le CRC lui-même)
    tarvos_TX_Buffer[bufferindex]= Get_CRC8((uint8_t *)(tarvos_TX_Buffer), bufferindex);///////pas surrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr///


    if( HAL_UART_Transmit(&hlpuart1,(uint8_t *) tarvos_TX_Buffer, bufferindex,1000)!=HAL_OK){
    	status=HAL_ERROR;
    }

    return status;

}

void SEND_DATA_NETW1(uint8_t *data, uint8_t channel, uint8_t dest_adress, int length) {
    // Longueur totale de la trame : en-tête (5 octets) + données + CRC
    uint8_t trame[5 + length + 1]; // +1 pour le CRC

    // Initialisation de l'en-tête
    trame[0] = 0x02;            // Start byte
    trame[1] = 0x01;            // Command identifier
    trame[2] = length + 2;      // Longueur totale (length + channel + dest_adress)
    trame[3] = channel;         // Canal
    trame[4] = dest_adress;     // Adresse de destination

    // Copie des données dans la trame
    for (uint8_t i = 0; i < length; i++) {
        trame[5 + i] = data[i];
    }

    // Calcul du CRC (sur tout sauf le CRC lui-même)
    trame[5 + length] = Get_CRC8(trame, 5 + length);

    // Transmission de la trame
    HAL_UART_Transmit(&hlpuart1, trame, sizeof(trame), 500);
}

HAL_StatusTypeDef SET_Destadr(uint8_t Destadrr){
	HAL_StatusTypeDef status=HAL_OK;
	//on change l'adresse de destination à Destadrr
	uint8_t buffer[]={0x02,0x08,0x01,Destadrr,0x00};
	uint8_t cmdsize=5;
	buffer[4]=Get_CRC8(buffer,4);
	if(HAL_UART_Transmit(&hlpuart1,(uint8_t *) buffer, cmdsize, 500)!=HAL_OK){
			status=HAL_ERROR;
		}
	return status;

}

HAL_StatusTypeDef REQ(uint8_t index){
	HAL_StatusTypeDef status=HAL_OK;
	uint8_t cmdsize=5;
	uint8_t buffer[]={0x02,0x0A,0x01,index,0x00};
	buffer[4]=Get_CRC8(buffer,4);
	if(HAL_UART_Transmit(&hlpuart1,(uint8_t *) buffer, cmdsize, 500)!=HAL_OK){
		status=HAL_ERROR;
	}
	return status;


}

HAL_StatusTypeDef REQ_RSSI(){
	HAL_StatusTypeDef status=HAL_OK;
	uint8_t cmdsize=4;
	uint8_t buffer_RSSI[]={0x02,0x0D,0x00,0x0F};
	if(HAL_UART_Transmit(&hlpuart1,(uint8_t *) buffer_RSSI, cmdsize, 500)!=HAL_OK){
			status=HAL_ERROR;
		}
	return status;

}


