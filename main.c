#include "msp430.h"
#include "system.h"
#include "lcd.h"
#include "dac.h"
#include "adc.h"
#include "encoder.h"


volatile int displayVset = 0;
volatile int displayIlim = 0;
volatile int displayOverCurrent = 0;

int main(void)
{
	unsigned long main_loop_counter = 0, display_delay_counter = 0;
	Vout_Data_t vout, vout_old;
	
	// Stop watchdog timer to prevent time out reset
	WDTCTL = WDTPW + WDTHOLD;
  
	System_Init();
	Set_Status_LED(LED_Yellow);
	Set_Status_LED(LED_Red);
	DAC_Init();
	ADC_Init();
	LCD_Init();
	Encoders_Init();
	Reset_Status_LED(LED_Yellow);
	Reset_Status_LED(LED_Red);
	
	LCD_go_line1();
	LCD_send_string("USB PSU");
	LCD_go_line2();
	LCD_send_char('A');
	LCD_send_char('p');
	LCD_send_char(0xb4);
	LCD_send_char(0xb8);
	LCD_send_char('p');
	LCD_send_char('o');
	LCD_send_char(0xb3);
	
	vout = Get_Vout();
	vout.DAC_value = DAC_VOUT_MAX_VALUE / 2;
	Set_Vout(vout);
	Increment_Vout();
	
	for(;;)
	{
		if (displayVset == 1 || main_loop_counter == 100000)
		{
			Disable_RE_Interrupt();
			Set_Status_LED(LED_Red);
			displayVset = 0;
			main_loop_counter = 0;
		
			vout = Get_Vout();
			ADC12CTL0 |= (ADC12SC | ADC12ENC);
			while ((ADC12CTL1 & ADC12BUSY) == ADC12BUSY);
			vout.ADC_value = ADC12MEM0;
			Set_Vout(vout);
			
			LCD_go_line1();
			LCD_send_string("Vs:");
			LCD_send_double(Get_Vout().Vset);
			LCD_go_line2();
			LCD_send_string("Vm:");
			LCD_send_double(Get_Vout().Vmeasured);
			
			Reset_Status_LED(LED_Red);
			Enable_RE_Interrupt();
		}
		
		if (displayIlim == 1)
		{
			Disable_RE_Interrupt();
			displayIlim = 0;
			LCD_go_line1();
			LCD_send_string("Imax:   ");
			LCD_go_line2();
			LCD_send_double(Get_Ilim().Vset);
			LCD_send_string("A  ");
			Enable_RE_Interrupt();
			for (display_delay_counter = 0; display_delay_counter < 100000; display_delay_counter++);
		}
		
		if (displayOverCurrent == 1)
		{
			__disable_interrupt();
			Set_Status_LED(LED_Red);
			vout = Get_Vout();
			vout_old = vout;
			vout.DAC_value = 0;
			Set_Vout(vout);
			displayOverCurrent = 0;
			LCD_go_line1();
			LCD_send_string("Over    ");
			LCD_go_line2();
			LCD_send_string("Current ");
			for (display_delay_counter = 0; display_delay_counter < 100000; display_delay_counter++);
			Set_Vout(vout_old);
			Reset_Status_LED(LED_Red);
			__enable_interrupt();
		}
		
		main_loop_counter++;
	}
}


#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR()
{
	unsigned int p1iv = P1IV;
	switch(p1iv)
	{
	case 0x02:
		CLimit_handler();
		break;
	case 0x04:
		RE1_A_handler();
		break;
	case 0x06:
		RE1_B_handler();
		break;
	case 0x08:
		RE1_S1_handler();
		break;
	case 0x0a:
		RE2_A_handler();
		break;
	case 0x0c:
		RE2_B_handler();
		break;
	case 0x0e:
		RE2_S1_handler();
		break;
	}
}