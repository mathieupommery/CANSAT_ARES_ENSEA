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

extern float temp;
extern float vrefint;
extern float vbat;

extern uint8_t tarvos_TX_Buffer[TarvosTxBufferSize];
extern uint8_t tarvos_RX_Buffer[TarvosRxBufferSize];
extern uint8_t sdcardbuffer[256];

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



FRESULT store_in_sd(uint8_t * filename){

	uint16_t sizeligne=0;
	FRESULT fres=FR_OK;

#ifdef PARTIE_HAUT

	sizeligne=snprintf((char *)sdcardbuffer,256,
			"%d,%d,%d,%0.7f,%0.7f,%0.2f,%0.2f,%0.1f,%0.1f,%0.1f,%0.0f,%0.0f,%0.0f,%lu\n\r",
			flag_calib,flag_drop,flag_separation,GNSSData.fLat,GNSSData.fLon,GNSSData.fhMSL,
			myDatabmp581.altitude,GNSSData.fgSpeed,GNSSData.fvspeed,myDatabmp581.press,
			myData6AXIS.AccelX,myData6AXIS.AccelY,myData6AXIS.AccelZ,timeindex);

#endif
#ifdef PARTIE_BAS

	sizeligne=snprintf((char *)sdcardbuffer,256,
				"%d,%d,%d,%0.7f,%0.7f,%0.2f,%0.2f,%0.1f,%0.1f,%0.1f,%0.0f,%0.0f,%0.0f,%0.2f,%lu\n\r",
				flag_calib,flag_drop,flag_separation,GNSSData.fLat,GNSSData.fLon,GNSSData.fhMSL,
				myDatabmp581.altitude,GNSSData.fgSpeed,GNSSData.fvspeed,myDatabmp581.press,
				myData6AXIS.AccelX,myData6AXIS.AccelY,myData6AXIS.AccelZ,distance_entre_module,timeindex);
#endif

	 fres = f_mount(&FatFs, "", 1);
			  if (fres == FR_OK) {
				  fres = f_open(&fil, (char *) filename,FA_WRITE | FA_OPEN_ALWAYS);
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
