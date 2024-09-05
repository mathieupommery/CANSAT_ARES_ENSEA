/*
 * ville.C
 *
 *  Created on: Jun 5, 2024
 *      Author: mathi
 */


#include "ville.h"
#include "math.h"
#include "nmea_parse.h"


double distancepoint(GPS * gpsdata,VILLE_t villedata){
	double distance=0;
	distance=60*acosl(sinl(gpsdata->latitude)*sinl(villedata.latville)+cosl(gpsdata->latitude)*cosl(villedata.latville)*cosl((villedata.lonville)-(gpsdata->longitude)))*(1.852);

	return distance;

}

