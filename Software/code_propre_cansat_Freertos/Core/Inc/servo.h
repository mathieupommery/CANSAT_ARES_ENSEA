/*
 * servo.h
 *
 *  Created on: Mar 23, 2025
 *      Author: mathi
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "main.h"



#define CLOSED_HIGH_TIME 1450
#define OPEN_HIGH_TIME 1280


HAL_StatusTypeDef Init_servo(void);
HAL_StatusTypeDef release_mecanism(void);
HAL_StatusTypeDef lock_mecanism(void);
HAL_StatusTypeDef set_duty(int Duty);
void stop_servo(void);


#endif /* INC_SERVO_H_ */
