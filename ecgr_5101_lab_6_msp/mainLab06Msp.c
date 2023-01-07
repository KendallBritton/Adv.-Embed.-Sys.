/*
 * Kendall Britton      ECGR 5101 Lab 6 MSP
 * This program takes in inputs and decodes the
 * input to return its corresponding output
 * message.
 */

#include <msp430.h>
#include "stdint.h"
#include "common.h"
#include "DIN.h"
#include "DOUT.h"
#include "pin_common.h"

#include <msp430g2553.h>


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Turns off watchdog timer
    P1DIR |= 0xF0;              // Sets first four bits as input
    P2DIR |= 0xFF;              // Set all of Port 2 as outputs
    P1SEL |= 0x00;              // Clears all of Port 1 to use GPIO function
    P2SEL |= 0x00;              // Clears all of Port 2 to use GPIO function
    P2SEL &= ~BIT6;             // Enables the XIN (P2.6) for output
    P2SEL &= ~BIT7;             // Enables Pin 2.6 for output

    while(1)
    {
        if(P1IN & 0x08 == 0x08){                // Code to decode input message based off table in Lab 6
            P2OUT = 0x01;
        } else if ((P1IN & 0x09) == 0x09){
            P2OUT = 0x02;
        } else if ((P1IN & 0x0A) == 0x0A){
            P2OUT = 0x04;
        } else if ((P1IN & 0x0B) == 0x0B){
            P2OUT = 0x08;
        } else if ((P1IN & 0x0C) == 0x0C){
            P2OUT = 0x10;
        } else if ((P1IN & 0x0D) == 0x0D){
            P2OUT = 0x20;
        } else if ((P1IN & 0x0E) == 0x0E){
            P2OUT = 0x40;
        } else if ((P1IN & 0x0F) == 0x0F){
            P2OUT = 0x80;
        } else {
            P2OUT = 0x32;
        }

    }
}

