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

uint8_t lcdbuffer[64];

extern uint16_t rawADCdata[3];
extern float temp;
extern float vrefint;
extern float vbat;


extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];
extern uint8_t tarvos_DATA[64];
extern uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];
extern uint8_t sdcardbuffer[512];

extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;

extern int flag_drop;
extern int flag_separation;
extern int flag_calib;
extern int flag_fin;
extern int pbmseeker;

extern int trameready;

int sd_counter=0;


#ifdef PARTIE_GROUND
extern DecodedPayload TOPData;
extern DecodedPayload BOTTOMData;
extern DecodedPayload SATData;
extern uint8_t tarvos_TOPDATA[64];
extern uint8_t tarvos_BOTTOMDATA[64];
extern uint8_t tarvos_SATDATA[64];
uint8_t uartsendbuffer[256];
int sendsize;
int onetimeflag=0;
extern uint8_t dma_rx_buffer[DMA_CHUNK_SIZE];               // Réception DMA par bloc de 5
extern uint8_t circular_buffer[CIRC_BUF_SIZE];              // Buffer circulaire
extern volatile uint16_t write_index;
extern volatile uint16_t read_index;
#endif



int pbmseeker_flag=0;
int blinker_sd_flag=0;

extern uint32_t timeindex;


int led_flag=0;

UBaseType_t sizestatemachine;
UBaseType_t sizedist;
UBaseType_t sizesdcard;
UBaseType_t sizetarvos;

/* USER CODE END Variables */
osThreadId statemachineHandle;
osThreadId SdcardwriteHandle;
osThreadId distancecalcHandle;
osThreadId tarvosDecodeHandle;
osMutexId SDCard_mutexeHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Startstatemachine(void const * argument);
void StartSdcard(void const * argument);
void Startdistancecalc(void const * argument);
void StarttarvosDecode(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of SDCard_mutexe */
  osMutexDef(SDCard_mutexe);
  SDCard_mutexeHandle = osMutexCreate(osMutex(SDCard_mutexe));

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
  osThreadDef(statemachine, Startstatemachine, osPriorityAboveNormal, 0, 512);
  statemachineHandle = osThreadCreate(osThread(statemachine), NULL);

  /* definition and creation of Sdcardwrite */
  osThreadDef(Sdcardwrite, StartSdcard, osPriorityNormal, 0, 512);
  SdcardwriteHandle = osThreadCreate(osThread(Sdcardwrite), NULL);

  /* definition and creation of distancecalc */
  osThreadDef(distancecalc, Startdistancecalc, osPriorityNormal, 0, 512);
  distancecalcHandle = osThreadCreate(osThread(distancecalc), NULL);

  /* definition and creation of tarvosDecode */
  osThreadDef(tarvosDecode, StarttarvosDecode, osPriorityHigh, 0, 512);
  tarvosDecodeHandle = osThreadCreate(osThread(tarvosDecode), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  osThreadSuspend(distancecalcHandle);
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
	TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {

	  if(flag_fin==0){

		  if(led_flag==0){
			  LED_Setcolour(255, 0, 0,0, 0, 255);
			  LED_Update();

		  }
		  else{
			  LED_Setcolour(0, 0, 255,255, 0, 0);
			  LED_Update();
		  }
		  led_flag=1-led_flag;

		 sendsize=snprintf((char *)uartsendbuffer,256,"%d,%d,%d,%d,%0.7f,%0.7f,%0.7f,%0.7f,%0.1f,%0.1f,%0.2f,%0.2f,%0.0f,%0.0f,%0.0f,%0.0f,%0.0f,%0.0f,%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%lu\n\r",
			  TOPData.flag_calib,TOPData.flag_drop,TOPData.flag_separation,TOPData.flag_fin,
			  TOPData.latitude,BOTTOMData.latitude,TOPData.longitude,BOTTOMData.longitude,TOPData.hMSL,
			  BOTTOMData.hMSL,TOPData.altitude_baro,BOTTOMData.altitude_baro,TOPData.Accx,BOTTOMData.Accx,TOPData.Accy,BOTTOMData.Accy,TOPData.Accz,BOTTOMData.Accz,SATData.hMSL,
			  TOPData.pression,TOPData.temperature,BOTTOMData.temperature,BOTTOMData.pression,timeindex);


		HAL_UART_Transmit(&huart1, (uint8_t *)uartsendbuffer, sendsize, 100);
	  }
	  else{

if(onetimeflag==0){
		  HAL_UART_Transmit(&huart1, (uint8_t *)"fin\n\r", 5, 100);
		  onetimeflag=1;
	  }
		LED_Setcolour(0, 255, 0,0, 255, 0);
	  }
	  	vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
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
	TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  		  osMutexWait(SDCard_mutexeHandle, portMAX_DELAY);

	  		  blinker_sd_flag=1-blinker_sd_flag;
	  		  if(blinker_sd_flag==1){
	  			  LED_Setcolour(0,0,0,255,255,255);
	  		  }
	  		  else{
	  			  LED_Setcolour(0,0,0,0,0,0);
	  		  }



	  		  if(flag_drop==0){

	  			  if(sd_counter==10){
	  				  store_in_sd();

	  			  sd_counter=0;
	  			  }
	  			  sd_counter++;

	  		  }
	  		  else{
	  			  store_in_sd();

	  		  }

	  		  osMutexRelease(SDCard_mutexeHandle);

	  		  if(flag_fin==1){
	  			osThreadSuspend(NULL);
	  		  }

	  		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
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
	TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {

		  vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
  }
  /* USER CODE END Startdistancecalc */
}

/* USER CODE BEGIN Header_StarttarvosDecode */
/**
* @brief Function implementing the tarvosDecode thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StarttarvosDecode */
void StarttarvosDecode(void const * argument)
{
  /* USER CODE BEGIN StarttarvosDecode */

	TickType_t xLastWakeTime = xTaskGetTickCount();
	uint8_t temp_buf[TRAME_SIZE];
  /* Infinite loop */
  for(;;)
  {

	          while (read_index != write_index) {
	              // Vérifie entête de trame
	              if (circular_buffer[read_index] == 0x02 &&
	                  circular_buffer[(read_index + 1) % CIRC_BUF_SIZE] == 0x81)
	              {
	                  // Vérifie disponibilité d'une trame complète
	                  uint16_t available = (write_index >= read_index)
	                      ? (write_index - read_index)
	                      : (CIRC_BUF_SIZE - read_index + write_index);

	                  if (available >= TRAME_SIZE) {
	                      // Copie la trame dans un tampon local
	                      for (int i = 0; i < TRAME_SIZE; i++) {
	                          temp_buf[i] = circular_buffer[(read_index + i) % CIRC_BUF_SIZE];
	                      }

	                      // Identifier le destinataire
	                      switch (temp_buf[3]) {
	                          case TOP_ADDR:
	                              memcpy(tarvos_TOPDATA, temp_buf, TRAME_SIZE);
	                              decode_payload(&TOPData,(uint8_t *) tarvos_TOPDATA);

	                              break;
	                          case BOTTOM_ADDR:
	                              memcpy(tarvos_BOTTOMDATA, temp_buf, TRAME_SIZE);
	                              decode_payload(&BOTTOMData,(uint8_t *) tarvos_BOTTOMDATA);

	                              break;
	                          case SAT_ADDR:
	                              memcpy(tarvos_SATDATA, temp_buf, TRAME_SIZE);
	                              decode_payload(&SATData,(uint8_t *) tarvos_SATDATA);
	                              break;
	                          default:
	                              break;  // Adresse inconnue
	                      }

	                      // Avancer read_index
	                      read_index = (read_index + TRAME_SIZE) % CIRC_BUF_SIZE;
	                  } else {
	                      break;  // Attendre réception de la suite
	                  }
	              } else {
	                  // Entête invalide, on avance
	                  read_index = (read_index + 1) % CIRC_BUF_SIZE;
	              }
	          }
  vTaskDelay(pdMS_TO_TICKS(5));

		  if(flag_fin==1){
			osThreadSuspend(NULL);
		  }

  }
  /* USER CODE END StarttarvosDecode */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

