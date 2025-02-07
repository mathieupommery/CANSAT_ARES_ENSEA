/*
 * 6axis.h
 *
 *  Created on: Jan 31, 2025
 *      Author: louisvoz
 */

#ifndef INC_6AXIS_H_
#define INC_6AXIS_H_

double Read_temp();
HAL_StatusTypeDef Read_sensor_data(int16_t* accel_data, int16_t* gyro_data);
void Init_HighPerf_Mode_6_axis(void);

#endif /* INC_6AXIS_H_ */
