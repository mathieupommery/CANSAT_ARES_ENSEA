/*
 * sd_app.c
 *
 *  Created on: Jun 20, 2025
 *      Author: mathi
 */
#include "sd_app.h"
#include "main.h"
#include "tarvos.h"
#include "led.h"
#include "usart.h"
#include "app_fatfs.h"


extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;

extern float hauteur_Initiale;

extern float temp;
extern float vrefint;
extern float vbat;

extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];
extern uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];
extern uint8_t sdcardbuffer[512];

extern int flag_calib;
extern int flag_drop;
extern int flag_separation;

#ifdef PARTIE_SAT
extern float distance_RSSI;
extern int vraiRSSI;
#endif

extern uint32_t timeindex;

extern DecodedPayload OTHERData;

#ifdef PARTIE_GROUND
extern DecodedPayload TOPData;
extern DecodedPayload BOTTOMData;
extern DecodedPayload SATData;
#endif


extern FATFS FatFs;   // FATFS handle
extern FILINFO fno;	  // Structure holds information
extern FATFS *getFreeFs; 	  // Read information
extern FIL fil;
extern DIR dir;			  // Directory object structure
extern DWORD free_clusters;  // Free Clusters
extern DWORD free_sectors;	  // Free Sectors
extern DWORD total_sectors;



FRESULT store_in_sd(void){

	uint16_t sizeligne=0;
	FRESULT fres=FR_OK;

	sizeligne=snprintf((char *)sdcardbuffer,512,
			"%d,%d,%d,%d,%0.7f,%0.7f,%0.7f,%0.7f,%0.1f,%0.1f,%0.0f,%0.0f,%0.0f,%0.0f,%0.0f,%0.0f,%0.1f,%0.1f,%0.1f,%0.1f,%lu\n\r",
		  BOTTOMData.flag_calib,BOTTOMData.flag_drop,BOTTOMData.flag_separation,BOTTOMData.flag_fin,
		  TOPData.latitude,BOTTOMData.latitude,TOPData.longitude,BOTTOMData.longitude,TOPData.hMSL,
		  BOTTOMData.hMSL,TOPData.Accx,BOTTOMData.Accx,TOPData.Accy,BOTTOMData.Accy,TOPData.Accz,BOTTOMData.Accz,SATData.temperature,//satdata.temp donnne la hauteur rssi
		  TOPData.pression,TOPData.temperature,BOTTOMData.temperature,TOPData.timeindex);



	 fres = f_mount(&FatFs, "", 1);
			  if (fres == FR_OK) {
				  if(flag_drop==0){
					  fres = f_open(&fil, (uint8_t *) "GR_01.csv",FA_WRITE | FA_OPEN_ALWAYS);

				  }
				  else{

					  fres = f_open(&fil, (uint8_t *) "GR_02.csv",FA_WRITE | FA_OPEN_ALWAYS);

				  }
				  if (fres == FR_OK) {
					  fres=f_lseek(&fil, f_size(&fil));
					  UINT bytesWrote;
					  fres = f_write(&fil,(char*)sdcardbuffer, sizeligne, &bytesWrote);
					  f_close(&fil);
				  }
				  f_mount(NULL, "", 0);
			  }


	return fres;

}
