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

uint32_t RX_BUFFER[] = {0};

uint32_t uin32ADC0Value[8] = {0,0,0,0,0,0,0,0};     // ADC values read

int xi = 0;    // Initial location
int yi = 0;
int xf = 0;     // Final location
int yf = 0;
int dx = 0;     // Steps to move
int dy = 0;

int playerOne = 0;
int playerTwo = 0;

int xiPaddle1 = 15;    // Initial location
int yiPaddle1 = 70;
int xfPaddle1 = 0;     // Final location
int yfPaddle1 = 0;
int dxPaddle1 = 0;     // Steps to move
int dyPaddle1 = 0;

int xfPaddle2 = 113;     // Final location
int yfPaddle2 = 70;

uint32_t clk;
uint32_t baud;

int yRead = 0;      // Stores the y coordinate ADC value
int lrDirection = 1;  // Allocates the direction of the ball
int udDirection = 0;  // Allocates the direction of the ball

char TX_BUFFER[] = {0xFF};

int seedCount = 150;
int iter = 0;
int iterChange = 1;

float PWM_FREQ = 10000;
float CPU_FREQ = 120000000;
float pwm_word;

// Function definitions
void intializePorts(void);
void delayWait10ms(uint32_t n);
void updatePaddleLocation(void);
void updatePaddleLocationR(void);
void updateBallLocation(void);
void circle(int x, int y);
void paddle(int x, int y);
float adcSampleY(void);
void buzzer(void);
void UARTRx(void);
void UARTTx(void);
void resetButton(void);
void displayScore(int);

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

//Creates a paddle to be displayed
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

    xi = (rand() % (121)) + 60;
    yi = (rand() % (125)) + 10;

    dx = (rand() % (4)) + 1;
    dy = (rand() % (4)) + 1;

    circle(xi,yi);      // Starts the ball in center of screen

    paddle(xiPaddle1,yiPaddle1);
    paddle(xfPaddle2, yfPaddle2);


    while(1)
    {
     //delayWait10ms(5);          // Updates every 50 ms
     resetButton();
     adcSampleY();       // Samples for y coordinate
     srand(seedCount);
     updatePaddleLocation();          // Updates the current pixel location of paddle
     UARTRx();
     paddle(xfPaddle2, yfPaddle2);
     updateBallLocation();          // Updates the current pixel location of ball
     seedCount--;
     if(seedCount == 0)
     {
         seedCount = 150;
     }
    }

}

void resetButton(void){

    if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6) == 0x00)
        {
            xi = (rand() % (121)) + 60;
            delayWait10ms(100);             // Wait to see results
            ST7735_FillScreen(0xFFFF);      // Clears the screen ~ makes it white
            xiPaddle1 = 15;
            yiPaddle1 = 70;
            yi = (rand() % (125)) + 10;
            dx = (rand() % (4)) + 1;
            dy = (rand() % (4)) + 1;

            delayWait10ms(100);
        }
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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);        // Enables port D GPIO peripheral
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
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

    FPULazyStackingEnable();

        // Enable FPU
    FPUEnable();

    clk = SysCtlClockGet();
    baud  = clk/(16-5);

    //UARTDisable(UART1_BASE);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
    UARTDisable(UART1_BASE);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART1_BASE, clk, baud,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
    UARTEnable(UART1_BASE);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6);
}

void UARTTx(void){
    //while(!UARTBusy(UART1_BASE)){}
    UARTCharPut(UART1_BASE,TX_BUFFER[0]);
}

void UARTRx(void){
    //while(!UARTBusy(UART1_BASE)){}
    if(UARTCharGet(UART1_BASE) == 'S'){
        yfPaddle2 = yfPaddle2;
    }else if(UARTCharGet(UART1_BASE) == 'U'){
        yfPaddle2--;
    }else if(UARTCharGet(UART1_BASE) == 'D'){
        yfPaddle2++;
    }

}

void buzzer(void){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,GPIO_PIN_2);
        delayWait10ms(50);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0x00);
}

// Function to update the proper location of ball
// Main author: Kendall
// Helped debugging the function and creating: Mateo and Jacob
void updateBallLocation(void)
{
    //delayWait10ms(15);
    //buzzer();
    // Updating X Values
    if((lrDirection == 0))     // Move to right
    {
           //xf = xi + dx;


           if(xf > 120)
           {
               xf = 120;
           }

           if((xf == xfPaddle2) && ((yi <= yfPaddle2 + 5) && (yi >= yfPaddle2 - 13)))
           {
               lrDirection = 1;
           }


         for(iter = dx; iter > 0; iter--){

               xf = xi + iterChange;
               circle(xf,yi);

           }

           xi = xf;

           if(xf > 134)      // Border
           {
               TX_BUFFER[0] = 'B';
               UARTTx();
               playerOne++;
               displayScore(1);
               xi = (rand() % (121)) + 60;
               delayWait10ms(200);             // Wait to see results, makes this 2 seconds!!
               ST7735_FillScreen(0xFFFF);      // Clears the screen ~ makes it white
               xiPaddle1 = 15;
               yiPaddle1 = 70;
               xfPaddle2 = 113;
               yfPaddle2 = 70;
               lrDirection = 1;
               yi = (rand() % (125)) + 10;
               dx = (rand() % (4)) + 1;
               dy = (rand() % (4)) + 1;
          }

     }

    if((lrDirection == 1))     // Move to left
    {
           //xf = xi - dx;


           if(xf > 120)
           {
               xf = 120;
           }

           if((xf == xiPaddle1) && ((yi <= yiPaddle1 + 5) && (yi >= yiPaddle1 - 13)))
           {
               lrDirection = 0;
           }

           for(iter = dx; iter > 0; iter--){

               xf = xi - iterChange;
               circle(xf,yi);

           }
           xi = xf;

           if(xf < -6)      // Border
           {
               buzzer();
               xi = (rand() % (61)) ;
               playerTwo++;
               displayScore(2);
               delayWait10ms(200);             // Wait to see results
               ST7735_FillScreen(0xFFFF);      // Clears the screen ~ makes it white
               xiPaddle1 = 15;
               yiPaddle1 = 70;
               xfPaddle2 = 113;
               yfPaddle2 = 70;
               lrDirection = 0;
               yi = (rand() % (125)) + 10;
               dx = (rand() % (4)) + 1;
               dy = (rand() % (4)) + 1;
           }
    }

    if((udDirection == 0))     // Move upwards
    {

           //yf = yi - dy;

           if(yf < 10)       // Border
           {
               yf = 10;
           }

           if(yf == 10)
           {
               udDirection = 1;
           }

           if(yf > 134)
           {
               yf = 134;
           }

           if(yf == 134)
           {
               udDirection = 0;
           }


           for(iter = dy; iter > 0; iter--){

               yf = yi - iterChange;
               circle(xi,yf);

           }
           yi = yf;

    }

    if((udDirection == 1))     // Move downwards
    {
           //yf = yi + dy;

           if(yf < 10)       // Border
           {
               yf = 10;
           }

           if(yf == 10)
           {
               udDirection = 1;
           }


           if(yf > 134)
           {
               yf = 134;
           }

           if(yf == 134)
           {
               udDirection = 0;
           }

           for(iter = dy; iter > 0; iter--){

               yf = yi + iterChange;
               circle(xi,yf);

           }
           yi = yf;

    }


}

// Function to update the proper location of paddle
// Main Author: Mateo
// Helped debugging function: Kendall and Jacob
void updatePaddleLocation(void)
{
    int z;
    delayWait10ms(5);

    // Updating Y Values
    if((yRead < MAX_Y) && (yRead > MIN_Y))      // Stop movement
    {
       for(z = 0; z < 1; z++)
       {
           dyPaddle1 = 0;
           yfPaddle1 = yiPaddle1 - dyPaddle1;

           if(yfPaddle1 < 18)       // Border
           {
               yfPaddle1 = 18;
           }

           if(yfPaddle1 > 131)
           {
               yfPaddle1 = 131;
           }

           paddle(xiPaddle1,yfPaddle1);
           yiPaddle1 = yfPaddle1;
       }
    }


    if((yRead > MAX_Y))     // Move upwards
    {
       for(z = 0; z < 1; z++)
       {
           dyPaddle1 = 1;
           yfPaddle1 = yiPaddle1 - dyPaddle1;

           if(yfPaddle1 < 18)       // Border
           {
               yfPaddle1 = 18;
           }

           if(yfPaddle1 > 131)
           {
               yfPaddle1 = 131;
           }

           paddle(xiPaddle1,yfPaddle1);
           yiPaddle1 = yfPaddle1;
       }
    }


    if((yRead < MIN_Y))     // Move downwards
    {
       for(z = 0; z < 1; z++)
       {
           dyPaddle1 = 1;
           yfPaddle1 = yiPaddle1 + dyPaddle1;

           if(yfPaddle1 < 18)       // Border
           {
               yfPaddle1 = 18;
           }

           if(yfPaddle1 > 131)
           {
               yfPaddle1 = 131;
           }

           paddle(xiPaddle1,yfPaddle1);
           yiPaddle1 = yfPaddle1;
       }
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

// Function to draw the pixels of the ball
// Author: All
void circle(int x, int y)
{
    ST7735_DrawBitmap(x, y, Circle_Dia_3, 12, 12);

}

// Function to draw the pixels of the paddle
// Author: All
void paddle(int x, int y)
{
    ST7735_DrawBitmap(x, y, Paddle, 5, 17);
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

void displayScore(int player){
    if(player == 1){
        if(playerOne == 0){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '0', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 1){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '1', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 2){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0,'2', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 3){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '3', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 4){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '4', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 5){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '5', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 6){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '6', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 7){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '7', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 8){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '8', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 9){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '9', ST7735_Color565(255,0,0),0,4);
        }else if(playerOne == 10){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'1', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '10', ST7735_Color565(255,0,0),0,4);
        }
    }else if(player == 2){
        if(playerTwo == 0){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '0', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 1){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '1', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 2){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0,'2', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 3){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '3', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 4){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '4', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 5){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '5', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 6){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '6', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 7){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '7', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 8){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '8', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 9){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '9', ST7735_Color565(255,0,0),0,4);
        }else if(playerTwo == 10){
            ST7735_DrawCharS(0,0,'P', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(30,0,'2', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(64,0,':', ST7735_Color565(255,0,0),0,4);
            ST7735_DrawCharS(102,0, '10', ST7735_Color565(255,0,0),0,4);
        }
    }
}

/*
 * extra code:
//    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)){}
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
//
//    GPIOPinConfigure(GPIO_PF2_M1PWM6);
//    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
//    pwm_word= (1/PWM_FREQ)*CPU_FREQ;
//
//    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN|PWM_GEN_MODE_NO_SYNC);
//    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 400);
//    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 200);
//    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
//    PWMOutputState(PWM1_BASE,PWM_OUT_6_BIT, true);
//void buzzer(void){
//    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,300);
//    SysCtlDelay(10000);
//    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,100);
//    SysCtlDelay(10000);
//}
 */
