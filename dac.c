#include "dac.h"

void DAC_Init()
{
	_BIC_SR(GIE);
	
	// 1. Set UCSWRST
	UCB1CTL1 |= UCSWRST;
	
	// 2. Initialize all USCI registers with UCSWRST = 1 (including UCxCTL1)
	UCB1CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC;
	UCB1CTL1 |= UCSSEL1;
	UCB1BR0 = 0x05;
	UCB1BR1 = 0x05;
	
	// 3. Configure ports
	P4DIR |= 0x0a;
	P4SEL |= 0x0a;
	P3OUT |= CS;
	P3DIR |= CS;
	
	// 4. Clear UCSWRST via software
	UCB1CTL1 &= ~UCSWRST;
	
	_BIS_SR(GIE);
}

void DAC_Command(dac_t dac, unsigned int value)
{
	unsigned int value_buf = 0;
	value = value << 2;
	// reset CS
	P3OUT &= ~CS;
	__no_operation();
	__no_operation();
	
	// send 4 config & 4 data bits
	value_buf = (value >> 8) | dac;
	UCB1TXBUF = value_buf;
	while((UCB1STAT & UCBUSY) == UCBUSY);
	
	// send other 6 data bits
	UCB1TXBUF = value & 0xff;
	while((UCB1STAT & UCBUSY) == UCBUSY);
	
	// set CS
	__no_operation();
	__no_operation();
	P3OUT |= CS;
}