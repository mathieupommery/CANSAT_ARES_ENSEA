/*
 * baro.h
 *
 *  Created on: Feb 27, 2025
 *      Author: louisvoz
 */

typedef struct BARO_DATA {
    float Pression;
    float Temperature;
    float Hauteur;
} BARO;

void READ_PRES(int32_t* temperature, int32_t* pression);
void INIT_BARO();
