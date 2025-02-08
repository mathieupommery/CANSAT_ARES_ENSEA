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
#include "adc.h"
#include "dma.h"
#include "app_fatfs.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fatfs_sd.h"
#include "nmea_parse.h"
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
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t total, free1;
uint8_t buffer[100] = "";

uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];
uint8_t tarvos_TX_Tampon[TarvosTxTamponSize];
uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];
uint8_t tarvos_RX_Tampon[TarvosRxTamponSize];
uint8_t GPS_RX_Buffer[GPSRxBufferSize];
uint8_t GPS_RX_Tampon[GPSRxTamponSize];

uint8_t SD_Card_Write_Buffer[256];

int GPSbufferoldpos=0;
int GPSbuffernewpos=0;
int TarvosRXbufferoldpos=0;
int TarvosRXbuffernewpos=0;

uint8_t commandtarvos[]={0x02,0x00,};


uint16_t rawADCdata[5];
float temp=0;
float vrefint=0;
float vbat=0;
float current=0;
float power=0;
float vexternalsensor=0;

char envoi[200];
char accel_gyro[60];
char temperature[20];
uint16_t accel_data[3];
uint16_t gyro_data[3];

extern int flag_envoi;
extern int flag_sensor;
double temp_data;


GPS myData;
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
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  if (MX_FATFS_Init() != APP_OK) {
    Error_Handler();
  }
  MX_I2C3_Init();
  MX_LPUART1_UART_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */

  HAL_ADC_Start_DMA(&hadc1,(uint16_t*)rawADCdata, 5);
  HAL_ADC_Start_DMA(&hadc2,(uint16_t*)rawADCdata+4, 1);
  HAL_TIM_Base_Start_IT(&htim4);

  TIM3->CCR1=0;
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);


  HAL_Delay(100);
  HAL_UART_Abort(&huart1);
  HAL_UART_Receive_DMA(&huart1, (uint8_t *)GPS_RX_Tampon, GPSRxTamponSize);

  HAL_Delay(100);
  HAL_UART_Abort(&hlpuart1);
  HAL_UART_Receive_DMA(&hlpuart1, (uint8_t *)tarvos_RX_Tampon, TarvosRxTamponSize);




  f_mount(&fs, "", 0);

// f_open(&fil, "debut.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
//
// f_getfree("", &fre_clust, &pfs);
//
//  total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
//  free1 = (uint32_t)(fre_clust * pfs->csize * 0.5);
//
//  snprintf((uint8_t *) SD_Card_Write_Buffer,256,"adc1=%d,adc2=%d,adc3=%d,adc4=%d,adc5=%d\n\r",rawADCdata[0],rawADCdata[1],rawADCdata[2],rawADCdata[3],rawADCdata[4]);
//
//  f_puts("test", &fil);
//  f_puts((uint8_t *) SD_Card_Write_Buffer, &fil);
//  snprintf((uint8_t *) SD_Card_Write_Buffer,256,"adc1=%d,adc2=%d,adc3=%d,adc4=%d,adc5=%d\n\r",rawADCdata[0],rawADCdata[1],rawADCdata[2],rawADCdata[3],rawADCdata[4]);
//  f_puts((uint8_t *) SD_Card_Write_Buffer, &fil);
//  f_close(&fil);
//  /* Unmount SDCARD */
//  f_mount(NULL, "", 1);


  int indexboucle=0;
  int tarvosen=0;
  f_open(&fil, "adc.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

  		   f_getfree("", &fre_clust, &pfs);





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  Init_HighPerf_Mode_6_axis();
  INIT_PERM_TARVOS(0x16, 0x15); //mon adresse est 1° et on envoit à 2°

  while (1)
   {
	if (flag_sensor == 1){
		temp_data = Read_temp();
		Read_sensor_data(accel_data,gyro_data);
		conversion_char(envoi);
		nmea_parse(&myData, (uint8_t *) GPS_RX_Buffer);
		SEND_DATA_NETW(envoi, 0x82, 0x15, strlen(envoi));

	}
	if (vbat <= 6.4){
		SEND_DATA_NETW("$2,batterie faible#", 0x82, 0x15, strlen("$2,batterie faible#"));

	}
//	if(TIM3->CCR1>=120){
//		TIM3->CCR1=65;
//
//	}
	TIM3->CCR1=1530;

	HAL_Delay(100);



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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void uint16_to_char_buffer(uint16_t *source, char *destination, size_t taille) {
  for (size_t i = 0; i < taille; i++) {
    destination[i] = (char)source[i];
  }
}

void conversion_char(char *envoi /*,uint16_t accel_data,uint16_t gyro_data,uint8_t position_data,uint8_t time_data,uint8_t baro_data, uint8_t temp_data*/){
    memset((char* )envoi, '\0', sizeof(envoi));
    size_t len;
    envoi[0]='$';
    envoi[1]='1';
    mef_accel_gyro((char* )accel_gyro,sizeof(accel_gyro),accel_data,gyro_data);
    len = strlen(envoi);
    for (int i = 0; i<(sizeof(accel_gyro));i++){
    	envoi[i+len] = accel_gyro[i];
    }
    len = strlen(envoi);
    mef_temp((char* )temperature, sizeof(temperature),temp_data);
    for (int i = 0; i<(sizeof(temperature));i++){
        	envoi[i+len] = temperature[i];
    }
    envoi[strlen(envoi)]='#';
}

void mef_accel_gyro(char *buffer, size_t buffer_size, uint16_t accel_data[3], uint16_t gyro_data[3]) {
    int len = snprintf(buffer, buffer_size, "%d,%d,%d,%d,%d,%d,",
                       accel_data[0], accel_data[1], accel_data[2],
                       gyro_data[0], gyro_data[1], gyro_data[2]);

    // S'assurer que la chaîne est bien terminée
    if (len >= buffer_size) {
        buffer[buffer_size - 1] = '\0';
    }
}

void mef_temp(char *buffer, size_t buffer_size, float temperature) {
    snprintf((char* )buffer, buffer_size, "%0.1lf",(double) temperature);
}
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
