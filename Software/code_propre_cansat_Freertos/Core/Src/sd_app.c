/*
 * sd_app.c
 *
 *  Created on: Jun 20, 2025
 *      Author: mathi
 */
#include "sd_app.h"
#include "statemachine.h"
#include "main.h"
#include "bmp581.h"
#include "6axis.h"
#include "tarvos.h"
#include "led.h"
#include "usart.h"
#include "ssd1306.h"
#include "GNSS.h"
#include "app_fatfs.h"

extern AXIS6 myData6AXIS;
extern BMP_t myDatabmp581;
extern GNSS_StateHandle GNSSData;

extern uint8_t LEDDMABUF[DMABUFLEN];
extern uint8_t DMA_COMPLETE_FLAG;

extern STATE state;

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

extern GNSS_StateHandle GNSSData;
#ifdef PARTIE_BAS
extern float distance_entre_module;
#endif

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

#ifdef PARTIE_HAUT

	sizeligne=snprintf((char *)sdcardbuffer,512,
			"%d,%d,%d,%0.7f,%0.7f,%0.2f,%0.2f,%0.1f,%0.1f,%0.1f,%0.0f,%0.0f,%0.0f,%d,%0.2f,%0.2f,%lu\n\r",
			flag_calib,flag_drop,flag_separation,GNSSData.fLat,GNSSData.fLon,GNSSData.fhMSL,
			hauteur_relative,GNSSData.fgSpeed,GNSSData.fvspeed,myDatabmp581.press,
			myData6AXIS.AccelX,myData6AXIS.AccelY,myData6AXIS.AccelZ,GNSSData.numSV,temp,vbat,timeindex);

#endif
#ifdef PARTIE_BAS

	sizeligne=snprintf((char *)sdcardbuffer,512,
				"%d,%d,%d,%0.7f,%0.7f,%0.2f,%0.2f,%0.1f,%0.1f,%0.1f,%0.0f,%0.0f,%0.0f,%0.2f,%d,%0.2f,%0.2f,%lu\n\r",
				flag_calib,flag_drop,flag_separation,GNSSData.fLat,GNSSData.fLon,GNSSData.fhMSL,
				hauteur_relative,GNSSData.fgSpeed,GNSSData.fvspeed,myDatabmp581.press,
				myData6AXIS.AccelX,myData6AXIS.AccelY,myData6AXIS.AccelZ,distance_entre_module,GNSSData.numSV,temp,vbat,timeindex);
#endif



			  if (fres == FR_OK) {
				  if(flag_drop==0){
		#ifdef PARTIE_HAUT
					  fres = f_open(&fil, (uint8_t *) "TOP_01.csv",FA_WRITE | FA_OPEN_ALWAYS);
		#endif
		#ifdef PARTIE_BAS
					  fres = f_open(&fil, (uint8_t *) "BOT_01.csv",FA_WRITE | FA_OPEN_ALWAYS);
		#endif
				  }
				  else{
		#ifdef PARTIE_HAUT
					  fres = f_open(&fil, (uint8_t *) "TOP_02.csv",FA_WRITE | FA_OPEN_ALWAYS);
		#endif
		#ifdef PARTIE_BAS
					  fres = f_open(&fil, (uint8_t *) "BOT_02.csv",FA_WRITE | FA_OPEN_ALWAYS);
		#endif

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
