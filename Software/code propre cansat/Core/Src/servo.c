/*
 * servo.c
 *
 *  Created on: Mar 23, 2025
 *      Author: mathi
 */

#include "servo.h"
#include "tim.h"




HAL_StatusTypeDef Init_servo(void){

	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,OPEN_HIGH_TIME);
	  return HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);




}
void release_mecanism(void){
	 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,OPEN_HIGH_TIME);


}
void lock_mecanism(void){
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,CLOSED_HIGH_TIME);


}
HAL_StatusTypeDef set_duty(int Duty){

	if(Duty <= 1000 || Duty >= 2000){
		return HAL_ERROR;
	}

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,Duty);

	return HAL_OK;
}
