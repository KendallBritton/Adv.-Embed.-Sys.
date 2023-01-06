/*
 * Kendall Britton      ECGR 5101 Lab 3
 * This program utilizes UART communication to
 * control two LEDs using a potentiometer
 *
 */

#include <msp430.h>
#include <msp430g2553.h>
#include <UART.h>
#include <stdint.h>
#include <common.h>
#include <pin_common.h>
#include <PWM.h>
#include <DOUT.h>
#include <DIN.h>

 void initializePorts(); // Call to the function that initializes the board ports
 float readADC();    // Call to the function that sets up/reads the ADC value
 void displayValue(int count);    // Call to the function that converts and displays the LED value

void main(void)
{

	initializePorts();  // Sets up the ports

    if((gpioGetVal(GPIO_PORT_P1, GPIO_PIN6) == GPIO_HIGH)){     // Represents board 1
        while(1){
            float adcVal = readADC();                           // Calculates value from the ADC
            int convert = (int)((adcVal/3.3)*100);              // Converts ADC value to an 8 bit value to represent brightness level
            displayValue(convert);                              // Displays the brightness level
            uartTransmitChar(convert);                          // Transmits the 8 bits of info to the receiver
       }
     }

    if((gpioGetVal(GPIO_PORT_P1, GPIO_PIN6) == GPIO_LOW)){      // Represents board 2
        while(1){
            displayValue(uartReceiveChar());                    // Receives and displays the  transmitted information
        }
	}
}

// This function sets up all the I/O ports on the board
void initializePorts(void)
{
    WDTCTL = WDTPW | WDTHOLD;                                   // Turns off the watchdog timer
    uartInit();                                                 // Initializes and sets up all the UART functions
    gpioSetIP(GPIO_PORT_P1, GPIO_PIN6);                         // Sets up P1.6 as an input pin for both boards
    gpioSetOP(GPIO_PORT_P2, GPIO_PIN0);                         // Sets up P2.0 as an output pin for both boards
    pinSetSEL(GPIO_PORT_P1, GPIO_PIN0);                         // Sets up the potentiometer input to be read
}

// This function reads the updated value from the ADC
float readADC(void)
{
    volatile float ADC_value;
    ADC10AE0 = 0x01;                                            // Select channel A0
    ADC10CTL1 = INCH_0 + ADC10DIV_3;                            // Select channel A0, ADC10CLK/3
    ADC10CTL0 = ADC10SHT_3 + MSC + ADC10ON + ADC10IE;           // Sample/hold 64 cycle, mult sample, turn on, and interrupt enable
    ADC10CTL0 |= ENC + ADC10SC;                                 // Sampling and conversion start
    ADC_value = ADC10MEM;                                       // Assigns value held in ADC10MEM
    ADC_value = ((float)ADC10MEM)*(3.3/1023.0);                 // Masks the ADC_value into a float value
    return ADC_value;
}

// This function implements a delay to a clk cycle to vary brightness
void displayValue(int count)
{
    volatile unsigned int i;
    gpioSetVal(GPIO_PORT_P2,GPIO_PIN0, GPIO_HIGH);              //  Turns on and blinks the led to output a certain brightness
    for(i = 0; i < count;i++){
        __delay_cycles(75);
    }
    gpioSetVal(GPIO_PORT_P2,GPIO_PIN0, GPIO_LOW);               // Turns off and blinks the LED to output a certain brightness
    for(i = 0; i < (99-count);i++){
        __delay_cycles(75);
    }
}
