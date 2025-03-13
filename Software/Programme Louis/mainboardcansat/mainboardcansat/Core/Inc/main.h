/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void uint16_to_char_buffer(uint16_t *source, char *destination, size_t taille);
void mef_accel_gyro(char *buffer, size_t buffer_size, uint16_t accel_data[3], uint16_t gyro_data[3]);
void mef_pos(char *buffer, size_t buffer_size, double position);
void Set_Servo_Position(uint16_t pulse_width);
void convertIntToChar(AXIS6 data, char *buffer, size_t buffer_size);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
#define GPSRxBufferSize 512
#define GPSRxTamponSize 64
#define TarvosRxBufferSize 256
#define TarvosTxBufferSize 256
#define TarvosRxTamponSize 30
#define TarvosTxTamponSize 8

extern int hauteur_0;
#define P0 101325.0  // Pression au niveau de la mer en Pascals
#define L 0.0065     // Gradient thermique en K/m
#define T0 288.15    // Température au niveau de la mer en Kelvin
#define g 9.80665    // Accélération gravitationnelle en m/s²
#define M 0.0289644  // Masse molaire de l'air en kg/mol
#define R 8.31432    // Constante des gaz parfaits en J/(mol·K)


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
