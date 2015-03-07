#ifndef DAC_H
#define DAC_H

#include "msp430.h"

#define CS BIT7

typedef enum {DACA = 0x30, DACB = 0xb0} dac_t;

void DAC_Init();
void DAC_Command(dac_t d, unsigned int value);

#endif