/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nmea_parse.h"
#include "bmp581.h"
#include "6axis.h"
#include "tarvos.h"
#include "led.h"
#include "statemachine.h"
#include "servo.h"
#include "ssd1306.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int tbtn1=0;


uint16_t rawADCdata[3];
float temp=0;
float vrefint=0;
float vbat=0;


uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];

uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];
uint8_t tarvos_RX_Tampon[TarvosRxTamponSize];

uint8_t GPS_RX_Buffer[GPSRxBufferSize];
uint8_t GPS_RX_Tampon[GPSRxTamponSize];

int GPSbufferoldpos;
int GPSbuffernewpos;
int TarvosRXbufferoldpos;
int TarvosRXbuffernewpos;

GPS myDataGPS;
AXIS6 myData6AXIS;
BMP_t myDatabmp581;


uint8_t LEDDMABUF[DMABUFLEN];
uint8_t DMA_COMPLETE_FLAG=0;


uint8_t timestamp=0;

int gros_btn_time=0;

STATE state=IDLE;


int flag_drop = 0;

int hauteur_Initiale=0;
int flag_calib=0;

int pbmseeker=0;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//lors d'un appuie sur un bouton, le systeme s'interrompt afin d'arriver dans cette fonction redefinie avec en parametre d'entre , le bouton sur lequel l'on a appuiyé
{
	if(GPIO_Pin==GPIO_PIN_4){
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)== GPIO_PIN_SET){

						gros_btn_time=0;
						tbtn1=HAL_GetTick();


					}
					else{

						gros_btn_time=(HAL_GetTick()-tbtn1);


		if(gros_btn_time<=380){

			lock_mecanism();
			gros_btn_time=0;

							}
					if(gros_btn_time>=400 && gros_btn_time<=2000){

						release_mecanism();
						gros_btn_time=0;

					}
					if(gros_btn_time>=4000){
						hauteur_Initiale=myDatabmp581.altitude;
						flag_calib=1;
						gros_btn_time=0;

										}
					}


	}


//	if(GPIO_Pin==GPIO_PIN_0){
//		flag_drop=1;
//
//		}

	}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){

	LED_Callback();
}
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
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  ssd1306_Init();
  ssd1306_Fill(Black);
  ssd1306_SetCursor(32, 40);
  ssd1306_WriteString("Init...", Font_7x10, White);
  ssd1306_UpdateScreen();

  if( HAL_TIM_Base_Start_IT(&htim4)!=HAL_OK){
	  ssd1306_Fill(Black);
	  ssd1306_WriteString("tim4pbm", Font_7x10, White);
	  ssd1306_UpdateScreen();
	  HAL_Delay(500);
	  pbmseeker=1;


  }

  if( HAL_TIM_Base_Start_IT(&htim6)!=HAL_OK){
	  ssd1306_Fill(Black);
	  ssd1306_WriteString("tim6pbm", Font_7x10, White);
	  ssd1306_UpdateScreen();
	  HAL_Delay(500);
	  pbmseeker=1;


  }



  if( Init_servo()!=HAL_OK){
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("servopbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }



  if(HAL_ADC_Start_DMA(&hadc1,(uint16_t*)rawADCdata, 3)!=HAL_OK){
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("adcpbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }

   HAL_Delay(100);
   HAL_UART_Abort(&huart1);

   if( HAL_UART_Receive_DMA(&huart1, (uint8_t *)GPS_RX_Tampon, GPSRxTamponSize)!=HAL_OK){
  	  ssd1306_Fill(Black);
  	  ssd1306_WriteString("gpspbm", Font_7x10, White);
  	  ssd1306_UpdateScreen();
  	  HAL_Delay(500);
  	pbmseeker=1;


    }

   HAL_Delay(100);
   HAL_UART_Abort(&hlpuart1);

   if(HAL_UART_Receive_DMA(&hlpuart1, (uint8_t *)tarvos_RX_Tampon, TarvosRxTamponSize)!=HAL_OK){
  	  ssd1306_Fill(Black);
  	  ssd1306_WriteString("tvsrxpbm", Font_7x10, White);
  	  ssd1306_UpdateScreen();
  	  HAL_Delay(500);
  	pbmseeker=1;


    }


  if(  LED_Init()!=HAL_OK){
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("ledpbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }
  Init_HighPerf_Mode_6_axis();


//
//  if(bmp581_init_precise_normal(&myDatabmp581)!=HAL_OK){
// 	  ssd1306_Fill(Black);
// 	  ssd1306_WriteString("bmp pbm", Font_7x10, White);
// 	  ssd1306_UpdateScreen();
// 	  HAL_Delay(500);
// 	 pbmseeker=1;
//
//
//   }

  //mon adresse est 1° et on envoit à 2°
  if( INIT_PERM_TARVOS(TOP_ADDR, GROUND_ADDR)!=HAL_OK){
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("tvsaddrpbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }

  HAL_GPIO_WritePin(PWEN_GPIO_Port,PWEN_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_ROUGE_GPIO_Port,LED_ROUGE_Pin,GPIO_PIN_SET);

  if(pbmseeker==0){

	  ssd1306_Fill(Black);
	  ssd1306_WriteString("done!", Font_7x10, White);
	  ssd1306_UpdateScreen();
	  HAL_Delay(500);


  }
  if(pbmseeker==1){

	  ssd1306_Fill(Black);
	  ssd1306_WriteString("PROBLEM", Font_7x10, White);
	  ssd1306_UpdateScreen();
	  HAL_Delay(2000);


  }
  int r=0;
  int g=0;
  int b=0;
  int t=0;



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(timestamp==1){

		  nmea_parse(&myDataGPS,(uint8_t*) GPS_RX_Buffer);
		  bmp581_read_precise_normal(&myDatabmp581);
		  Read_sensor_data(&myData6AXIS);

		  statemachine();
		  timestamp=0;



	  }
	  LED_Setcolour(r, g, b,r, g, b);
	  r=floor(abs(255*cosf((2*M_PI/255)*t)));
	  g=floor(abs(255*cosf((2*M_PI/255)*t+(180/M_PI))));
	  b=floor(abs(255*cosf((2*M_PI/255)*t+(270/M_PI))));

	  if(t>=255){
		  t=0;
	  }
	  t++;

	  LED_Update();







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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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
