/*
 * 6axis.h
 *
 *  Created on: Jan 31, 2025
 *      Author: louisvoz
 */

#ifndef INC_6AXIS_H_
#define INC_6AXIS_H_

/*******************************************************************************
 6 axis data
 ******************************************************************************/

#include "main.h"

#define SENSOR_ADDRESS  0x6B << 1

#define dTime 10;
#define CTRL1_XL 0x10
#define CTRL2_G  0x11
#define CTRL3_C  0x12
#define CTRL6_C  0x15
#define CTRL7_G  0x16
#define CTRL8_XL 0x17
#define STATUS_REG 0x1E
#define G_X_OUT_L 0x22             // Adresse du premier registre de sortie gyroscope (G_)
#define XL_X_OUT_L 0x28            // Adresse du premier registre de sortie accéléromètre (XL_)


// Sensibilités pour accéléromètre et gyroscope
#define ACC_SENSITIVITY_16G 0.488 // Sensibilité pour ±16g
#define GYRO_SENSITIVITY_2000DPS 70 // Sensibilité pour ±2000 dps


typedef struct AXIS6_DATA {
    float AccelX;
    float AccelY;
    float AccelZ;
    int GyroX;
    int GyroY;
    int GyroZ;
    double temp;
} AXIS6;


double Read_temp();
HAL_StatusTypeDef Read_sensor_data(AXIS6 *data);
void Init_HighPerf_Mode_6_axis(void);
#endif /* INC_6AXIS_H_ */
