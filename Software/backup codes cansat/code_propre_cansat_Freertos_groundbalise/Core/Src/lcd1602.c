/*
 * lcd1602.c
 *
 *  Created on: Jun 14, 2025
 *      Author: mathi
 */


#include "main.h"
#include "lcd1602.h"

#define SLAVE_ADDRESS_LCD 0x4E

char str[40];

extern I2C_HandleTypeDef hi2c1;

void lcd_send_data( char data){

	char data_u, data_l;
	uint8_t data_t[4];
	data_u=(data&0xf0);
	data_l=((data<<4)&0xf0);
	data_t[0]=data_u|0x0D;
	data_t[1]=data_u|0x09;
	data_t[2]=data_l|0x0D;
	data_t[3]=data_l|0x09;
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *) data_t,4, 100);





}

void lcd_send_cmd( char data){

	char data_u, data_l;
	uint8_t data_t[4];
	data_u=(data&0xf0);
	data_l=((data<<4)&0xf0);
	data_t[0]=data_u|0x0C;
	data_t[1]=data_u|0x08;
	data_t[2]=data_l|0x0C;
	data_t[3]=data_l|0x08;
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *) data_t,4, 100);





}

void lcd_Init(void){

	HAL_Delay(50);
	lcd_send_cmd(0x30);
	HAL_Delay(5);
	lcd_send_cmd(0x30);
	HAL_Delay(1);
	lcd_send_cmd(0x30);
	HAL_Delay(10);
	lcd_send_cmd(0x20);

	HAL_Delay(10);
	lcd_send_cmd(0x28);

	HAL_Delay(1);
	lcd_send_cmd(0x08);
	HAL_Delay(1);
	lcd_send_cmd(0x01);
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd(0x06);
	HAL_Delay(1);
	lcd_send_cmd(0x0C);


}

void lcd_send_string (char *str){


	while (*str)lcd_send_data(*str++);
}

void setCursor(int x, int y){


	switch(y){
	case 0:
		y|=0x80;
		break;
	case 1:
		y|=0xC0;
		break;

	}
	lcd_send_cmd(y);
	lcd_send_cmd(x);

}
