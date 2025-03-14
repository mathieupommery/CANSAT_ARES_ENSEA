/*
 * led.c
 *
 *  Created on: Mar 14, 2025
 *      Author: mathi
 */


#include "led.h"


extern LEDDATARGB LEDDATA[LED_NUM];
extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;
HAL_StatusTypeDef LED_Init(){

	HAL_StatusTypeDef halStatus = HAL_TIM_PWM_Init(&LED_TIM);
	for(uint16_t i=0; i<DMABUFLEN;i++){
		LEDDMABUF[i]=0;
	}

	DMA_COMPLETE_FLAG=1;


	return halStatus;




}



void LED_Setcolour(uint8_t index, uint8_t r, uint8_t g, uint8_t b){
	uint16_t dmabufindex=0;

	LEDDATA[index].COLOUR.b=b;
	LEDDATA[index].COLOUR.r=r;
	LEDDATA[index].COLOUR.g=g;



	for(uint8_t i=0; i<LED_NUM;i++){
		for(uint8_t j=0; j<BIT_PER_LED;j++){
			if((LEDDATA[i].data>>j)&0x01){
				LEDDMABUF[dmabufindex]=HI_VAL;
			}
			else{
				LEDDMABUF[dmabufindex]=LOW_VAL;
			}
			dmabufindex++;


		}
	}

}


HAL_StatusTypeDef LED_Update(){
	HAL_StatusTypeDef halStatus = HAL_TIM_PWM_Start_DMA(&LED_TIM, LED_TIM_CHANNEL, (uint32_t *)LEDDMABUF,DMABUFLEN);

	if(halStatus=HAL_OK){
		DMA_COMPLETE_FLAG=0;
	}


	return halStatus;

}



void LED_Callback(){

	HAL_TIM_PWM_Stop_DMA(&LED_TIM, LED_TIM_CHANNEL);
	DMA_COMPLETE_FLAG=1;


}
