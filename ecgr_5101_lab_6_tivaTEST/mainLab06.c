/* File name: LabOne.c
 * Target board: TIVA C: Tiva TM4C123GH6PM
 * Compiler version: TI v16.9.0.LTS
 * Description: This file provides the template for LabOne.
 * The completed lab will have the following functionalities:
 * 1. Blink on-board Red LED for 10 times at 0.5s blink intervals when left switch
 * on Tiva C board is pressed.
 * 2. Blink on-board Blue LED for 5 times at 1s blink intervals when right switch
 * on Tiva C board is pressed.
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
#include <stdint.h>
#include<stdio.h>
#include <string.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "inc/hw_nvic.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/systick.h"
#include "buttons.h"

/****************************************************/
//Declare Preprocessor definition
/****************************************************/
#define PORTF         0x40025000  // GPIO Port F
#define PIN4          0x10  // GPIO pin 4

/****************************************************/
//Declare the prototype of the functions here
/****************************************************/
void init(void);
void test(void);
void normalOp(void);
void Display(unsigned int,unsigned int,bool);
void displayError(unsigned int,unsigned int,bool);
void toString(char[],int);
void eightBit(void);
void buttonStart(void);
void reverseInit(void);
void initButton(void);
void checkPins(void);

unsigned int tivaInE;
unsigned int tivaInB ;
unsigned int tivaInA1;
unsigned int tivaInA2;
unsigned int tivaIn;
//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif




/****************************************************/
//Start of main routine
/****************************************************/

/****************************************************/
/* Function: main
 * Description: main routine to capture the switch
 * press and call LED blink function
 * Arguments: void
 * Return: 0 on clean exit, integer other than 0 for
 * any error
 */
/****************************************************/
int main(void){
    volatile float value;
    SysCtlClockSet(SYSCTL_SYSDIV_16|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    initButton();

    reverseInit();
    checkPins();

    init();
    //Display(0x0A);
    int isSW1Down = 0;
    int input = 0;
    while(1){
        input = (uint8_t)GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
        isSW1Down = ~(input >> 4) & 0x01;

        if(isSW1Down){
            normalOp();
            eightBit();
        }
    }
}

void checkPins(){
    unsigned volatile int j;
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000100);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000001);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b11111100);
    for(j =0; j<160000;j++);

    tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5) & GPIO_PIN_4|GPIO_PIN_5);//0b10000000
    tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_1|GPIO_PIN_0) & GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_1|GPIO_PIN_0);//0b01000000

    displayError(0x01,tivaInE,1);
    displayError(0x01,tivaInB,1);

    tivaIn = tivaInE | tivaInB;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
    if(!(tivaIn == 0x00)){
        displayError(0x01,tivaIn,1);
    }else{
        displayError(0x01,tivaIn,0);
    }
}

void reverseInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
}

void initButton(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    GPIOPinTypeGPIOInput(PORTF, GPIO_PIN_4);
    GPIOPadConfigSet(PORTF, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

void init(){

    // Enable the GPIO port that is used for the on-board LED.
    // Check if the peripheral access is enabled.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}

    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);

    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
}

void toString(char str[],int num){
    int i, rem, len = 0,n;

    n= num;
    while(n != 0){
        len++;
        n /= 10;
    }
    for(i = 0;i < len;i++){
        rem = num % 10;
        num = num /10;
        str[len -(i+1)] = rem + '0';
    }
    str[len] = '\0';
}

void Display(unsigned int input,unsigned int output,bool error){//unsigned expected){
//void Display(unsigned int input){
    if(error == 1){
        char str[] = "Error: Output: ";
        if(output == 0){
            strcat(str,"0");
            puts(str);
        }else{
            char cc[5];
            toString(cc,output);
            strcat(str,cc);
            //strcat(str," input: ");
            //toString(cc,input);
            //strcat(str,cc);
            puts(str);
        }
    }else{
        char str[] = "Correct: Output: ";
        if(output == 0){
            strcat(str,"0");
            puts(str);
        }else{
            char cc[5];
            toString(cc,output);
            strcat(str,cc);
            //strcat(str," input: ");
            //toString(cc,input);
            //strcat(str,cc);
            puts(str);
        }
    }
}

void displayError(unsigned int input,unsigned int output,bool error){//unsigned expected){
//void Display(unsigned int input){
    if(error == 1){
        char str[] = "Error: The ports and pins are not setup correctly";
        if(output == 0){
            strcat(str,"0");
            puts(str);
        }else{
            char cc[5];
            toString(cc,output);
            strcat(str,cc);
            //strcat(str," input: ");
            //toString(cc,input);
            //strcat(str,cc);
            puts(str);
        }
    }else{
        char str[] = "The ports and pins are setup correctly ";
        if(output == 0){
            strcat(str,"0");
            puts(str);
        }else{
            char cc[5];
            toString(cc,output);
            strcat(str,cc);
            //strcat(str," input: ");
            //toString(cc,input);
            //strcat(str,cc);
            puts(str);
        }
    }
}


void normalOp(){
    unsigned volatile int i = 0;
    unsigned volatile int j;
    for(i = 0; i< 16; i++){
        if(i == 0){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0xFF){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }

        }else if(i == 1){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00100000);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0xFF){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 2){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000001);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0xFF){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 3){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00100001);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0xFF){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 4){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000010);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0xFF){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 5){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00100010);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0xFF){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 6){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000011);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0xFF){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 7){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00100011);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0xFF){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 8){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000000);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00010000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0x01){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 9){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00100000);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00010000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0x02){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 10){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000001);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00010000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0x04){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 11){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00100001);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00010000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0x08){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 12){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000010);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00010000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0x10){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 13){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00100010);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00010000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0x20){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 14){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00000011);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00010000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0x40){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }else if(i == 15){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00100011);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0b00010000);
            for(j =0; j<160000;j++);

            tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
            tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
            tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
            tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

            tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5
            if(tivaIn == 0x80){
                Display(i,tivaIn,0);
            }else{
                Display(i,tivaIn,1);
            }
        }
    }
}

void eightBit(){
    unsigned volatile int i,j;
    unsigned int B0,B1,B4,B5,B6,B7,E4,E5,BData,EData;
    for(i = 0; i< 256;i++){
        B5 = i << 5;
        B5 = B5 & 0b00100000;
        B0 = i >> 1;
        B0 = B0 & 0b00000001;
        B1 = i >> 1;
        B1 = B1 & 0b00000010;
        E4 = i << 1;
        E4 = E4 & 0b00010000;
        E5 = i << 1;
        E5 = E5 & 0b00100000;
        B4 = i >> 1;
        B4 = B4 & 0b00010000;
        B6 = B6 & 0b01000000;
        B7 = B7 & 0b10000000;

        BData = B7|B6|B5|B4|B1|B0;
        EData = E5|E4;

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, BData);
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, EData);

        for(j =0; j<160000;j++);

        tivaInE = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) & GPIO_PIN_0)<<7;//0b10000000
        tivaInB = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) & GPIO_PIN_2)<<4;//0b01000000
        tivaInA1 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2) & (GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2))<<1;//0b00111000
        tivaInA2 = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5) & (GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5)) >> 5;//0b00000111

        tivaIn = tivaInE | tivaInB | tivaInA1 | tivaInA2;//0b PE0 PB2 PA4 PA3 PA2 PA7 PA6 PA5

        Display(i,tivaIn,0);

    }
}


