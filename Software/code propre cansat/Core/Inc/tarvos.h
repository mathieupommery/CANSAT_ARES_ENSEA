/*
 * tarvos.h
 *
 *  Created on: Jan 19, 2025
 *      Author: louisvoz
 */


#include "main.h"



#define CMD_PAYLOAD_MAX 224

#define TOP_ADDR 0x15
#define BOTTOM_ADDR 0x16
#define GROUND_ADDR 0x01
#define SAT_ADDR 0x10


HAL_StatusTypeDef SET_tcMODE(char mode);
HAL_StatusTypeDef INIT_PERM_TARVOS(uint8_t Hadr, uint8_t Sadr);
HAL_StatusTypeDef INIT_TARVOS(UART_HandleTypeDef hlpuart1);
uint8_t Get_CRC8(uint8_t * bufP, uint16_t len);

int SEND_DATA_NETW(uint8_t *data, uint8_t channel, uint8_t dest_adress, int length);

void SEND_DATA_NETW1(uint8_t *data, uint8_t channel, uint8_t dest_adress, int length);



HAL_StatusTypeDef SET_Destadr(uint8_t Destadrr);
HAL_StatusTypeDef REQ(uint8_t index);
HAL_StatusTypeDef REQ_RSSI();













