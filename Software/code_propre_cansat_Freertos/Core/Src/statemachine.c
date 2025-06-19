/*
 * statemachine.c
 *
 *  Created on: Mar 22, 2025
 *      Author: mathi
 */

#include "statemachine.h"
#include "main.h"
#include "bmp581.h"
#include "6axis.h"
#include "tarvos.h"
#include "led.h"
#include "usart.h"
#include "ssd1306.h"
#include "GNSS.h"

extern AXIS6 myData6AXIS;
extern BMP_t myDatabmp581;
extern GNSS_StateHandle GNSSData;

extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;

extern STATE state;

extern float hauteur_Initiale;

extern float temp;
extern float vrefint;
extern float vbat;


extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];

extern uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];



extern int flag_calib;
extern int flag_drop;
extern int sd_detect_flag;
int send_data_len=0;

uint8_t screenbuffer[50];

extern GNSS_StateHandle GNSSData;
int screenindex=0;
int datascreenindex=0;
int delaycounterforscreenindex=0;
#ifdef PARTIE_BAS
extern float distance_entre_module;
#endif

extern uint32_t timeindex;




void statemachine(void){

	switch(state){

	case IDLE:
		ssd1306_Fill(Black);
		ssd1306_SetCursor(32, 32);
switch(screenindex){
case 0:
	if (vbat <= 7){
					ssd1306_WriteString("bat_low", Font_7x10, White);
					LED_Setcolour(255,0,0,255,0,0);
				}
	break;
case 1:
	if(flag_calib==0){
		ssd1306_WriteString("alt_cal", Font_7x10, White);
		LED_Setcolour(0,255,0,0,255,0);
				}
	break;
case 2:
	if (GNSSData.fixType <= 2){
		ssd1306_WriteString("gps_fix", Font_7x10, White);
		LED_Setcolour(255,255,0,255,255,0);
				}

	break;
case 3:
	if(HAL_GPIO_ReadPin(PWEN_GPIO_Port,PWEN_Pin)==0){
		ssd1306_WriteString("TELEPWR", Font_7x10, White);
		LED_Setcolour(0,255,255,0,255,255);
	}
	break;
case 4:
	if(sd_detect_flag==0){
		ssd1306_WriteString("no_sd", Font_7x10, White);
		LED_Setcolour(255,0,255,255,0,255);
	}
	break;
}


ssd1306_SetCursor(32, 42);

if(datascreenindex<=10){
	ssd1306_WriteString("bmpalt:", Font_7x10, White);
	ssd1306_SetCursor(32, 52);
    	snprintf((char *)screenbuffer,50,"%0.3f",myDatabmp581.altitude);
    	ssd1306_WriteString((char *) screenbuffer, Font_7x10, White);
}
else if(datascreenindex>10 && datascreenindex<=20){
		if(flag_calib){
			ssd1306_WriteString("h_init:", Font_7x10, White);
				ssd1306_SetCursor(32, 52);
			snprintf((char *)screenbuffer,50,"%0.1f",hauteur_Initiale);
			ssd1306_WriteString((char *) screenbuffer, Font_7x10, White);
		}
}
else if (datascreenindex>20 && datascreenindex<=30) {
    		ssd1306_WriteString("sat:", Font_7x10, White);
    			ssd1306_SetCursor(32, 52);
    		snprintf((char *)screenbuffer,50,"%d",GNSSData.numSV);
    		ssd1306_WriteString((char *) screenbuffer, Font_7x10, White);

    	}
else if (datascreenindex>30 && datascreenindex<=40){
#ifdef PARTIE_HAUT
	datascreenindex=0;

#endif
#ifdef PARTIE_BAS
	ssd1306_WriteString("dist:", Font_7x10, White);
	ssd1306_SetCursor(32, 52);
	snprintf((char *)screenbuffer,50,"%f",distance_entre_module);
	ssd1306_WriteString((char *) screenbuffer, Font_7x10, White);

#endif
    	}
else{
#ifdef PARTIE_BAS
	datascreenindex=0;

#endif


}


if((flag_drop==1) && (flag_calib==1)){

				state++;
#ifdef PARTIE_HAUT
				create_and_send_payload((uint8_t *) tarvos_TX_Buffer,0x82,BOTTOM_ADDR,0x10,0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0);
#endif
			}

			delaycounterforscreenindex++;
			if(delaycounterforscreenindex>=5){
				delaycounterforscreenindex=0;
				screenindex++;
			}

			if(screenindex>4){
				screenindex=0;
			}
			datascreenindex++;
#ifdef PARTIE_HAUT
			if(datascreenindex>=30){
				datascreenindex=0;
			}
#endif
#ifdef PARTIE_BAS
			if(datascreenindex>=40){
				datascreenindex=0;
			}
#endif
		break;

	case PRESEPARATION:

		create_and_send_payload((uint8_t *) tarvos_TX_Buffer,0x82,GROUND_ADDR,0x10,0,GNSSData.fLat,GNSSData.fLon,GNSSData.fhMSL,myDatabmp581.altitude,0.0,0.0,temp,myDatabmp581.press,myData6AXIS.AccelX,myData6AXIS.AccelY,myData6AXIS.AccelZ,timeindex);


#ifdef PARTIE_HAUT
#endif
#ifdef PARTIE_BAS
#endif
		break;

	case POSTSEPARATION:

		create_and_send_payload((uint8_t *) tarvos_TX_Buffer,0x82,GROUND_ADDR,0x10,0,GNSSData.fLat,GNSSData.fLon,GNSSData.fhMSL,myDatabmp581.altitude,0.0,0.0,temp,myDatabmp581.press,myData6AXIS.AccelX,myData6AXIS.AccelY,myData6AXIS.AccelZ,timeindex);
		create_and_send_payload((uint8_t *) tarvos_TX_Buffer,0x82,BOTTOM_ADDR,0x10,0,GNSSData.fLat,GNSSData.fLon,GNSSData.fhMSL,myDatabmp581.altitude,0.0,0.0,temp,myDatabmp581.press,myData6AXIS.AccelX,myData6AXIS.AccelY,myData6AXIS.AccelZ,timeindex);


#ifdef PARTIE_HAUT
		create_and_send_payload((uint8_t *) tarvos_TX_Buffer,0x82,BOTTOM_ADDR,0x10,0,GNSSData.fLat,GNSSData.fLon,GNSSData.fhMSL,myDatabmp581.altitude,0.0,0.0,temp,myDatabmp581.press,myData6AXIS.AccelX,myData6AXIS.AccelY,myData6AXIS.AccelZ,timeindex);
#endif
#ifdef PARTIE_BAS
#endif


		break;
	case FIN:

		ssd1306_Fill(Black);
		ssd1306_SetCursor(32, 32);
		ssd1306_WriteString("FIN", Font_16x24, White);
		break;
	}
}
