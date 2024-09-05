/*
 * statemachine.h
 *
 *  Created on: May 27, 2024
 *      Author: mathi
 */

#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_



#endif /* INC_STATEMACHINE_H_ */

#include "main.h"
#include "QMC5883.h"
#include "ssd1306.h"
#include "math.h"



typedef enum{
	STATE_INIT,
	STATE_SPEED,
	STATE_HEURE,
	STATE_INFO,
	STATE_DISTANCEA,
	STATE_CHOOSE

}STATE_TYPE;

void statemachine(void);

typedef enum{
	STATE_HEURES,
	STATE_MINUTE,
	STATE_VALID

}COMPTEUR;



typedef enum{
	STATE_CHOOSE_LATITUDE,
	STATE_CHOOSE_LONGITUDE,
	STATE_CHOOSE_DISTANCE,
	STATE_VALID1

}CHOOSEDISTANCE;

typedef enum{
	STATE_VILLE1,
	STATE_VILLE2


}DISTANCEVILLE;
