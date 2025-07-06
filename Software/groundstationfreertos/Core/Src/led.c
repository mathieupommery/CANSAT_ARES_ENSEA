/*
 * led.c
 *
 *  Created on: Mar 14, 2025
 *      Author: mathi
 */


#include "led.h"


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



void LED_Setcolour(uint8_t r, uint8_t g, uint8_t b,uint8_t r1, uint8_t g1, uint8_t b1){
	uint16_t dmabufindex=0;
	uint8_t ledbuf[LED_NUM*3];
	ledbuf[0]=g;
	ledbuf[1]=r;
	ledbuf[2]=b;
	ledbuf[3]=g1;
	ledbuf[4]=r1;
	ledbuf[5]=b1;

	for(int i=0;i<DMABUFLEN;i++){
			LEDDMABUF[i]=0;
		}



	for(uint8_t i=0; i<LED_NUM;i++){
		for(uint8_t j=0; j<3;j++){
			for(int k=0;k<8;k++){
			if((ledbuf[(3*i)+j]>>k)&0x01){
				LEDDMABUF[dmabufindex]=HI_VAL;
			}
			else{
				LEDDMABUF[dmabufindex]=LOW_VAL;
			}
			dmabufindex++;
			}


		}
	}

	for(int i=0;i<RSTPERIOD;i++){
		LEDDMABUF[dmabufindex]=0;
		dmabufindex++;
	}

	for(int i=0;i<5;i++){
			LEDDMABUF[dmabufindex]=0;
			dmabufindex++;
		}
	LED_Update();

}


HAL_StatusTypeDef LED_Update(){
	HAL_StatusTypeDef halStatus = HAL_TIM_PWM_Start_DMA(&LED_TIM, LED_TIM_CHANNEL, (uint32_t *)LEDDMABUF,DMABUFLEN);

	if(halStatus==HAL_OK){
		DMA_COMPLETE_FLAG=0;
	}


	return halStatus;

}



void LED_Callback(){

	HAL_TIM_PWM_Stop_DMA(&LED_TIM, LED_TIM_CHANNEL);
	DMA_COMPLETE_FLAG=1;


}
