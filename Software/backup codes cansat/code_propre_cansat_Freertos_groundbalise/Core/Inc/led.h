/*
 * led.h
 *
 *  Created on: Mar 14, 2025
 *      Author: mathi
 */

#ifndef INC_LED_H_
#define INC_LED_H_



#endif /* INC_LED_H_ */

#include "main.h"
#include "tim.h"


typedef enum COLOR{
	jaune,
	bleu,
	vert,
	rouge,
	blanc,
	orange,
	rose,
	violet
};

#define LED_TIM htim2
#define LED_TIM_CHANNEL TIM_CHANNEL_2

#define LED_NUM 2

#define HI_VAL 90
#define LOW_VAL 30

#define RSTPERIOD 260

#define BIT_PER_LED 24


#define DMABUFLEN ((LED_NUM*BIT_PER_LED)+RSTPERIOD)+5
//
//typedef union {
//
//	struct {
//		uint8_t g;
//		uint8_t r;
//		uint8_t b;
//	}COLOUR;
//
//	uint32_t data;
//}LEDDATARGB;



HAL_StatusTypeDef LED_Init();
void LED_Setcolour(uint8_t r, uint8_t g, uint8_t b,uint8_t r1, uint8_t g1, uint8_t b1);

HAL_StatusTypeDef LED_Update();
void LED_Callback();







