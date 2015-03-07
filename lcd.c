#include "lcd.h"
#include <stdio.h>

void LCD_Init()
{
	Delay_500ms();
	Delay_500ms();
	
	// config pins as outputs
	Set_LCD_Data(0);
	Set_LCD_Command(0);
	P3SEL = 0;
	P3DIR = 0xff;
	
	// Function set - Sets to 4-bit operation
	Set_LCD_Data(LCD_DB5);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Delay_5ms();
	
	// Function set - Sets 4-bit operation and selects 1-line display and 5 x 8 dot character font
	Set_LCD_Data(LCD_DB5);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Set_LCD_Data(LCD_DB6 | LCD_DB7);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Delay_100ms();
	
	// Display on/off control - Turns on display and cursor
	Set_LCD_Data(0);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Set_LCD_Data(LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Delay_100ms();
	
	// Entry mode set - Sets mode to increment the address by one and to shift the cursor to the right at the time of write to the DD/CGRAM. Display is not shifted.
	Set_LCD_Data(0);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Set_LCD_Data(LCD_DB5 | LCD_DB6);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Delay_100ms();
}

void LCD_send_char(char c)
{
	Set_LCD_Data(c >> 4);
	Set_LCD_Command(LCD_RS);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Set_LCD_Data(c & 0x0f);
	Set_LCD_Command(LCD_RS);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
}

void LCD_send_string(char *c)
{
	while(*c)
	{
		LCD_send_char(*c++);
	}
}

void LCD_send_int(unsigned int num)
{
	char str[5];
	sprintf(str, "%d", num);
	LCD_send_string(str);
}

void LCD_send_double(double num)
{
	char str[6];
	sprintf(str, "%4.3f", num);
	LCD_send_string(str);
}

void LCD_go_line1()
{
	Set_LCD_Data(LCD_DB7);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
}

void LCD_go_line2()
{
	Set_LCD_Data(LCD_DB6 | LCD_DB7);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
}

void LCD_go_home()
{
	Set_LCD_Data(0);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	
	Set_LCD_Data(LCD_DB5);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Delay_5ms();
}

void LCD_soft_clear()
{
	LCD_go_line1();
	for (int i = 0; i < 8; i++)
	{
		LCD_send_char(' ');
	}
	
	LCD_go_line2();
	for (int i = 0; i < 8; i++)
	{
		LCD_send_char(' ');
	}
	
	LCD_go_line1();
}

void LCD_clear()
{
	Set_LCD_Data(0);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	
	Set_LCD_Data(LCD_DB4);
	Delay_E();
	Set_LCD_E();
	Delay_E();
	Reset_LCD_E();
	Set_LCD_Data(0);
	Delay_5ms();
}

void Delay_E()
{
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
}

void Delay_5ms()
{
	for (int i = 0; i < 50; i++)
	{
		Delay_E();
	}
}

void Delay_100ms()
{
	for (int i = 0; i < 20; i++)
	{
		Delay_5ms();
	}
}

void Delay_500ms()
{
	Delay_100ms();
	Delay_100ms();
	Delay_100ms();
	Delay_100ms();
	Delay_100ms();
}