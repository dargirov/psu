#ifndef LCD_H
#define LCD_H

#include "msp430.h"

#define LCD_DB4 0x01
#define LCD_DB5 0x02
#define LCD_DB6 0x04
#define LCD_DB7 0x08
#define LCD_E	0x40
#define LCD_RS	0x10
#define LCD_Data P3OUT
#define LCD_Command LCD_Data

#define Set_LCD_Data(x) (LCD_Data = (LCD_Data & 0x80) | (x))
#define Set_LCD_Command(x) (LCD_Command = (x & 0xf0) | (LCD_Command & 0x8f))
#define Set_LCD_E() (LCD_Command |= 0x40)
#define Reset_LCD_E() (LCD_Command &= 0xbf)

void Delay_E();
void Delay_5ms();
void Delay_100ms();
void Delay_500ms();

void LCD_Init();
void LCD_send_char(char);
void LCD_send_string(char *);
void LCD_send_int(unsigned int);
void LCD_send_double(double num);
void LCD_go_line1();
void LCD_go_line2();
void LCD_go_home();
void LCD_soft_clear();
void LCD_clear();

#endif