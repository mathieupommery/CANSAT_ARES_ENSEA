/*
 * statemachine.c
 *
 *  Created on: Mar 22, 2025
 *      Author: mathi
 */

#include "statemachine.h"
#include "main.h"
#include "nmea_parse.h"
#include "bmp581.h"
#include "6axis.h"
#include "tarvos.h"
#include "led.h"
#include "usart.h"
#include "ssd1306.h"

uint8_t buffertosend[224];

extern GPS myDataGPS;
extern AXIS6 myData6AXIS;
extern BMP_t myDatabmp581;

extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;

extern STATE state;

extern int hauteur_Initiale;

extern float temp;
extern float vrefint;
extern float vbat;


extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];

extern uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];
extern uint8_t tarvos_RX_Tampon[TarvosRxTamponSize];

extern uint8_t GPS_RX_Buffer[GPSRxBufferSize];
extern uint8_t GPS_RX_Tampon[GPSRxTamponSize];
extern int flag_calib;
extern int flag_drop;

int send_data_len=0;


void statemachine(void){

	switch(state){

	case IDLE:


			if (vbat <= 7){

				ssd1306_Fill(Black);
				ssd1306_SetCursor(32, 40);
				ssd1306_WriteString("BAT LOW", Font_7x10, White);
				ssd1306_UpdateScreen();
				send_data_len=snprintf((char *) buffertosend,224,"$2,batterie faible#");
				SEND_DATA_NETW((uint8_t *) buffertosend, 0x82, GROUND_ADDR, send_data_len);


			}

			if(flag_calib==0){

				ssd1306_Fill(Black);
				ssd1306_SetCursor(32, 32);
				ssd1306_WriteString("attention", Font_7x10, White);
				ssd1306_SetCursor(32, 42);
				ssd1306_WriteString("alt_calib", Font_7x10, White);
				ssd1306_UpdateScreen();
				send_data_len=snprintf((char *) buffertosend,224,"$2,[top]altitude non calibre#");
				SEND_DATA_NETW((uint8_t *) buffertosend, 0x82,  GROUND_ADDR, send_data_len);
			}
			if (myDataGPS.fix == 0){
					send_data_len=snprintf((char *) buffertosend,224,"$2,pas de GPS#");
					SEND_DATA_NETW((uint8_t *) buffertosend, 0x82,  GROUND_ADDR, send_data_len);
					ssd1306_SetCursor(32, 52);
					ssd1306_WriteString("gps_fix", Font_7x10, White);
					ssd1306_UpdateScreen();
			}




			if((flag_drop==1) && (flag_calib==1)){
				state++;
			}




		break;

	case PRESEPARATION:


		send_data_len=snprintf((char *) buffertosend,224,"$3,0.7f,0.7f,0.2f#",myDataGPS.latitude,myDataGPS.longitude,myDatabmp581.altitude);
		SEND_DATA_NETW((uint8_t *) buffertosend, 0x82, GROUND_ADDR, send_data_len);












		if((myDatabmp581.altitude-hauteur_Initiale)<=60.0){
			state++;
			release_mecanism();
			send_data_len=snprintf((char *) buffertosend,224,"$1,separation#");
			SEND_DATA_NETW((uint8_t *) buffertosend, 0x82, GROUND_ADDR, send_data_len);
			send_data_len=snprintf((char *) buffertosend,224,"$1,0.2f\n\r",(myDatabmp581.altitude-hauteur_Initiale));
			SEND_DATA_NETW((uint8_t *) buffertosend, 0x82, BOTTOM_ADDR, send_data_len);
		}
		break;





	case POSTSEPARATION:

		send_data_len=snprintf((char *) buffertosend,224,"$3,0.7f,0.7f,0.2f#",myDataGPS.latitude,myDataGPS.longitude,myDatabmp581.altitude);
		SEND_DATA_NETW((uint8_t *) buffertosend, 0x82, BOTTOM_ADDR, send_data_len);




		break;







	}


}
