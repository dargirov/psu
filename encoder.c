#include "encoder.h"
#include "system.h"
#include "lcd.h"

static RE_Data RE1, RE2;
extern volatile int displayVset;
extern volatile int displayIlim;
extern volatile int displayOverCurrent;

void Encoders_Init()
{
	// config P1 interrpts
	P1DIR &= 0xff80;	// input
	P1OUT |= 0x007f;	// pulled up
	P1REN |= 0x007f;	// enable resistors
	P1SEL &= 0xff80;	// pins are set as IO (requred for interrupts)
	P1IES |= 0x007f;	// high-to-low transition
	P1IFG &= 0xff80;	// clear interrupt flags
	Enable_RE_Interrupt();	// enable interrupt
	
	// enable CLIMIT interrupt
	P1IE |= 1;
	
	unsigned int portData = P1IN;
	RE1.a = (portData & BIT1) == BIT1 ? 1 : 0;
	RE1.b = (portData & BIT2) == BIT2 ? 1 : 0;
	RE2.a = (portData & BIT4) == BIT4 ? 1 : 0;
	RE2.b = (portData & BIT5) == BIT5 ? 1 : 0;
}

void Disable_RE_Interrupt()
{
	P1IE &= 0x0049;
}

void Enable_RE_Interrupt()
{
	P1IE |= 0x005b;
}

void CLimit_handler()
{
	displayOverCurrent = 1;
}

void RE1_A_handler()
{
	Set_Status_LED(LED_Yellow);
	unsigned int port = P1IN;
	unsigned int a = (port & BIT1) >> 1;
	unsigned int b = (port & BIT2) >> 2;
	
	if (RE1.a == 0 && RE1.b == 0)
	{
		if (a == 1)
		{
			//cw
			Increment_Ilim();
		}
		else
		{
			//ccw
			Decrement_Ilim();
		}
	}
	else if (RE1.a == 0 && RE1.b == 1)
	{
		if (a == 1)
		{
			//ccw
			Decrement_Ilim();
		}
		else
		{
			//cw
			Increment_Ilim();
		}
	}
	else if (RE1.a == 1 && RE1.b == 1)
	{
		if (a == 0)
		{
			//cw
			Increment_Ilim();
		}
		else
		{
			//ccw
			Decrement_Ilim();
		}
	}
	else if (RE1.a == 1 && RE1.b == 0)
	{
		if (a == 0)
		{
			//ccw
			Decrement_Ilim();
		}
		else
		{
			//cw
			Increment_Ilim();
		}
	}
	
	RE1.a = a;
	RE1.b = b;
	Delay();
	Reset_Status_LED(LED_Yellow);
	displayIlim = 1;
	Toggle_Edge(BIT1);
}

void RE1_B_handler()
{
}

void RE1_S1_handler()
{
}

void RE2_A_handler()
{
	Set_Status_LED(LED_Yellow);
	unsigned int port = P1IN;
	unsigned int a = (port & BIT4) >> 4;
	unsigned int b = (port & BIT5) >> 5;
	
	if (RE2.a == 0 && RE2.b == 0)
	{
		if (a == 1)
		{
			//cw
			Increment_Vout();
		}
		else
		{
			//ccw
			Decrement_Vout();
		}
	}
	else if (RE2.a == 0 && RE2.b == 1)
	{
		if (a == 1)
		{
			//ccw
			Decrement_Vout();
		}
		else
		{
			//cw
			Increment_Vout();
		}
	}
	else if (RE2.a == 1 && RE2.b == 1)
	{
		if (a == 0)
		{
			//cw
			Increment_Vout();
		}
		else
		{
			//ccw
			Decrement_Vout();
		}
	}
	else if (RE2.a == 1 && RE2.b == 0)
	{
		if (a == 0)
		{
			//ccw
			Decrement_Vout();
		}
		else
		{
			//cw
			Increment_Vout();
		}
	}
	
	RE2.a = a;
	RE2.b = b;
	Delay();
	Reset_Status_LED(LED_Yellow);
	displayVset = 1;
	Toggle_Edge(BIT4);
}

void RE2_B_handler()
{
}

void RE2_S1_handler()
{
}

static void Delay()
{
	for (int i = 0; i < 100; i++);
}

void Toggle_Edge(unsigned int bit)
{
	unsigned int reg = P1IES;
	Disable_RE_Interrupt();	
	if ((reg & bit) == bit)
	{
		P1IES &= ~bit;
	}
	else
	{
		P1IES |= bit;
	}
	
	P1IFG &= 0xff80;
	Enable_RE_Interrupt();
}