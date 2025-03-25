/*
 * baro.c
 *
 *  Created on: Feb 27, 2025
 *      Author: louisvoz
 */
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "app_fatfs.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "usb.h"
#include "gpio.h"
#include "6axis.h"
#include "baro.h"
//******Variable*********
//Pour la température
#define temp_7_0 Ox1D
#define temp_15_8 Ox1E
#define temp_23_16 Ox1F
//Pour la pression
#define press_7_0 Ox20
#define press_15_8 Ox21
#define press_23_16 Ox22
//Configuration
#define ODR 0x37
#define OSR 0x36
#define SENSOR_ADDRESS 0x46<<1
#define FIFO_SEL 0x18
#define FIFO_CONFIG 0x16
#define FIFO_DATA_ADDR 0x29


void INIT_BARO(){
	HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, OSR, I2C_MEMADD_SIZE_8BIT, 0b01111011, 1, HAL_MAX_DELAY);//EN la pression, opti lecture
    HAL_Delay(500);
    status = HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, ODR, I2C_MEMADD_SIZE_8BIT, 0b00000001, 1, HAL_MAX_DELAY);//240Hz, Normal mode
    HAL_Delay(500);
    status = HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, FIFO_SEL, I2C_MEMADD_SIZE_8BIT, 0b00000111, 1, HAL_MAX_DELAY);//EN FIFO
    HAL_Delay(500);
    status = HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, FIFO_CONFIG, I2C_MEMADD_SIZE_8BIT, 0b00001111, 1, HAL_MAX_DELAY);//stream-to-FIFO/threshold
    HAL_Delay(500);
}

void READ_PRES(int32_t* temperature, int32_t* pression){
	uint8_t data[6];
	HAL_I2C_Mem_Read(&hi2c1, SENSOR_ADDRESS & 1, FIFO_DATA_ADDR, I2C_MEMADD_SIZE_8BIT, data, 6, HAL_MAX_DELAY);// on lit les 48 octets
	*temperature = (data[0] & data[1]<<8 & data[2]<<16);
	*pression = (data[3] & data[4]<<8 & data[5]<<16);
}

void BARO_FILL(BARO *data){
	int32_t temperature,pression;
	READ_PRES(temperature,pression);
	data->Pression = pression / (2^8);
	data->Temperature = temperature / (2^16);
	data->Hauteur = ((T0 / L) * (1 - pow(pression / P0, (R * L) / (g * M))));


}
