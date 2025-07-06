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
#include "servo.h"
#include "math.h"
#include "usart.h"
#include "app_fatfs.h"
#include "sd_app.h"
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
extern uint8_t sdcardbuffer[512];
extern uint8_t workingbuffer[110];

extern uint8_t dma_rx_buffer[DMA_CHUNK_SIZE];               // Réception DMA par bloc de 5
extern uint8_t circular_buffer[CIRC_BUF_SIZE];              // Buffer circulaire
extern volatile uint16_t write_index;
extern volatile uint16_t read_index;

extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;

extern int flag_drop;
extern int flag_separation;
extern int flag_calib;

extern int flag_bouton_servo;
extern float hauteur_Initiale;

extern float hauteur_relative;

extern int pbmseeker;

extern int trameready;

extern int flag_servo_started;


extern int received_flag;


int counterrecalib=0;
int sd_counter=0;


int8_t vraiRSSI=0;
float distance_RSSI=0.0;



extern DecodedPayload OTHERData;



int pbmseeker_flag=0;
int blinker_sd_flag=0;

extern uint32_t timeindex;

extern uint32_t cpt_tps_chute;
extern int flag_fin;

UBaseType_t sizestatemachine;
UBaseType_t sizeGNSS;
UBaseType_t sizesdcard;
UBaseType_t sizetarvos;


float timestatemachine=0.0;
float timesdcard=0.0;
float timedecode=0.0;
float timeservo=0.0;
float timedist=0.0;
float timeGNSS=0.0;

extern FATFS FatFs;   // FATFS handle
extern FILINFO fno;	  // Structure holds information
extern FATFS *getFreeFs; 	  // Read information
extern FIL fil;
extern DIR dir;			  // Directory object structure
extern DWORD free_clusters;  // Free Clusters
extern DWORD free_sectors;	  // Free Sectors
extern DWORD total_sectors;

int led_flag=0;
uint8_t lcdbuffer[64];

/* USER CODE END Variables */
osThreadId statemachineHandle;
osThreadId SdcardwriteHandle;
osThreadId tarvosDecodeHandle;
osMutexId SDCard_mutexeHandle;
osMutexId I2CmutexHandle;
osMutexId uartmutexHandle;
osSemaphoreId uartTxDoneHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Startstatemachine(void const * argument);
void StartSdcard(void const * argument);
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
  /* Create the mutex(es) */
  /* definition and creation of SDCard_mutexe */
  osMutexDef(SDCard_mutexe);
  SDCard_mutexeHandle = osMutexCreate(osMutex(SDCard_mutexe));

  /* definition and creation of I2Cmutex */
  osMutexDef(I2Cmutex);
  I2CmutexHandle = osMutexCreate(osMutex(I2Cmutex));

  /* definition and creation of uartmutex */
  osMutexDef(uartmutex);
  uartmutexHandle = osMutexCreate(osMutex(uartmutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of uartTxDone */
  osSemaphoreDef(uartTxDone);
  uartTxDoneHandle = osSemaphoreCreate(osSemaphore(uartTxDone), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  osSemaphoreWait(uartTxDoneHandle, 0);
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
  osThreadDef(statemachine, Startstatemachine, osPriorityAboveNormal, 0, 300);
  statemachineHandle = osThreadCreate(osThread(statemachine), NULL);

  /* definition and creation of Sdcardwrite */
  osThreadDef(Sdcardwrite, StartSdcard, osPriorityNormal, 0, 512);
  SdcardwriteHandle = osThreadCreate(osThread(Sdcardwrite), NULL);

  /* definition and creation of tarvosDecode */
  osThreadDef(tarvosDecode, startTarvosDecode, osPriorityRealtime, 0, 256);
  tarvosDecodeHandle = osThreadCreate(osThread(tarvosDecode), NULL);

  /* USER CODE BEGIN RTOS_THREADS */


  CoreDebug->DEMCR |=CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT =0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

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
	  lcd_clear();
	  if(led_flag==0){
		  LED_Setcolour(255, 0, 0,0, 0, 255);
		  LED_Update();

	  }
	  else{
		  LED_Setcolour(0, 0, 255,255, 0, 0);
		  LED_Update();
	  }
	  led_flag=1-led_flag;
	  if(flag_fin==0){
	  setCursor(0,0);
	  snprintf((char *)lcdbuffer,30,"hauteur=%0.2f",OTHERData.altitude_baro);
	  lcd_send_string((char *)lcdbuffer);
	  osDelay(2);
	  setCursor(1,0);
	  snprintf((char *)lcdbuffer,30,"dist=%0.1fm",distance_RSSI);
	  lcd_send_string((char *)lcdbuffer);
	  }
	  else{
		  setCursor(0,0);
		  	  snprintf((char *)lcdbuffer,30,"FIN");
		  	  lcd_send_string((char *)lcdbuffer);
		  	  setCursor(1,0);
		  	  snprintf((char *)lcdbuffer,30,"FIN");
		  	  lcd_send_string((char *)lcdbuffer);

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

	FRESULT fres=FR_OK;
	fres = f_mount(&FatFs, "", 1);
	TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  uint32_t start1= DWT->CYCCNT;
		  osMutexWait(SDCard_mutexeHandle, portMAX_DELAY);

		  blinker_sd_flag=1-blinker_sd_flag;
		  if(blinker_sd_flag==1){
			  LED_Setcolour(0,0,0,255,255,255);
		  }
		  else{
			  LED_Setcolour(0,0,0,0,0,0);
		  }

		  if(flag_drop==0){

			  if(sd_counter==5){
				  fres=store_in_sd(fres);

			  sd_counter=0;
			  }
			  sd_counter++;

		  }
		  else{
			  fres=store_in_sd(fres);

		  }



		  osMutexRelease(SDCard_mutexeHandle);

			if(flag_fin==1){

				f_mount(NULL, "", 0);
				osThreadSuspend(NULL);
			}

			  uint32_t end1= DWT->CYCCNT;
			  uint32_t cycles= end1-start1;
			  timesdcard=(float) cycles/(SystemCoreClock/1000000.0f);


			  vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
  }
  /* USER CODE END StartSdcard */
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
	TickType_t xLastWakeTime = xTaskGetTickCount();
	 uint8_t temp_trame[TRAME_SIZE];
		      uint8_t temp5[5];
		      uint8_t data_index = 0;
  /* Infinite loop */
  for(;;)
  {
	  uint32_t start1= DWT->CYCCNT;



	          while (read_index != write_index) {
	              // Recherche de l'entête principale
	              if (circular_buffer[read_index] == 0x02 &&
	                  circular_buffer[(read_index + 1) % CIRC_BUF_SIZE] == 0x81)
	              {
	                  data_index = 0;

	                  while (data_index < TRAME_SIZE) {
	                      // Vérifie s’il reste au moins 5 octets
	                      uint16_t available = (write_index >= read_index)
	                          ? (write_index - read_index)
	                          : (CIRC_BUF_SIZE - read_index + write_index);

	                      if (available < 5) {
	                          break; // attendre plus de données
	                      }

	                      // Copie 5 octets
	                      for (int i = 0; i < 5; i++) {
	                          temp5[i] = circular_buffer[(read_index + i) % CIRC_BUF_SIZE];
	                      }

	                      // Confirmation TX ? (trame parasite)
	                      if (temp5[0] == 0x02 && temp5[1] == 0x40 && temp5[2] == 0x01) {
	                          // Skip trame de confirmation
	                          read_index = (read_index + 5) % CIRC_BUF_SIZE;
	                          continue;
	                      }

	                      // Sinon : partie utile, on ajoute à la trame
	                      for (int i = 0; i < 5 && data_index < TRAME_SIZE; i++) {
	                          temp_trame[data_index++] = temp5[i];
	                      }

	                      read_index = (read_index + 5) % CIRC_BUF_SIZE;
	                  }

	                  if (data_index == TRAME_SIZE) {

	                	  if (tarvos_checksum(temp_trame, TRAME_SIZE) == temp_trame[TRAME_SIZE - 1]) {

	                      memcpy(tarvos_DATA, temp_trame, TRAME_SIZE);
	                      decode_payload(&OTHERData,(uint8_t *) tarvos_DATA);

	    if ((vraiRSSI >= 0) || (vraiRSSI < -120) || (NVALUE <= 0.0f)) {
	        distance_RSSI= -1.0f; // Valeur invalide
	    }
	    else{
	    	float exponent= 0.0f;
		    exponent = ((float)RSSI0 - (float)vraiRSSI) / (10.0f * NVALUE);
		    distance_RSSI= powf(10.0f, exponent);
	    }

	                	  }

	                  }
	              } else {
	                  // Entête invalide : skip 1 octet
	                  read_index = (read_index + 1) % CIRC_BUF_SIZE;
	              }
	          }




		if(flag_fin==1){

			osThreadSuspend(NULL);
		}

		  uint32_t end1= DWT->CYCCNT;
		  uint32_t cycles= end1-start1;

		  timedecode=(float) cycles/(SystemCoreClock/1000000.0f);

vTaskDelay(pdMS_TO_TICKS(50));
  }
  /* USER CODE END startTarvosDecode */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

