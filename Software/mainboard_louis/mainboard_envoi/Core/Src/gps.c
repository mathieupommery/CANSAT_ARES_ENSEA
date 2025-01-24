/*
 * gps.c
 *
 *  Created on: Jan 24, 2025
 *      Author: louisvoz
 */

extern int GPGGA[75];
int loc[26];

int Get_Loc(void){
	uint8_t loc[26];
	for (int i  =0 ; i<75 ; i++){
		if ((GPGGA[i]==78) || (GPGGA[i]==83)){
			for (int j = 0 ; j<26 ; j++){
					loc[j]=GPGGA[j+i-11];
			}
		}

	}
	return loc ;
}

void Get_Time(void){
	uint8_t time[9];
	time[0]=GPGGA[0+7]-48;
	time[1]=GPGGA[1+7]-48+2;
	time[2]='h';
	time[3]=GPGGA[2+7]-48;
	time[4]=GPGGA[3+7]-48;
	time[5]='m';
	if (time[1]>9){
			time[0]=2;
			time[1]=time[1]%10;
		}
	if ((time[0]*10+time[1])>24){
		time[0]=0;
		time[1]=(time[0]*10+time[1])%24;
	}
	return time;
	}




void INIT_GPS(void){
	HAL_StatusTypeDef status;
	uint8_t test;
	//10Hz
	uint8_t buffer2[]={0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7A, 0x12, 0xB5, 0x62, 0x06, 0x08, 0x00, 0x00, 0x0E, 0x30 };
	status=HAL_UART_Transmit_IT(&huart2, buffer2, 22);
	HAL_Delay(200);
}
