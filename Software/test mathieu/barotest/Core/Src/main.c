/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PADS.h"
#include "string.h"
#include "nmea_parse.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
int a=0;
extern double finaltemp;
extern double finalpress;
extern double alt;
extern double alt1;
uint16_t oldPos=0;
uint16_t newPos=0;
uint8_t RxBuffer[RxBuffer_SIZE];
uint8_t DataBuffer[DataBuffer_SIZE];
GPS myData;

uint8_t commandgps[64];
extern UART_HandleTypeDef hlpuart1;




/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_LPUART1_UART_Init();
  /* USER CODE BEGIN 2 */


  memcpy((uint8_t*)commandgps,(uint8_t*)"$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0*",sizeof("$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0*"));
  a=checksumcalculate((uint8_t*)commandgps);
  HAL_UART_Transmit(&hlpuart1, (uint8_t*)commandgps, sizeof((uint8_t*)commandgps),HAL_MAX_DELAY);
  memset((uint8_t*)commandgps,'\0',64);
  HAL_Delay(50);
//  memcpy((uint8_t*)commandgps,(uint8_t*)"$PMTK335,2007,1,1,0,0,0*",sizeof("$PMTK335,2007,1,1,0,0,0*"));
//  a=checksumcalculate((uint8_t*)commandgps);
//  HAL_UART_Transmit_IT(&hlpuart1, (uint8_t*)commandgps, sizeof((uint8_t*)commandgps));
//  memset((uint8_t*)commandgps,'\0',64);
//    HAL_Delay(50);
//  memcpy((uint8_t*)commandgps,(uint8_t*)"$PMTK500,100,0,0,0.0,0.0*",sizeof("$PMTK500,100,0,0,0.0,0.0*"));
//  a=checksumcalculate((uint8_t*)commandgps);
//  HAL_UART_Transmit_IT(&hlpuart1, (uint8_t*)commandgps, sizeof((uint8_t*)commandgps));
//  memset((uint8_t*)commandgps,'\0',64);
//    HAL_Delay(50);
//  memcpy((uint8_t*)commandgps,(uint8_t*)"$PMTK300,100,0,0,0,0*",sizeof("$PMTK300,100,0,0,0,0*"));
//  a=checksumcalculate((uint8_t*)commandgps);
//  HAL_UART_Transmit_IT(&hlpuart1, (uint8_t*)commandgps, sizeof((uint8_t*)commandgps));
//  memset((uint8_t*)commandgps,'\0',64);
//    HAL_Delay(50);
//    memcpy((uint8_t*)commandgps,(uint8_t*)"$PMTK220,100*",sizeof("$PMTK220,100*"));
//     a=checksumcalculate((uint8_t*)commandgps);
//     HAL_UART_Transmit_IT(&hlpuart1, (uint8_t*)commandgps, sizeof((uint8_t*)commandgps));
//     memset((uint8_t*)commandgps,'\0',64);
//       HAL_Delay(50);




//  a=PADS_continuous_init(&hi2c2);
  HAL_UART_Receive_DMA(&hlpuart1, (uint8_t *)RxBuffer, RxBuffer_SIZE);

  a=checksumcalculate((uint8_t*)commandgps);











  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  //nmea_parse(&myData,(uint8_t *)DataBuffer);
	  //HAL_UART_Transmit(&huart2, (uint8_t *) DataBuffer, sizeof((uint8_t *) DataBuffer), HAL_MAX_DELAY);
	  HAL_Delay(1000);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
