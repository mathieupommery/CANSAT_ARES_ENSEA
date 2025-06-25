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
extern uint8_t sdcardbuffer[256];
extern uint8_t workingbuffer[110];


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

extern float hauteur_servo;

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

/* USER CODE END Variables */
osThreadId statemachineHandle;
osThreadId GNSSParseHandle;
osThreadId SdcardwriteHandle;
osThreadId servoHandle;
osThreadId distancecalcHandle;
osThreadId tarvosDecodeHandle;
osMutexId SDCard_mutexeHandle;

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
  osThreadDef(statemachine, Startstatemachine, osPriorityNormal, 0, 300);
  statemachineHandle = osThreadCreate(osThread(statemachine), NULL);

  /* definition and creation of GNSSParse */
  osThreadDef(GNSSParse, StartGNSSParse, osPriorityAboveNormal, 0, 256);
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
  osThreadDef(tarvosDecode, startTarvosDecode, osPriorityHigh, 0, 256);
  tarvosDecodeHandle = osThreadCreate(osThread(tarvosDecode), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
#ifdef PARTIE_HAUT

  osThreadSuspend(distancecalcHandle);


#endif
#ifdef PARTIE_BAS

  osThreadSuspend(servoHandle);

#endif
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
	  }
	  ssd1306_UpdateScreen();



	  sizestatemachine=uxTaskGetStackHighWaterMark(statemachineHandle);
	  sizeGNSS=uxTaskGetStackHighWaterMark(GNSSParseHandle);
	  sizesdcard=uxTaskGetStackHighWaterMark(SdcardwriteHandle);
	  sizetarvos=uxTaskGetStackHighWaterMark(tarvosDecodeHandle);










    osDelay(100);
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
  /* Infinite loop */
  for(;;)
  {
	  GNSS_ParsePVTData(&GNSSData);
	  bmp581_read_precise_normal(&myDatabmp581);
	  Read_sensor_data(&myData6AXIS);

	  if(counterrecalib>=20){
		  if(GNSSData.fixType>=3){
			  float altitude = GNSSData.fhMSL;
			  float base = 1.0f - (altitude / 44330.0f);

			  if (base > 0.01f)  // Évite le 0 ou négatif
			  {
			      float denominator = powf(base, 5.255f);
			      if (denominator > 0.001f)
			          P0 = (float) myDatabmp581.press / denominator;
			      else
			          P0 = 101325.0;  // ou une valeur par défaut
			  }
			  else
			  {
			      P0 = 101325.0;
			  }

		  }
		  counterrecalib=0;
	  }

	  if(flag_calib && (GNSSData.fixType>=3)){

		  hauteur_servo=(float)(GNSSData.fhMSL-hauteur_Initiale);

	  }
	  counterrecalib++;


    osDelay(100);
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

    osDelay(100);
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
  /* Infinite loop */
  for(;;)
  {

	  if(flag_servo_started==1){
		  stop_servo();
		  flag_servo_started=0;
	  }


	  if((flag_drop==1) && (flag_calib==1)){
		  if((hauteur_servo<=60.0)){
			  release_mecanism();
			  flag_separation=1;
			  flag_servo_started=1;
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

    osDelay(200);
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
  /* Infinite loop */
  for(;;)
  {

	  if(flag_calib==1){

		  if(GNSSData.fixType>=3){
#ifdef PARTIE_BAS
			  distance_entre_module=distancecalc(GNSSData.fLat,TOPData.latitude, GNSSData.fLon,TOPData.longitude,GNSSData.fhMSL,TOPData.hMSL);

#endif
	  }
}
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
#ifdef PARTIE_BAS
		  case TOP_ADDR:
			  decode_payload(&TOPData,(uint8_t *) tarvos_DATA);
			  break;
#endif
		  default:
			  decode_payload(&OTHERData,(uint8_t *) tarvos_DATA);

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

