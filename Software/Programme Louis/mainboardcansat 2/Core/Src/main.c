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
#include "baro.h"

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
#define PIN_ENABLE GPIO_PIN_15
#define LED_ROUGE GPIO_PIN_14

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

uint8_t usart_buffer[BUFFER_SIZE];


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
char accel_gyro[33];
char temperature_6axis[20];
char temperature_baro[20];
char pression[20];

uint16_t accel_data[3];
uint16_t gyro_data[3];

extern int flag_envoi;
extern int flag_sensor;
int erreur ;
double temp_data;

GPS myDataGPS;
AXIS6 myData6AXIS ;
BARO myDataBaro ;

char pos[10];
char altitude[10];

extern int position_servo;
extern int flag_servo;
int flag_ouverture = 0;

int pointnull = 123;
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */



  HAL_ADC_Start_DMA(&hadc1,(uint16_t*)rawADCdata, 5);
  HAL_ADC_Start_DMA(&hadc2,(uint16_t*)rawADCdata+4, 1);
  HAL_TIM_Base_Start_IT(&htim4);

  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,1520);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  HAL_GPIO_WritePin(GPIOB,PIN_ENABLE,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB,LED_ROUGE,GPIO_PIN_RESET);



  HAL_Delay(100);
  HAL_UART_Abort(&huart1);
  HAL_UART_Receive_DMA(&huart1, (uint8_t *)GPS_RX_Tampon, GPSRxTamponSize);

  HAL_Delay(100);
  HAL_UART_Abort(&hlpuart1);
  HAL_UART_Receive_DMA(&hlpuart1, (char*)usart_buffer, BUFFER_SIZE);




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
  INIT_PERM_TARVOS(0x15, 0x16); //mon adresse est 1° et on envoit à 2°
  INIT_BARO();

  while (1)
   {
	//Servo entre 1350 et 1550

	nmea_parse(&myDataGPS,(uint8_t*) GPS_RX_Buffer);
	//Évitement de l'ouverture
	if (myDataGPS.altitude - pointnull > 60 && flag_ouverture == 0){
		flag_ouverture = 1 ;
	}
	//Séparation des parties du CANSAT
	if (myDataGPS.altitude - pointnull < 60 && myDataGPS.altitude != 0 && flag_ouverture == 1) {
	    char trame[51];  // Augmenté pour inclure l'astérisque et le '\0'
	    char altitude_str[10];

	    // Conversion de l'altitude en chaîne de caractères
	    strcpy(altitude_str, conv_float_to_char(myDataGPS.altitude));

	    // Construction du message
	    strcpy(trame, "$2,le parachute s'est ouvert à ");
	    strcat(trame, altitude_str);
	    strcat(trame, "*");

	    // Action sur le timer
	    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1350);

	    // Envoi du message
	    //SEND_DATA_NETW(trame, 0x82, 0x16, strlen(trame));
	}
	//Lecture de la valeur du servo
	if (flag_servo == 1){
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,position_servo);
		//HAL_Delay(1000);
		flag_servo = 0 ;
	}
	//Batterie faible
	if (vbat <= 7){
		//SEND_DATA_NETW("$2,batterie faible#", 0x82, 0x16, strlen("$2,batterie faible#"));
	}
	// Problème de GPS
	if (myDataGPS.fix == 0){
		SEND_DATA_NETW("$2,pas de GPS#", 0x82, 0x16, strlen("$2,pas de GPS#"));
	}
	//Envoi des données selon le timer
	if (flag_sensor == 1){
		Read_sensor_data(&myData6AXIS);
//		BARO_FILL(&myDataBaro);
		conversion_char(envoi);
		SEND_DATA_NETW("$23,42,N,2,E,14,120,1013,2025,01,18,14,30,45*", 0x82, 0x16, strlen("$23,42,N,2,E,14,120,1013,2025,01,18,14,30,45*"));

		//SEND_DATA_NETW(envoi, 0x82, 0x16, strlen(envoi));
		flag_sensor = 0;
	}
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

void conversion_char(char *envoi){
    size_t len;
    memset(envoi, '0', strlen(envoi));  // Remplit la chaîne de '0'
    envoi[0]='$';
    envoi[1]='1';
    envoi[2]=',';
    snprintf(accel_gyro, accel_gyro, "%u , %u , %u , %u,%u,%u,",
                           myData6AXIS.AccelX, myData6AXIS.AccelY, myData6AXIS.AccelZ,
    					   myData6AXIS.GyroX, myData6AXIS.GyroY, myData6AXIS.GyroZ);
    convertIntToChar(myData6AXIS, accel_gyro, 20);
    len = strlen(envoi);
    for (int i = 0; i<(sizeof(accel_gyro));i++){
    	envoi[i+len] = accel_gyro[i];
    }
    len = strlen(envoi);
    conv_float_to_char(temperature_6axis, sizeof(temperature_6axis),myData6AXIS.temp);
    for (int i = 0; i<(sizeof(temperature_6axis));i++){
        	envoi[i+len] = temperature_6axis[i];
    }
    envoi[strlen(envoi)]=',';
    len = strlen(envoi);
    mef_pos(pos, sizeof(pos),myDataGPS.latitude);
    for (int i = 0; i<(sizeof(pos));i++){
            	envoi[i+len] = pos[i];
    }
    envoi[strlen(envoi)]=',';
    len = strlen(envoi);
	mef_pos(pos, sizeof(pos),myDataGPS.longitude);
	for (int i = 0; i<(sizeof(pos));i++){
				envoi[i+len] = pos[i];
	}
    envoi[strlen(envoi)]=',';
    len = strlen(envoi);
    conv_float_to_char(altitude,sizeof(altitude),myDataGPS.altitude);
    for (int i = 0; i<(sizeof(altitude));i++){
    				envoi[i+len] = altitude[i];
    }
    envoi[strlen(envoi)]=',';
	len = strlen(envoi);
	conv_float_to_char(pression,sizeof(pression),myDataBaro.Pression);
	for (int i = 0; i<(sizeof(pression));i++){
					envoi[i+len] = pression[i];
	}
	envoi[strlen(envoi)]=',';
	len = strlen(envoi);
	conv_float_to_char(temperature_baro,sizeof(temperature_baro),myDataBaro.Temperature);
	for (int i = 0; i<(sizeof(temperature_baro));i++){
					envoi[i+len] = temperature_baro[i];
	}
    envoi[strlen(envoi)]='#';
}



void conv_float_to_char(char *buffer, size_t buffer_size, float temperature) {
    snprintf(buffer, buffer_size, "%.1f", temperature);
}

void mef_pos(char *buffer, size_t buffer_size, double position) {
    snprintf(buffer, buffer_size, "%.7f", position);
}

void Set_Servo_Position(uint16_t pulse_width) {
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse_width);
}

void convertIntToChar(AXIS6 data, char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size,
             "%d,%d,%d,%d,%d,%d",
             data.AccelX, data.AccelY, data.AccelZ,
             data.GyroX, data.GyroY, data.GyroZ);
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
