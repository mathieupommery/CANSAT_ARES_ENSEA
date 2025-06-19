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
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "app_fatfs.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bmp581.h"
#include "6axis.h"
#include "tarvos.h"
#include "led.h"
#include "statemachine.h"
#include "servo.h"
#include "ssd1306.h"
#include "math.h"
#include "GNSS.h"
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



uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];
uint8_t tarvos_DATA[64];
uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];

uint8_t workingbuffer[110];
uint8_t sdcardbuffer[256];
AXIS6 myData6AXIS;
BMP_t myDatabmp581;


FATFS FatFs;   // FATFS handle
FRESULT fres;  // Common result code
FILINFO fno;	  // Structure holds information
FATFS *getFreeFs; 	  // Read information
FIL fil;
DIR dir;			  // Directory object structure
DWORD free_clusters;  // Free Clusters
DWORD free_sectors;	  // Free Sectors
DWORD total_sectors;


int tbtn1=0;
uint16_t rawADCdata[3];
float temp=0;
float vrefint=0;
float vbat=0;

#ifdef PARTIE_BAS
DecodedPayload TOPData;
#endif


DecodedPayload OTHERData;

GNSS_StateHandle GNSSData;

uint8_t LEDDMABUF[DMABUFLEN];
uint8_t DMA_COMPLETE_FLAG=0;

int gros_btn_time=0;

STATE state=IDLE;


int flag_drop = 0;
int flag_separation = 0;
int flag_calib=0;
int flag_bouton_servo=0;
int flag_servo_started=0;
int sd_detect_flag=0;

float hauteur_Initiale=0.0;

float hauteur_servo=0.0;

int pbmseeker=0;



int received_flag=0;

int trameready=0;

uint32_t timeindex=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
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
#ifdef PARTIE_HAUT
			flag_bouton_servo=0;
#endif

							}
					if(gros_btn_time>=400 && gros_btn_time<=2000){
#ifdef PARTIE_HAUT
						flag_bouton_servo=1;
#endif
					}
					if(gros_btn_time>=2500 && gros_btn_time<=3500){
#ifdef PARTIE_HAUT
						//hauteur_Initiale=myDatabmp581.altitude;
						if(GNSSData.fixType>=3){
							hauteur_Initiale=GNSSData.fhMSL;
						}

						create_and_send_payload((uint8_t *) tarvos_TX_Buffer,0x82,GROUND_ADDR,0x20,0,0.0,0.0,GNSSData.fhMSL,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0);
						flag_calib=1;
#endif

										}
					if(gros_btn_time>=4000){
											HAL_GPIO_TogglePin(PWEN_GPIO_Port,PWEN_Pin);
											gros_btn_time=0;

															}

					gros_btn_time=0;
					}


	}

#ifdef PARTIE_HAUT
	if(GPIO_Pin==GPIO_PIN_1){

		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)== GPIO_PIN_SET){
			flag_drop=0;
		}
		else{
			flag_drop=1;
		}


		}
#endif

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
  MX_TIM2_Init();
  MX_SPI1_Init();
  if (MX_FATFS_Init() != APP_OK) {
    Error_Handler();
  }
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  ssd1306_Init();
  ssd1306_Fill(Black);
  ssd1306_SetCursor(32, 40);
  ssd1306_WriteString("Init...", Font_7x10, White);
  ssd1306_UpdateScreen();

  HAL_GPIO_WritePin(PWEN_GPIO_Port,PWEN_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_ROUGE_GPIO_Port,LED_ROUGE_Pin,GPIO_PIN_SET);


#ifdef PARTIE_HAUT
  if( release_mecanism()!=HAL_OK){
	  ssd1306_SetCursor(32, 40);
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("servopbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }
  flag_servo_started=1;


#endif



  if(HAL_ADC_Start_DMA(&hadc1,(uint16_t*)rawADCdata, 3)!=HAL_OK){
	  ssd1306_SetCursor(32, 40);
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("adcpbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }

   HAL_Delay(100);
   HAL_UART_Abort(&huart1);
   if( HAL_UART_Receive_DMA(&huart1, (uint8_t *)workingbuffer, 100)!=HAL_OK){
	   ssd1306_SetCursor(32, 40);
  	  ssd1306_Fill(Black);
  	  ssd1306_WriteString("gpspbm", Font_7x10, White);
  	  ssd1306_UpdateScreen();
  	  HAL_Delay(500);
  	pbmseeker=1;


    }


  if(  LED_Init()!=HAL_OK){
	  ssd1306_SetCursor(32, 40);
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("ledpbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }
  Init_HighPerf_Mode_6_axis();



  if(bmp581_init_precise_normal(&myDatabmp581)!=HAL_OK){
	  ssd1306_SetCursor(32, 40);
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("bmp pbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }

  //mon adresse est 1° et on envoit à 2°
#ifdef PARTIE_BAS
  if( INIT_PERM_TARVOS(BOTTOM_ADDR, GROUND_ADDR)!=HAL_OK){
	  ssd1306_SetCursor(32, 40);
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("tvsaddrpbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }
#endif

#ifdef PARTIE_HAUT
  if( INIT_PERM_TARVOS(TOP_ADDR, BOTTOM_ADDR)!=HAL_OK){
	  ssd1306_SetCursor(32, 40);
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("tvsaddrpbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	 pbmseeker=1;


   }
#endif

  HAL_Delay(100);
  HAL_UART_Abort(&hlpuart1);
  if(HAL_UART_Receive_DMA(&hlpuart1, (uint8_t *)tarvos_RX_Buffer,5)!=HAL_OK){
	   ssd1306_SetCursor(32, 40);
 	  ssd1306_Fill(Black);
 	  ssd1306_WriteString("tvsrxpbm", Font_7x10, White);
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(500);
 	pbmseeker=1;


   }
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_Delay(10);

  fres = f_mount(&FatFs, "", 1);
  		  if (fres == FR_OK) {
  			  fres = f_mkdir("DEMO");
  			  BYTE readBuf[30];
  			  fres = f_open(&fil, "/DEMO/write.txt",FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
  			  if (fres == FR_OK) {
  				  //snprintf((char*) readBuf,30, "I hate Java!");
  				  UINT bytesWrote;
  				  fres = f_write(&fil,(uint8_t *)"test123test", 11, &bytesWrote);
  				  f_close(&fil);

  			  }
  			  f_mount(NULL, "", 0);
  		  }



  /* USER CODE END 2 */

  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


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
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
