/*
 * Kendall Britton      ECGR 5101 Lab 2
 * This program activates and  operates a 7-Seg LED that
 * goes through the values 0-E or 0-15 in HEX
 *
 */

#include <msp430.h>				

void main(void)
{
    void initializePorts(); // Call to the function that initializes the board ports
    float readADC();    // Call to the function that sets up/reads the ADC value
    void displayValue(void);    // Call to the function that converts and displays the LED value

	volatile unsigned int i, number;    // Volatile variables to prevent optimization

	initializePorts();  // Sets up the ports

	while(1)
	{
	    displayValue(); // Updates the value on the LED
		for(i=10000; i>0; i--); // Delay to smooth out transition
	}
}



void initializePorts(void)      // This function sets up all the I/O ports on the board
{
    WDTCTL = WDTPW | WDTHOLD;   // Turns off the watchdog timer
    P1DIR |= 0x00;              // Sets P1 as Input
    P2DIR |= 0xFF;              // Sets P2 as Output
    P1SEL |= 0x00;
    P2SEL |= 0x00;
    P2SEL &= ~BIT6;             // Enables the XIN (P2.7) for output

}

float readADC(void)     // This function reads the updated value from the ADC
{
    volatile float ADC_value;
    ADC10AE0 = 0x01;            // Select channel A0
    ADC10CTL1 = INCH_0 + ADC10DIV_3;    // Select channel A0, ADC10CLK/3
    ADC10CTL0 = ADC10SHT_3 + MSC + ADC10ON + ADC10IE; // Sample/hold 64 cycle, mult sample, turn on, and interrupt enable
    ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
    ADC_value = ADC10MEM;   //Assigns value held in ADC10MEM
    return ADC_value;
}

void displayValue(void)    // This function converts the ADC value to the associated LED value
{
    volatile int x;
    x = readADC();

    if((x >= 0) && (x <= 63.4))
    {
        P2OUT = 0xC0;

    } else if((x >= 63.5) && (x <= 126.8))
    {
        P2OUT = 0xF9;

    } else if((x >= 126.9) && (x <= 190.3))
    {
        P2OUT = 0xA4;

    } else if((x >= 190.4) && (x <= 253.8))
    {
        P2OUT = 0xB0;

    } else if((x >= 253.9) && (x <= 317.3))
    {
        P2OUT = 0x99;

    } else if((x >= 317.4) && (x <= 380.8))
    {
        P2OUT = 0x92;

    } else if((x >= 380.9) && (x <= 444.3))
    {
        P2OUT = 0x82;

    } else if((x >= 444.4) && (x <= 507.8))
    {
        P2OUT = 0xF8;

    } else if((x >= 507.9) && (x <= 571.3))
    {
        P2OUT = 0x80;

    } else if((x >= 571.4) && (x <= 634.8))
    {
        P2OUT = 0x98;

    } else if((x >= 634.9) && (x <= 698.3))
    {
        P2OUT = 0x88;

    } else if((x >= 698.4) && (x <= 761.8))
    {
        P2OUT = 0x83;

    } else if((x >= 761.9) && (x <= 825.3))
    {
        P2OUT = 0xC6;

    } else if((x >= 825.4) && (x <= 888.8))
    {
        P2OUT = 0xA1;

    } else if((x >= 888.9) && (x <= 952.3))
    {
        P2OUT = 0x86;

    } else if((x >= 952.4) && (x <= 1015.8))
    {
        P2OUT = 0x8E;

    } else {

        P2OUT = 0xBF;
    }
}

