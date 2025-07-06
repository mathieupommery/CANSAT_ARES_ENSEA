/*
 * tarvos.h
 *
 *  Created on: Jan 19, 2025
 *      Author: louisvoz
 */

#ifndef INC_tarvos_H_
#define INC_tarvos_H_
#include "main.h"



#define CMD_PAYLOAD_MAX 224

#define TOP_ADDR 0x15
#define BOTTOM_ADDR 0x16
#define GROUND_ADDR 0x01
#define SAT_ADDR 0x10
#define CHANNEL 0x82

#define RSSI0 -56
#define NVALUE 2

typedef struct {
	uint8_t workingbuffer[64];
    uint16_t header_code;
    uint8_t flag_calib;
    uint8_t flag_drop;
    uint8_t flag_separation;
    uint8_t flag_fin;
    float latitude;
    float longitude;
    float hMSL;
    float altitude_baro;
    float vspeed;
    float hspeed;
    float temperature;
    float Accx;
    float Accy;
    float Accz;
    float pression;
    uint32_t timeindex;
    uint8_t RSSI;
    uint8_t senderadress;
} DecodedPayload;



HAL_StatusTypeDef SET_tcMODE(char mode);
HAL_StatusTypeDef INIT_PERM_TARVOS(uint8_t Hadr, uint8_t Sadr);
HAL_StatusTypeDef INIT_TARVOS(UART_HandleTypeDef hlpuart1);
uint8_t Get_CRC8(uint8_t * bufP, uint16_t len);

int SEND_DATA_NETW(uint8_t *data, uint8_t channel, uint8_t dest_adress, int length);

void SEND_DATA_NETW1(uint8_t *data, uint8_t channel, uint8_t dest_adress, int length);

void create_and_send_payload(uint8_t* buffer,uint8_t channel,uint8_t dest_adress,uint16_t header_code,float latitude,float longitude,float hMSL,float altitude_baro,float vspeed,float hspeed,float temperature,float pression, float Accx, float Accy, float Accz, uint32_t timeindex);

void decode_payload(DecodedPayload* out,uint8_t * receivingbuffer);

HAL_StatusTypeDef SET_Destadr(uint8_t Destadrr);
HAL_StatusTypeDef REQ(uint8_t index);
HAL_StatusTypeDef REQ_RSSI();

uint8_t tarvos_checksum(uint8_t *data, uint16_t len);

#include <stdint.h>
#include <string.h>
#include <stdio.h>








#endif /* INC_tarvos_H_ */





