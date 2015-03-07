#ifndef SYSTEM_H
#define SYSTEM_H

#include "msp430.h"

typedef enum
{
	Vout_Increment_Success = 0,
	Vout_Increment_Fail,
	Vout_Decrement_Success,
	Vout_Decrement_Fail
} Vout_Status_t;

typedef enum
{
	Ilim_Increment_Success = 0,
	Ilim_Increment_Fail,
	Ilim_Decrement_Success,
	Ilim_Decrement_Fail
} Ilim_Status_t;

typedef struct 
{
	double Vset;
	double Vmeasured;
	unsigned int DAC_value;
	unsigned int ADC_value;
} Vout_Data_t;

typedef struct 
{
	double Vset;
	unsigned int DAC_value;
} Ilim_Data_t;


typedef enum
{
	LED_Yellow = BIT5,
	LED_Red = BIT6
} LED_t;

#define DAC_VOUT_INC_DEC_VALUE 10
#define DAC_ILIM_INC_DEC_VALUE 10
#define DAC_VOUT_MAX_VALUE 0x3ff
#define DAC_VOUT_MIN_VALUE 0
#define DAC_ILIM_MAX_VALUE 490
#define DAC_ILIM_MIN_VALUE 280
#define DAC_RESOLUTION 1024
#define DAC_VREF 2.5
#define ADC_RESOLUTION 4095
#define ADC_VREF DAC_VREF
#define VOUT_GAIN 2
#define VSENSE_DIVIDER 2
#define DIFF_AMP_GAIN 3

void System_Init();
Vout_Status_t Increment_Vout();
Vout_Status_t Decrement_Vout();
Ilim_Status_t Increment_Ilim();
Ilim_Status_t Decrement_Ilim();
Vout_Data_t Get_Vout();
Ilim_Data_t Get_Ilim();
void Set_Vout(Vout_Data_t);
void Set_Status_LED(LED_t);
void Reset_Status_LED(LED_t);
static void Calculate_Vout();
static void Calculate_Ilim();

#endif