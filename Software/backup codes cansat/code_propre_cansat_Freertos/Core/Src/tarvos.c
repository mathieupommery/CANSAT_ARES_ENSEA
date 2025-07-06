
#include "tarvos.h"
#include "usart.h"
#include "GNSS.h"
#include "bmp581.h"
#include <stdint.h>
#include "cmsis_os.h"

// Définition des variables



extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];
extern GNSS_StateHandle GNSSData;
extern BMP_t myDatabmp581;
extern UART_HandleTypeDef hlpuart1;
extern int flag_drop;
extern int flag_separation;
extern int flag_calib;
extern int flag_fin;
extern float hauteur_Initiale;
#ifdef PARTIE_BAS
int noreturn_flag1=0;
int noreturn_flag2=0;
int noreturn_flag3=0;
#endif

extern osMutexId uartmutexHandle;
extern osSemaphoreId uartTxDoneHandle;

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
    //HAL_UART_Transmit(&hlpuart1, trame, sizeof(trame), 500);
    if (osMutexWait(uartmutexHandle, osWaitForever) == osOK)
    {
    HAL_UART_Transmit_DMA(&hlpuart1, trame, sizeof(trame));
    osSemaphoreWait(uartTxDoneHandle, osWaitForever);
    osMutexRelease(uartmutexHandle);
}

    //uint8_t bufferreceivetest[10];
    //HAL_UART_Receive_IT(&hlpuart1,(uint8_t *)bufferreceivetest,5);

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


//payload size==54
void create_and_send_payload(uint8_t* buffer,uint8_t channel,uint8_t dest_adress,uint16_t header_code,
		float latitude,float longitude,float hMSL,float altitude_baro,float vspeed,float hspeed,
		float temperature,float pression, float Accx, float Accy, float Accz, uint32_t timeindex){
    uint8_t buffdonnee[54];
    buffdonnee[0] = (header_code >> 8) & 0xFF;
    buffdonnee[1] = header_code & 0xFF;
    buffdonnee[2] = flag_calib;
    buffdonnee[3] = flag_drop;
    buffdonnee[4] = flag_separation;
    buffdonnee[5] = flag_fin;

    memcpy(&buffdonnee[6],  &latitude,      sizeof(float));
    memcpy(&buffdonnee[10],  &longitude,     sizeof(float));
    memcpy(&buffdonnee[14], &hMSL,      sizeof(float));
    memcpy(&buffdonnee[18], &altitude_baro, sizeof(float));
    memcpy(&buffdonnee[22], &vspeed,        sizeof(float));
    memcpy(&buffdonnee[26], &hspeed,        sizeof(float));
    memcpy(&buffdonnee[30], &temperature,    sizeof(float));
    memcpy(&buffdonnee[34], &pression,    sizeof(float));
    memcpy(&buffdonnee[38], &Accx,    sizeof(float));
    memcpy(&buffdonnee[42], &Accy,    sizeof(float));
    memcpy(&buffdonnee[46], &Accz,    sizeof(float));
    memcpy(&buffdonnee[50], &timeindex,    sizeof(uint32_t));

    SEND_DATA_NETW1((uint8_t *)buffdonnee, channel,dest_adress, 54);

}

void decode_payload(DecodedPayload* out,uint8_t * receivingbuffer) {
	if(receivingbuffer[0]!=0x02 || receivingbuffer[1]!=0x81){
		return;
	}
	if(receivingbuffer[2]!=56){
		return;
	}

	out->senderadress=receivingbuffer[3];
	out->header_code = (receivingbuffer[4] << 8) | receivingbuffer[5];
    out->flag_calib = receivingbuffer[6];
    out->flag_drop = receivingbuffer[7];
    out->flag_separation = receivingbuffer[8];
    out->flag_fin = receivingbuffer[9];
#ifdef PARTIE_BAS


    memcpy(&out->latitude,      &receivingbuffer[10],  sizeof(float));
    memcpy(&out->longitude,     &receivingbuffer[14],  sizeof(float));
    memcpy(&out->hMSL,      &receivingbuffer[18], sizeof(float));
    memcpy(&out->altitude_baro, &receivingbuffer[22], sizeof(float));
    memcpy(&out->vspeed,        &receivingbuffer[26], sizeof(float));
    memcpy(&out->hspeed,        &receivingbuffer[30], sizeof(float));
    memcpy(&out->temperature,     &receivingbuffer[34], sizeof(float));
    memcpy(&out->pression,     &receivingbuffer[38], sizeof(float));
    memcpy(&out->Accx,     &receivingbuffer[42], sizeof(float));
    memcpy(&out->Accy,     &receivingbuffer[46], sizeof(float));
    memcpy(&out->Accz,     &receivingbuffer[50], sizeof(float));
    memcpy(&out->timeindex,     &receivingbuffer[54], sizeof(uint32_t));
    memcpy(&out->RSSI,     &receivingbuffer[58], sizeof(uint8_t));

    if(noreturn_flag3==0){
    if(receivingbuffer[6]==1){
    	hauteur_Initiale=myDatabmp581.altitude;
    	flag_calib=1;
    	noreturn_flag3=1;
    }
    }

    if(noreturn_flag1==0){
    if(receivingbuffer[7]==1){
    	flag_drop=1;
    	noreturn_flag1=1;
    }

    }
    if(noreturn_flag2==0){
    if(receivingbuffer[8]==1){
    	flag_separation=1;
    	noreturn_flag2=1;
    }

    }

#endif

#ifdef PARTIE_HAUT
    memcpy(&out->latitude,      &receivingbuffer[10],  sizeof(float));
    memcpy(&out->longitude,     &receivingbuffer[14],  sizeof(float));
    memcpy(&out->hMSL,      &receivingbuffer[18], sizeof(float));
    memcpy(&out->altitude_baro, &receivingbuffer[22], sizeof(float));
    memcpy(&out->vspeed,        &receivingbuffer[26], sizeof(float));
    memcpy(&out->hspeed,        &receivingbuffer[30], sizeof(float));
    memcpy(&out->temperature,     &receivingbuffer[34], sizeof(float));
    memcpy(&out->pression,     &receivingbuffer[38], sizeof(float));
    memcpy(&out->Accx,     &receivingbuffer[42], sizeof(float));
    memcpy(&out->Accy,     &receivingbuffer[46], sizeof(float));
    memcpy(&out->Accz,     &receivingbuffer[50], sizeof(float));
    memcpy(&out->timeindex,     &receivingbuffer[54], sizeof(uint32_t));
    memcpy(&out->RSSI,     &receivingbuffer[58], sizeof(uint8_t));
#endif




    memset((uint8_t *)receivingbuffer,0,64);
}

uint8_t tarvos_checksum(uint8_t *data, uint16_t len)
{
    uint8_t checksum = 0x00;
    for (uint16_t i = 0; i < len - 1; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

