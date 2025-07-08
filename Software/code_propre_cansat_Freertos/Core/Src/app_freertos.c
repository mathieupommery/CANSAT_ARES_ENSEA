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
#include "bmp581.h"
#include "6axis.h"
#include "tarvos.h"
#include "led.h"
#include "statemachine.h"
#include "servo.h"
#include "ssd1306.h"
#include "math.h"
#include "usart.h"
#include "GNSS.h"
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


extern AXIS6 myData6AXIS;
extern BMP_t myDatabmp581;
extern GNSS_StateHandle GNSSData;

extern double P0;


extern GNSS_StateHandle GNSSData;

extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;

extern STATE state;

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

#ifdef PARTIE_BAS
extern DecodedPayload TOPData;
float distance_entre_module=0.0;
#endif


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

/* USER CODE END Variables */
osThreadId statemachineHandle;
osThreadId GNSSParseHandle;
osThreadId SdcardwriteHandle;
osThreadId servoHandle;
osThreadId distancecalcHandle;
osThreadId tarvosDecodeHandle;
osMutexId SDCard_mutexeHandle;
osMutexId I2CmutexHandle;
osMutexId uartmutexHandle;
osSemaphoreId uartTxDoneHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Startstatemachine(void const * argument);
void StartGNSSParse(void const * argument);
void StartSdcard(void const * argument);
void Startservo(void const * argument);
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

  /* definition and creation of GNSSParse */
  osThreadDef(GNSSParse, StartGNSSParse, osPriorityNormal, 0, 256);
  GNSSParseHandle = osThreadCreate(osThread(GNSSParse), NULL);

  /* definition and creation of Sdcardwrite */
  osThreadDef(Sdcardwrite, StartSdcard, osPriorityNormal, 0, 512);
  SdcardwriteHandle = osThreadCreate(osThread(Sdcardwrite), NULL);

  /* definition and creation of servo */
  osThreadDef(servo, Startservo, osPriorityAboveNormal, 0, 256);
  servoHandle = osThreadCreate(osThread(servo), NULL);

  /* definition and creation of distancecalc */
  osThreadDef(distancecalc, Startdistancecalc, osPriorityBelowNormal, 0, 256);
  distancecalcHandle = osThreadCreate(osThread(distancecalc), NULL);

  /* definition and creation of tarvosDecode */
  osThreadDef(tarvosDecode, startTarvosDecode, osPriorityRealtime, 0, 256);
  tarvosDecodeHandle = osThreadCreate(osThread(tarvosDecode), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
#ifdef PARTIE_HAUT

  osThreadSuspend(distancecalcHandle);


#endif
#ifdef PARTIE_BAS

  osThreadSuspend(servoHandle);

#endif


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

	  uint32_t start1= DWT->CYCCNT;
      if (osMutexWait(I2CmutexHandle, 20) == osOK)
      {


	  if(pbmseeker_flag==0){
		  if(pbmseeker==0){
			  ssd1306_SetCursor(32, 40);
			  ssd1306_Fill(Black);
			  ssd1306_WriteString("OK!", Font_16x24, White);
			  ssd1306_UpdateScreen();


		  }
		  if(pbmseeker==1){
			  ssd1306_SetCursor(32, 40);
			  ssd1306_Fill(Black);
			  ssd1306_WriteString("PB!", Font_16x24, White);
			  ssd1306_UpdateScreen();


		  }
		  vTaskDelay(pdMS_TO_TICKS(500));
		  pbmseeker_flag=1;
	  }
	  else{
	  statemachine();
	  ssd1306_UpdateScreen();
	  }

      osMutexRelease(I2CmutexHandle);
  }


	  sizestatemachine=uxTaskGetStackHighWaterMark(statemachineHandle);
	  sizeGNSS=uxTaskGetStackHighWaterMark(GNSSParseHandle);
	  sizesdcard=uxTaskGetStackHighWaterMark(SdcardwriteHandle);
	  sizetarvos=uxTaskGetStackHighWaterMark(tarvosDecodeHandle);


	  uint32_t end1= DWT->CYCCNT;
	  uint32_t cycles= end1-start1;

	  timestatemachine=(float) cycles/(SystemCoreClock/1000000.0f);











	  vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
  }
  /* USER CODE END Startstatemachine */
}

/* USER CODE BEGIN Header_StartGNSSParse */
/**
* @brief Function implementing the GNSSParse thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGNSSParse */
void StartGNSSParse(void const * argument)
{
  /* USER CODE BEGIN StartGNSSParse */
	TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  uint32_t start1= DWT->CYCCNT;
	  GNSS_ParsePVTData(&GNSSData);
	  bmp581_read_precise_normal(&myDatabmp581);
      if (osMutexWait(I2CmutexHandle, 20) == osOK)
      {
    	  Read_sensor_data(&myData6AXIS);
          osMutexRelease(I2CmutexHandle);
      }


	  if(flag_calib){
		  hauteur_relative=(float)(myDatabmp581.altitude-hauteur_Initiale);
	  }

		if(flag_fin==1){

			osThreadSuspend(NULL);
		}

		  uint32_t end1= DWT->CYCCNT;
		  uint32_t cycles= end1-start1;

		  float times=(float) cycles/(SystemCoreClock/1000000.0f);
if(times>timeGNSS){
	timeGNSS=times;
}

vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
  }
  /* USER CODE END StartGNSSParse */
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

/* USER CODE BEGIN Header_Startservo */
/**
* @brief Function implementing the servo thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Startservo */
void Startservo(void const * argument)
{
  /* USER CODE BEGIN Startservo */
	TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  uint32_t start1= DWT->CYCCNT;

	  if((flag_drop==1) && (flag_calib==1)){

		  if((hauteur_relative<=HAUTEUR_SEPARATION)){
			  release_mecanism();
			  flag_separation=1;
			  flag_servo_started=1;
			  vTaskDelay(pdMS_TO_TICKS(500));
			  osThreadSuspend(NULL);



		  }
	  }
	  if(flag_drop==0){
		  if(flag_bouton_servo==1){
			  release_mecanism();
			  flag_bouton_servo=2;
			  flag_servo_started=1;
		  }
		  if(flag_bouton_servo==0){
			  lock_mecanism();
			  flag_bouton_servo=2;
			  flag_servo_started=1;
		  }

	  }

	  	  if(flag_servo_started==1){
	  		 vTaskDelay(pdMS_TO_TICKS(500));

	  		  stop_servo();
	  		  flag_servo_started=0;
	  	  }



		if(flag_fin==1){

			osThreadSuspend(NULL);
		}

		  uint32_t end1= DWT->CYCCNT;
		  uint32_t cycles= end1-start1;
		  float times=(float) cycles/(SystemCoreClock/1000000.0f);
if(times>timeservo){
	timeservo=times;
}
vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
  }
  /* USER CODE END Startservo */
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
	  uint32_t start1= DWT->CYCCNT;

	  if(flag_calib==1){

		  if(GNSSData.fixType>=3){
#ifdef PARTIE_BAS
			  distance_entre_module=distancecalc(GNSSData.fLat,TOPData.latitude, GNSSData.fLon,TOPData.longitude,hauteur_relative,TOPData.altitude_baro);

#endif
	  }
}

		if(flag_fin==1){

			osThreadSuspend(NULL);
		}



	  uint32_t end1= DWT->CYCCNT;
	  uint32_t cycles= end1-start1;

	  float times=(float) cycles/(SystemCoreClock/1000000.0f);
if(times>timedist){
timedist=times;
}
vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
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

#ifdef PARTIE_HAUT
	                      memcpy(tarvos_DATA, temp_trame, TRAME_SIZE);
	                      decode_payload(&OTHERData, tarvos_DATA);  // ou un seul struct global


#endif

#ifdef PARTIE_BAS
	                      memcpy(tarvos_DATA, temp_trame, TRAME_SIZE);
	                      decode_payload(&TOPData, tarvos_DATA);  // ou un seul struct global


#endif

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
//
//	  if(trameready==1){
//
//		  switch(tarvos_DATA[3]){
//#ifdef PARTIE_BAS
//		  case TOP_ADDR:
//			  decode_payload(&TOPData,(uint8_t *) tarvos_DATA);
//			  break;
//#endif
//		  default:
//			  decode_payload(&OTHERData,(uint8_t *) tarvos_DATA);
//
//			  break;
//		  }
//		  trameready=0;
//	  }
/* USER CODE END Application */

