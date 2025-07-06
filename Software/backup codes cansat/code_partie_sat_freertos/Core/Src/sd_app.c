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
extern float hauteur_relative;

extern float temp;
extern float vrefint;
extern float vbat;

extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];
extern uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];
extern uint8_t sdcardbuffer[512];

extern int flag_calib;
extern int flag_drop;
extern int flag_separation;

extern int8_t vraiRSSI;
extern float distance_RSSI;


extern uint32_t timeindex;



extern FATFS FatFs;   // FATFS handle
extern FILINFO fno;	  // Structure holds information
extern FATFS *getFreeFs; 	  // Read information
extern FIL fil;
extern DIR dir;			  // Directory object structure
extern DWORD free_clusters;  // Free Clusters
extern DWORD free_sectors;	  // Free Sectors
extern DWORD total_sectors;



FRESULT store_in_sd(FRESULT fres){

	uint16_t sizeligne=0;

	sizeligne=snprintf((char *)sdcardbuffer,256,"%0.1f,%0.1f,%d,%0.2f,%lu\n\r",temp,vbat,vraiRSSI,distance_RSSI,timeindex);




			  if (fres == FR_OK) {
				  if(flag_drop==0){
			  fres = f_open(&fil, (uint8_t *) "SAT_01.csv",FA_WRITE | FA_OPEN_ALWAYS);

				  }
				  else{
					  fres = f_open(&fil, (uint8_t *) "SAT_02.csv",FA_WRITE | FA_OPEN_ALWAYS);

				  }
				  if (fres == FR_OK) {
					  fres=f_lseek(&fil, f_size(&fil));
					  UINT bytesWrote;
					  fres = f_write(&fil,(char*)sdcardbuffer, sizeligne, &bytesWrote);
					  f_close(&fil);
				  }

			  }


	return fres;

}
