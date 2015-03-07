#include "system.h"
#include "dac.h"

static Vout_Data_t Vout;
static Ilim_Data_t Ilim;

void System_Init()
{
	__disable_interrupt();
	
	// reset variables
	Vout.Vset = 0;
	Vout.Vmeasured = 0;
	Vout.DAC_value = 0;
	Vout.ADC_value = 0;
	Ilim.DAC_value = DAC_ILIM_MAX_VALUE / 2;
	
	
	// config all IO pins as GPIO outputs
	P1SEL = P2SEL = P3SEL = P4SEL = P5SEL = P6SEL = P7SEL = 0;
	P1OUT = P2OUT = P3OUT = P4OUT = P5OUT = P6OUT = P7OUT = 0;
	P1DIR = P2DIR = P3DIR = P4DIR = P5DIR = P6DIR = P7DIR = 0xff;

	// config REF Module
	P5SEL |= 1;
	REFCTL0 |= REFMSTR | REFON | REFVSEL1 | REFOUT;
	
	// set current limit to the minimum value
	Increment_Ilim();

	__enable_interrupt();
}


Vout_Status_t Increment_Vout()
{
	if (Vout.DAC_value < DAC_VOUT_MAX_VALUE)
	{
		Vout.DAC_value += DAC_VOUT_INC_DEC_VALUE;
		Calculate_Vout();
		DAC_Command(DACB, Vout.DAC_value);
		return Vout_Increment_Success;
	}
	
	return Vout_Increment_Fail;
}

Vout_Status_t Decrement_Vout()
{
	if (Vout.DAC_value > DAC_VOUT_MIN_VALUE)
	{
		Vout.DAC_value -= DAC_VOUT_INC_DEC_VALUE;
		Calculate_Vout();
		DAC_Command(DACB, Vout.DAC_value);
		return Vout_Decrement_Success;
	}
	
	return Vout_Decrement_Fail;
}

Ilim_Status_t Increment_Ilim()
{
	if (Ilim.DAC_value < DAC_ILIM_MAX_VALUE)
	{
		Ilim.DAC_value += DAC_ILIM_INC_DEC_VALUE;
		Calculate_Ilim();
		DAC_Command(DACA, Ilim.DAC_value);
		return Ilim_Increment_Success;
	}
	
	return Ilim_Increment_Fail;
}

Ilim_Status_t Decrement_Ilim()
{
	if (Ilim.DAC_value > DAC_ILIM_MIN_VALUE)
	{
		Ilim.DAC_value -= DAC_ILIM_INC_DEC_VALUE;
		Calculate_Ilim();
		DAC_Command(DACA, Ilim.DAC_value);
		return Ilim_Decrement_Success;
	}
	
	return Ilim_Decrement_Fail;
}

Vout_Data_t Get_Vout()
{
	return Vout;
}

void Set_Vout(Vout_Data_t newVout)
{
	Vout = newVout;
	Calculate_Vout();
}

void Set_Status_LED(LED_t bits)
{
	P7OUT |= bits;
}

void Reset_Status_LED(LED_t bits)
{
	P7OUT &= ~bits;
}

Ilim_Data_t Get_Ilim()
{
	return Ilim;
}
	
static void Calculate_Vout()
{
	Vout.Vset = (DAC_VREF * Vout.DAC_value) / DAC_RESOLUTION * VOUT_GAIN;
	Vout.Vmeasured = ADC_VREF * Vout.ADC_value / ADC_RESOLUTION * VSENSE_DIVIDER;
}

static void Calculate_Ilim()
{
	Ilim.Vset = (DAC_VREF * Ilim.DAC_value) / DAC_RESOLUTION / DIFF_AMP_GAIN;
}