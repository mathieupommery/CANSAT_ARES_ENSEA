/*
 * statemachine.c
 *
 *  Created on: Mar 22, 2025
 *      Author: mathi
 */

#include "statemachine.h"
#include "main.h"
#include "tarvos.h"
#include "led.h"
#include "usart.h"
extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;

extern STATE state;

extern float temp;
extern float vrefint;
extern float vbat;


extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];

extern uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];



extern int flag_calib;
extern int flag_drop;

int send_data_len=0;

uint8_t screenbuffer[50];

int screenindex=0;
int datascreenindex=0;




void statemachine(void){

	switch(state){

	case IDLE:
if(datascreenindex<=10){

}
else if(datascreenindex>10 && datascreenindex<=20){

}
else{

}

			datascreenindex++;
			if(datascreenindex>=30){
				datascreenindex=0;
			}




		break;

	case PRESEPARATION:


		break;





	case POSTSEPARATION:

		break;







	}


}
