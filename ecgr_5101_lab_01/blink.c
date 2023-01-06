#include <msp430g2553.h>


/**
 * blink.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P1DIR |= 0x00;					// configure all P1 as input
	P2DIR |= 0xFF;                  // configure all P2 as output

	volatile unsigned int i;		// volatile to prevent optimization

	while(1)
	{

		P2OUT ^= 0xFF;				// toggle P1.0
		for(i=500000; i>0; i--);     // delay for 1 sec based off 1MHz clk
	}
}
