//***********************************************************************************************************************
//Lab : 2-Player Pong
//12/5/21
// Jacob Smith & Mateo Cubillos & Kendall Britton
// Version 1.1

// This code utilizes both the Tiva board and the educational booster pack Mk.2 to create a version of Pong.
// This code is one player based which is represented with one paddle on the left. The goal of the game is to keep
// the ball from crossing the left side of the screen.

// void intializePorts(void),void updateBallLocation(void), void updatePaddleLocation(void)
//void delayWait10ms(uint32_t n), void circle(int x, int y), void paddle(int x, int y)
//float adcSampleY(void)

//Global Variables Used
//int xi;int yi;int xf;int yf;
//int dx;int dy;int xiPaddle;
//int yiPaddle;int xfPaddle;int yfPaddle;
//int dxPaddle;int dyPaddle;int yRead;
//int lrDirection;//int udDirection;
//int seedCount;int iter;int iterChange;
// Dr. Conrad gave us permission to keep the braces not indented for our if and else if statements.
//************************************************************************************************************************
/*
 *
 *  Copyright 2016 by ABHILASH DR, adevalap@uncc.edu
 *  You may use, edit, run or distribute this file
 *  as long as the above copyright notice remains
 *  THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *  OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *  AUTHOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 *  OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
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
#include "driverlib/pwm.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"


// Defining ADC boundaries corresponding for +/- 10 degrees
// Values are taken by measuring ADC values
#define MAX_Y 2500
#define MIN_Y 1800

uint32_t uin32ADC0Value[8] = {0,0,0,0,0,0,0,0};     // ADC values read

int yRead = 0;      // Stores the y coordinate ADC value
char TX_BUFFER[] = {0xFF};

uint32_t clk;
uint32_t baud;

int xiPaddle = 15;    // Initial location
int yiPaddle = 70;
int xfPaddle = 0;     // Final location
int yfPaddle = 0;
int dxPaddle = 0;     // Steps to move
int dyPaddle = 0;

// Function definitions
void intializePorts(void);
float adcSampleY(void);
void updatePaddleLocation(void);
void UARTTx(void);
void delayWait10ms(uint32_t);
void buzzer(void);
/*
 * Main function to infinitely display the ball and paddles coordinates and make sure the game runs
 * Its initializes all the ports and pins needed for this lab.
 * Author: Mateo, Kendall, and Jacob
 */
int main(void)
{
    intializePorts();   // Function call to setup ports

    PLL_Init(3);     // Sets screen to run at 80 MHz

    ST7735_InitR(INITR_REDTAB);     // Sets up the screen

    delayWait10ms(500);             // Wait to see results
    ST7735_FillScreen(0xFFFF);      // Clears the screen ~ makes it black
    delayWait10ms(500);             // Wait to see results


    while(1)
    {
     adcSampleY();       // Samples for y coordinate
     updatePaddleLocation();          // Updates the current pixel location of paddle
    }

}

// Function to wait 10 ms for how many times n
// Main author: Karim
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

void buzzer(void){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,GPIO_PIN_2);
        delayWait10ms(50);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0x00);
}

// Initializes all the ports/adc for the program
// Author: Kendall, Mateo, and Jacob
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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    clk = SysCtlClockGet();
    baud  = clk/(16-5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
    //UARTDisable(UART1_BASE);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART1_BASE, clk, baud,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
    UARTEnable(UART1_BASE);

}

void UARTTx(void){
    //while(!UARTBusy(UART1_BASE)){}
    UARTCharPut(UART1_BASE,TX_BUFFER[0]);
}

void updatePaddleLocation(void)
{
    int z;
    //delayWait10ms(5);

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

           if(yfPaddle > 131)
           {
               yfPaddle = 131;
           }

           TX_BUFFER[0] = 'S';
           UARTTx();
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

           if(yfPaddle > 131)
           {
               yfPaddle = 131;
           }

           TX_BUFFER[0] = 'U';
           UARTTx();
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

           if(yfPaddle > 131)
           {
               yfPaddle = 131;
           }

           TX_BUFFER[0] = 'D';
           UARTTx();
           yiPaddle = yfPaddle;
       }
    }


}

// Function to sample the y coordinate of accelerometer
// Author: Mateo
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
