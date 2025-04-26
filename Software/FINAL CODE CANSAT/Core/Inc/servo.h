/*
 * servo.h
 *
 *  Created on: Mar 23, 2025
 *      Author: mathi
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "main.h"



#define CLOSED_HIGH_TIME 1350
#define OPEN_HIGH_TIME 1520


HAL_StatusTypeDef Init_servo(void);
void release_mecanism(void);
void lock_mecanism(void);
HAL_StatusTypeDef set_duty(int Duty);


#endif /* INC_SERVO_H_ */
