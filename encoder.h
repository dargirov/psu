#ifndef ENCODER_H
#define ENCODER_H

#include "msp430.h"

typedef struct
{
	unsigned int a;
	unsigned int b;
	unsigned int s;
}  RE_Data;

static void Delay();
void Encoders_Init();
void CLimit_handler();
void RE1_A_handler();
void RE1_B_handler();
void RE1_S1_handler();
void RE2_A_handler();
void RE2_B_handler();
void RE2_S1_handler();
void Toggle_Edge(unsigned int);
void Disable_RE_Interrupt();
void Enable_RE_Interrupt();

#endif