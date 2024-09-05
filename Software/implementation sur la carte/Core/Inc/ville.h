/*
 * ville.h
 *
 *  Created on: Jun 5, 2024
 *      Author: mathi
 */

#ifndef INC_VILLE_H_
#define INC_VILLE_H_

#include "nmea_parse.h"


#endif /* INC_VILLE_H_ */


typedef struct VILLE_DATA
{
	char nom;
	double latville;
	double lonville;
}VILLE_t;

double distancepoint(GPS * gpsdata,VILLE_t villedata);
