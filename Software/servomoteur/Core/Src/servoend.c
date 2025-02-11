/*
 * servoend.c
 *
 *  Created on: Oct 22, 2024
 *      Author: ybena
 */

#include <stm32l4xx_hal.h>
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;
#include "servoenda.h"
#include <stdio.h>

//début 25
//fin 250
void rote(int deg,int arr_value){
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,(5+5*deg/180)*arr_value/100);//mathématiquement
}
//0° à 25 et 180° à 250
//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1, 25+deg*225/180);
void duty_cycle(int alpha, int arr_value){
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,alpha*arr_value/100);
}
