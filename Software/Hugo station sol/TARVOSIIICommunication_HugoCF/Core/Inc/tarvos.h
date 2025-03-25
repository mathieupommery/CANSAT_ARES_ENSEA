/* --- Includes --- */
#include "main.h"

/* --- Prototypes --- */
void INIT_PERM_TARVOS(uint8_t Hadr, uint8_t Sadr);
void SET_tcMODE(char mode);
uint8_t Get_CRC8(uint8_t * bufP, uint16_t len);
int SEND_DATA_NETW(uint8_t *data, uint8_t channel, uint8_t dest_adress, int length);
uint8_t* copyTarvosBuffer();
void SET_Destadr(uint8_t Destadrr);
void REQ(uint8_t index);
void REQ_RSSI();
