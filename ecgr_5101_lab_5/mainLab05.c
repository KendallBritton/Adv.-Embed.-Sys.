/*
 * Kendall Britton      ECGR 5101 Lab 5
 * This program activates and  operates a 7-Seg LED that
 * goes through the values 0-E or 0-15 in HEX
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

#define VREF    3.3
#define ADCBIT  4096.0

void displayValue(float voltageValue);    // Call to the function that converts and displays the LED value
float adcSample(void);                    // Function that calculate/sample the ADC value
void intializePorts(void);                // Sets up all the ports to output the 7-Seg LED

uint32_t adcVal[1];                       // Variable that stores the value of the ADC
volatile float voltageValue;              // Variable that take voltage value from array to float


int main(void)
{
    intializePorts();   // Function call to setup ports
    volatile float x;

    while(1)
    {
        x = (4095/3.3) * adcSample();   // Converts the value to a value between 0 - 4095
        displayValue(x);                // Displays the LED value
    }
}


void intializePorts(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // Sets the clock rate

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        // Enables port F GPIO peripheral
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);        // Enables port E GPIO peripheral
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);        // Enables port B GPIO peripheral
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);        // Enables port C GPIO peripheral
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);         // Enables ADC channel 0 peripheral
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    {
    }

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);        // Sets the pin association for the ADC in port E

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);                            // Sets up pin 2 and 3 as output on port F
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);                                         // Sets up pin 3 as output on port B
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);  // Sets up pin 4, 5, 6, and 7 as output on port C

    ADCSequenceDisable(ADC0_BASE, 3);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);          // Configures the ADC to be used with external devices
    ADCSequenceEnable(ADC0_BASE, 3);

    ADCIntClear(ADC0_BASE, 3);
}


float adcSample(void) // Function to sample/calculates the voltage value from the ADC
{
    ADCProcessorTrigger(ADC0_BASE, 3);

    while(!ADCIntStatus(ADC0_BASE, 3, false))
    {
    }

    ADCIntClear(ADC0_BASE, 3);

    ADCSequenceDataGet(ADC0_BASE, 3, adcVal);

    voltageValue = (float)adcVal[0] * (VREF / ADCBIT);

    return voltageValue;
}


void displayValue(float voltageValue)  // This function converts the ADC value to the associated LED value
{
    if((voltageValue >= 0) && (voltageValue <= 255.9))              // Outputs a 0 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_7);

    } else if((voltageValue >= 256) && (voltageValue <= 511.9))     // Outputs a 1 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0 | GPIO_PIN_2);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    } else if((voltageValue >= 512) && (voltageValue <= 767.9))     // Outputs a 2 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_6);

    } else if((voltageValue >= 768) && (voltageValue <= 1023.9))    // Outputs a 3 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_5 | GPIO_PIN_6);

    } else if((voltageValue >= 1024) && (voltageValue <= 1279.9))   // Outputs a 4 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0 | GPIO_PIN_2);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_4 | GPIO_PIN_5);

    } else if((voltageValue >= 1280) && (voltageValue <= 1535.9))   // Outputs a 5 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0 | GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_5);

    } else if((voltageValue >= 1536) && (voltageValue <= 1791.9))   // Outputs a 6 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0 | GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0);

    } else if((voltageValue >= 1792) && (voltageValue <= 2047.9))   // Outputs a 7 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    } else if((voltageValue >= 2048) && (voltageValue <= 2303.9))   // Outputs a 8 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0);

    } else if((voltageValue >= 2304) && (voltageValue <= 2559.9))   // Outputs a 9 on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_4 | GPIO_PIN_5);

    } else if((voltageValue >= 2560) && (voltageValue <= 2815.9))   // Outputs a A on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_4);

    } else if((voltageValue >= 2816) && (voltageValue <= 3071.9))   // Outputs a B on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_2 | GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0);

    } else if((voltageValue >= 3072) && (voltageValue <= 3327.9))   // Outputs a C on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0 | GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_7);

    } else if((voltageValue >= 3328) && (voltageValue <= 3583.9))   // Outputs a D on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0 | GPIO_PIN_2);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_6);

    } else if((voltageValue >= 3584) && (voltageValue <= 3839.9))   // Outputs a E on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x0 | GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0);

    } else if((voltageValue >= 3840) && (voltageValue <= 4095))     // Outputs a F on the 7-Seg LED
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_4);

    } else  // All the components are of the LED are turned off
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1 | GPIO_PIN_2);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x0 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    }
}
