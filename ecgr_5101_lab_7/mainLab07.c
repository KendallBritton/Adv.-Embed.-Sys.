/*
 * Kendall Britton      ECGR 5101 Lab 7
 * This program moves utilizes the accelerometer off
 * the Educational Boosterpack alongside the TIVA to move
 * a 3x3 ball on the LCD based on the tilt/orientation of
 * the board.
 */

#include "TM4C123.h"
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/adc.h"
#include <stdio.h>
#include "ST7735.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"

// Defining ADC boundaries corresponding for +/- 10 degrees
// Values are taken by measuring ADC values
#define MAX_X 2140
#define MAX_Y 2175
#define MIN_X 1880
#define MIN_Y 1915


uint32_t uin32ADC0Value[8] = {0,0,0,0,0,0,0,0};     // ADC values read

int xi = 60;    // Initial location
int yi = 70;
int xf = 0;     // Final location
int yf = 0;
int dx = 0;     // Steps to move
int dy = 0;

int xRead = 0;      // Stores the x coordinate ADC value
int yRead = 0;      // Stores the y coordinate ADC value
int xNow, yNow;     // Takes in values from ADC

// Function definitions
void intializePorts(void);
void delayWait10ms(uint32_t n);
void updateLocation(void);
void circle(int x, int y);
float adcSampleX(void);
float adcSampleY(void);

// Array that draws a 3 x 3 ball
const uint16_t Circle_Dia_3[] = {
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0x0000,
    0x0000,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0x0000,     0x0000,
    0x0000,     0x0000,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0x0000,     0x0000,
    0x0000,     0x0000,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0x0000,
    0x0000,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF
};



int main(void)
{
    intializePorts();   // Function call to setup ports

    PLL_Init(Bus80MHz);     // Sets screen to run at 80 MHz

    ST7735_InitR(INITR_REDTAB);     // Sets up the screen

    delayWait10ms(500);             // Wait to see results
    ST7735_FillScreen(0xFFFF);      // Clears the screen ~ makes it black
    delayWait10ms(500);             // Wait to see results

    circle(xi,yi);      // Starts the ball in center of screen

    while(1)
    {
     delayWait10ms(5);          // Updates every 50 ms
     xNow = adcSampleX();       // Samples for x coordinate
     yNow = adcSampleY();       // Samples for y coordinate
     updateLocation();          // Updates the current pixel location
    }

}

// Initializes all the ports/adc for the program
void intializePorts(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // Sets the clock rate

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);        // Enables port B GPIO peripheral
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);        // Enables port A GPIO peripheral
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);        // Enables port E GPIO peripheral
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinTypeADC(GPIO_PORTD_BASE, 0x03);

    ADCSequenceDisable(ADC0_BASE, 0);

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH7);

    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH6 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 0);

    ADCIntClear(ADC0_BASE, 0);

}

// Function to update the proper location of ball
void updateLocation(void)
{
    int z;

    // Updating X Values
    if((xRead < MAX_X) && (xRead > MIN_X))      // Stop movement
    {
       for(z = 0; z < 1; z++)
       {
           dx = 0;
           xf = xi - dx;

           if(xf < -2)      // Border
           {
               xf = -2;
           }

           if(xf > 120)
           {
               xf = 120;
           }

           circle(xf,yi);
           xi = xf;
       }
    }

    if((xRead > MAX_X))     // Move to right
    {
       for(z = 0; z < 1; z++)
       {
           dx = 1;
           xf = xi + dx;

           if(xf < -2)      // Border
           {
               xf = -2;
           }

           if(xf > 120)
           {
               xf = 120;
           }

           circle(xf,yi);
           xi = xf;
       }
    }

    if((xRead < MIN_X))     // Move to left
    {
       for(z = 0; z < 1; z++)
       {
           dx = 1;
           xf = xi - dx;

           if(xf < -2)      // Border
           {
               xf = -2;
           }

           if(xf > 120)
           {
               xf = 120;
           }

           circle(xf,yi);
           xi = xf;
       }
    }

    // Updating Y Values
    if((yRead < MAX_Y) && (yRead > MIN_Y))      // Stop movement
    {
       for(z = 0; z < 1; z++)
       {
           dy = 0;
           yf = yi - dy;

           if(yf < 10)       // Border
           {
               yf = 10;
           }

           if(yf > 134)
           {
               yf = 134;
           }

           circle(xi,yf);
           yi = yf;
       }
    }

    if((yRead > MAX_Y))     // Move upwards
    {
       for(z = 0; z < 1; z++)
       {
           dy = 1;
           yf = yi - dy;

           if(yf < 10)       // Border
           {
               yf = 10;
           }

           if(yf > 134)
           {
               yf = 134;
           }

           circle(xi,yf);
           yi = yf;
       }
    }

    if((yRead < MIN_Y))     // Move downwards
    {
       for(z = 0; z < 1; z++)
       {
           dy = 1;
           yf = yi + dy;

           if(yf < 10)       // Border
           {
               yf = 10;
           }

           if(yf > 134)
           {
               yf = 134;
           }

           circle(xi,yf);
           yi = yf;
       }
    }


}

// Function to wait 10 ms for how many times n
void delayWait10ms(uint32_t n)
{
    uint32_t volatile time;
    while(n){
        time = 727240*2/91;
        while(time)
        {
            time--;
        }
        n--;
    }
}

// Function to draw the pixels of the ball
void circle(int x, int y)
{
    ST7735_DrawBitmap(x, y, Circle_Dia_3, 12, 12);

}

// Function to sample the x coordinate of accelerometer
float adcSampleX(void)
{
    ADCProcessorTrigger(ADC0_BASE, 0);

    while(!ADCIntStatus(ADC0_BASE, 0, false))
    {
    }

    ADCIntClear(ADC0_BASE, 0);

    ADCSequenceDataGet(ADC0_BASE, 0, uin32ADC0Value);

    xRead = (float)uin32ADC0Value[0];

    return xRead;

}

// Function to sample the y coordinate of accelerometer
float adcSampleY(void)
{
    ADCProcessorTrigger(ADC0_BASE, 0);

    while(!ADCIntStatus(ADC0_BASE, 0, false))
    {
    }

    ADCIntClear(ADC0_BASE, 0);

    ADCSequenceDataGet(ADC0_BASE, 0, uin32ADC0Value);

    yRead = (float)uin32ADC0Value[1];

    return yRead;

}

