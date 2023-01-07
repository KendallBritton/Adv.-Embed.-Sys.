/*
 * Kendall Britton      ECGR 5101 Lab 8
 * This program utilizes the joystick off
 * the Educational Boosterpack alongside the TIVA to move
 * a 3x3 ball on the LCD in accordance of playing a 1 Player
 * game of Pong.
 */

#include "TM4C123.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
#include <math.h>

// Defining ADC boundaries corresponding for +/- 10 degrees
// Values are taken by measuring ADC values
#define MAX_Y 2500
#define MIN_Y 1800

uint32_t uin32ADC0Value[8] = {0,0,0,0,0,0,0,0};     // ADC values read

int xi = 0;    // Initial location
int yi = 0;
int xf = 0;     // Final location
int yf = 0;
int dx = 0;     // Steps to move
int dy = 0;


int xiPaddle = 15;    // Initial location
int yiPaddle = 70;
int xfPaddle = 0;     // Final location
int yfPaddle = 0;
int dxPaddle = 0;     // Steps to move
int dyPaddle = 0;

int yRead = 0;        // Stores the y coordinate ADC value
int lrDirection = 1;  // Allocates the left/right direction of the ball
int udDirection = 0;  // Allocates the up/down direction of the ball

// Function definitions
void intializePorts(void);
void delayWait10ms(uint32_t n);
void updatePaddleLocation(void);
void updateBallLocation(void);
void circle(int x, int y);
void paddle(int x, int y);
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

// Array that draws a 2 x 15 paddle
const uint16_t Paddle[]={
     0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0x0000,0x0000,0xFFFF,0xFFFF,
     0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
};


int seedCount = 150;    // Variable used to change the seed on Rand()
int iter = 0;           // Variable used to standardize speed
int iterChange = 1;     // Variable used to update ball location for different speeds


int main(void)
{
    intializePorts();   // Function call to setup ports

    PLL_Init(1);     // Sets screen to run at 80 MHz

    ST7735_InitR(INITR_REDTAB);     // Sets up the screen

    delayWait10ms(500);             // Wait to see results
    ST7735_FillScreen(0xFFFF);      // Clears the screen ~ makes it black
    delayWait10ms(500);             // Wait to see results

    xi = (rand() % (121)) + 60;     // Randomizes start value
    yi = (rand() % (125)) + 10;     // Randomizes start value

    dx = (rand() % (4)) + 1;        // Randomizes initial angle value
    dy = (rand() % (4)) + 1;        // Randomizes initial angle value

    circle(xi,yi);      // Starts the ball in center of screen

    paddle(xiPaddle,yiPaddle);

    while(1)
    {
     adcSampleY();                  // Samples for y coordinate
     srand(seedCount);              // Seed for the rand() function
     updatePaddleLocation();        // Updates the current pixel location of paddle
     updateBallLocation();          // Updates the current pixel location of ball
     seedCount--;                   // Update seed for true randomization
     if(seedCount == 0)             // Loop to reset the seed
     {
         seedCount = 150;
     }
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

    GPIOPinTypeADC(GPIO_PORTD_BASE, 0x08);

    ADCSequenceDisable(ADC0_BASE, 0);

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH4 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 0);

    ADCIntClear(ADC0_BASE, 0);

}

// Function to update the proper location of ball
void updateBallLocation(void)
{
    // Updating X Values
    if((lrDirection == 0))     // Move to right
    {

           if(xf > 120)     // Border
           {
               xf = 120;
           }

           if(xf == 120)    // Border
           {
               lrDirection = 1;

           }

           for(iter = dx; iter > 0; iter--){    // Speed allocation

               xf = xi + iterChange;
               circle(xf,yi);

           }
           xi = xf;

           if(xf < -6)      // Border
           {
               xi = (rand() % (121)) + 60;      // Randomize restart value
               delayWait10ms(100);              // Wait to see results
               ST7735_FillScreen(0xFFFF);       // Clears the screen ~ makes it white
               xiPaddle = 15;                   // Reset paddle coordinate
               yiPaddle = 70;                   // Reset paddle coordinate
               yi = (rand() % (125)) + 10;      // Randomize restart value
               dx = (rand() % (4)) + 1;         // Randomize angle value
               dy = (rand() % (4)) + 1;         // Randomize angle value
           }


     }

    if((lrDirection == 1))     // Move to left
    {

           if(xf > 120)     // Border
           {
               xf = 120;
           }

           if((xf == xiPaddle) && ((yi <= yiPaddle + 5) && (yi >= yiPaddle - 10)))      // Detects paddle collision
           {
               lrDirection = 0;
           }

           for(iter = dx; iter > 0; iter--){        // Speed allocation

               xf = xi - iterChange;
               circle(xf,yi);

           }
           xi = xf;

           if(xf < -6)      // Border
           {
               xi = (rand() % (121)) + 60;      // Randomize restart value
               delayWait10ms(100);              // Wait to see results
               ST7735_FillScreen(0xFFFF);       // Clears the screen ~ makes it white
               xiPaddle = 15;                   // Reset paddle coordinate
               yiPaddle = 70;                   // Reset paddle coordinate
               yi = (rand() % (125)) + 10;      // Randomize restart value
               dx = (rand() % (4)) + 1;         // Randomize angle value
               dy = (rand() % (4)) + 1;         // Randomize angle value

           }

    }

    if((udDirection == 0))     // Move upwards
    {

           if(yf < 10)       // Border
           {
               yf = 10;
           }

           if(yf == 10)     // Border
           {
               udDirection = 1;
           }

           if(yf > 134)     // Border
           {
               yf = 134;
           }

           if(yf == 134)    // Border
           {
               udDirection = 0;
           }


           for(iter = dy; iter > 0; iter--){        // Speed allocation

               yf = yi - iterChange;
               circle(xi,yf);

           }
           yi = yf;

    }

    if((udDirection == 1))     // Move downwards
    {

           if(yf < 10)       // Border
           {
               yf = 10;
           }

           if(yf == 10)     // Border
           {
               udDirection = 1;
           }


           if(yf > 134)     // Border
           {
               yf = 134;
           }

           if(yf == 134)    // Border
           {
               udDirection = 0;
           }

           for(iter = dy; iter > 0; iter--){        // Speed Allocation

               yf = yi + iterChange;
               circle(xi,yf);

           }
           yi = yf;

    }


}

// Function to update the proper location of paddle
void updatePaddleLocation(void)
{
    int z;
    delayWait10ms(5);       // Update every 50 ms

    // Updating Y Values
    if((yRead < MAX_Y) && (yRead > MIN_Y))      // Stop movement
    {
       for(z = 0; z < 1; z++)
       {
           dyPaddle = 0;
           yfPaddle = yiPaddle - dyPaddle;

           if(yfPaddle < 18)       // Border
           {
               yfPaddle = 18;
           }

           if(yfPaddle > 131)      // Border
           {
               yfPaddle = 131;
           }

           paddle(xiPaddle,yfPaddle);
           yiPaddle = yfPaddle;
       }
    }

    if((yRead > MAX_Y))     // Move upwards
    {
       for(z = 0; z < 1; z++)
       {
           dyPaddle = 1;
           yfPaddle = yiPaddle - dyPaddle;

           if(yfPaddle < 18)       // Border
           {
               yfPaddle = 18;
           }

           if(yfPaddle > 131)     // Border
           {
               yfPaddle = 131;
           }

           paddle(xiPaddle,yfPaddle);
           yiPaddle = yfPaddle;
       }
    }

    if((yRead < MIN_Y))     // Move downwards
    {
       for(z = 0; z < 1; z++)
       {
           dyPaddle = 1;
           yfPaddle = yiPaddle + dyPaddle;

           if(yfPaddle < 18)       // Border
           {
               yfPaddle = 18;
           }

           if(yfPaddle > 131)     // Border
           {
               yfPaddle = 131;
           }

           paddle(xiPaddle,yfPaddle);
           yiPaddle = yfPaddle;
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

// Function to draw the pixels of the paddle
void paddle(int x, int y)
{
    ST7735_DrawBitmap(x, y, Paddle, 5, 17);

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

    yRead = (float)uin32ADC0Value[0];

    return yRead;

}

