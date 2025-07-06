/*
 * lcd1602.h
 *
 *  Created on: Jun 14, 2025
 *      Author: mathi
 */

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_

#include "main.h"









void lcd_send_cmd( char data);
void lcd_Init(void);
void lcd_send_string (char *str);
void setCursor(int row, int col);
void lcd_clear(void);









#endif /* INC_LCD1602_H_ */
