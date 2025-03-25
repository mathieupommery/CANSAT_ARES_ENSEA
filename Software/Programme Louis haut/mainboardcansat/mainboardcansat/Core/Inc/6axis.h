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
typedef struct AXIS6_DATA {
    int AccelX;
    int AccelY;
    int AccelZ;
    int GyroX;
    int GyroY;
    int GyroZ;
    double temp;
} AXIS6;

extern float angle[3];

double Read_temp();
HAL_StatusTypeDef Read_sensor_data(AXIS6 *data);
void Init_HighPerf_Mode_6_axis(void);
#endif /* INC_6AXIS_H_ */
