/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tarvos.h"
#include "led.h"
#include "statemachine.h"
#include "math.h"
#include "usart.h"
#include "lcd1602.h"
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
/* USER CODE BEGIN Variables */


extern uint16_t rawADCdata[3];
extern float temp;
extern float vrefint;
extern float vbat;


extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];
extern uint8_t tarvos_DATA[64];
extern uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];
extern uint8_t sdcardbuffer[256];

extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;

extern STATE state;

extern int flag_drop;
extern int flag_separation;
extern int sd_detect_flag;
extern int pbmseeker;

extern int trameready;

extern int received_flag;
extern DecodedPayload OTHERData;



/* USER CODE END Variables */
osThreadId statemachineHandle;
osThreadId SdcardwriteHandle;
osThreadId distancecalcHandle;
osThreadId tarvosDecodeHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Startstatemachine(void const * argument);
void StartSdcard(void const * argument);
void Startdistancecalc(void const * argument);
void startTarvosDecode(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of statemachine */
  osThreadDef(statemachine, Startstatemachine, osPriorityNormal, 0, 256);
  statemachineHandle = osThreadCreate(osThread(statemachine), NULL);

  /* definition and creation of Sdcardwrite */
  osThreadDef(Sdcardwrite, StartSdcard, osPriorityNormal, 0, 256);
  SdcardwriteHandle = osThreadCreate(osThread(Sdcardwrite), NULL);

  /* definition and creation of distancecalc */
  osThreadDef(distancecalc, Startdistancecalc, osPriorityNormal, 0, 256);
  distancecalcHandle = osThreadCreate(osThread(distancecalc), NULL);

  /* definition and creation of tarvosDecode */
  osThreadDef(tarvosDecode, startTarvosDecode, osPriorityHigh, 0, 256);
  tarvosDecodeHandle = osThreadCreate(osThread(tarvosDecode), NULL);

  /* USER CODE BEGIN RTOS_THREADS */

  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Startstatemachine */
/**
  * @brief  Function implementing the statemachine thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Startstatemachine */
void Startstatemachine(void const * argument)
{
  /* USER CODE BEGIN Startstatemachine */
  /* Infinite loop */
  for(;;)
  {
	  statemachine();

	  create_and_send_payload((uint8_t*) tarvos_TX_Buffer,CHANNEL,SAT_ADDR,0x29,0,0,0.0,0.0,0.0,0.0,0.0,0.0,0);




    osDelay(1000);
  }
  /* USER CODE END Startstatemachine */
}

/* USER CODE BEGIN Header_StartSdcard */
/**
* @brief Function implementing the Sdcardwrite thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSdcard */
void StartSdcard(void const * argument)
{
  /* USER CODE BEGIN StartSdcard */
  /* Infinite loop */
  for(;;)
  {

	  if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET){
		  sd_detect_flag=1;










	  }
	  else{sd_detect_flag=0;}






    osDelay(100);
  }
  /* USER CODE END StartSdcard */
}

/* USER CODE BEGIN Header_Startdistancecalc */
/**
* @brief Function implementing the distancecalc thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Startdistancecalc */
void Startdistancecalc(void const * argument)
{
  /* USER CODE BEGIN Startdistancecalc */
  /* Infinite loop */
  for(;;)
  {

    osDelay(100);
  }
  /* USER CODE END Startdistancecalc */
}

/* USER CODE BEGIN Header_startTarvosDecode */
/**
* @brief Function implementing the tarvosDecode thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startTarvosDecode */
void startTarvosDecode(void const * argument)
{
  /* USER CODE BEGIN startTarvosDecode */
  /* Infinite loop */
  for(;;)
  {


	  if(trameready==1){

		  switch(tarvos_DATA[3]){
		  case BOTTOM_ADDR:
			  decode_payload(&OTHERData,(uint8_t *) tarvos_DATA);
			  break;
		  default:


			  break;
		  }
		  trameready=0;
	  }


    osDelay(100);
  }
  /* USER CODE END startTarvosDecode */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

