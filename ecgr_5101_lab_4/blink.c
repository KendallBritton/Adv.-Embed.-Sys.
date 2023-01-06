/*
 * Kendall Britton      ECGR 5101 Lab 4
 * This program utilizes interrupts and timers
 * to implement a stopwatch using two 7-Sed LEDs where one
 * will count in seconds and the other tenths of a second.
 *
 */

#include <msp430.h>
#include "stdint.h"
#include "common.h"
#include "DIN.h"
#include "DOUT.h"
#include "pin_common.h"

typedef enum {START,INCREMENT,PAUSE,SWITCH,TOTAL_STATES} state;   // Defines all the states that will be used

void initializePorts(void);                                     // Function that sets up/initailizes the ports

// Group of functions associated with all the states
void beginSTART();
void beginINCREMENT();
void beginPAUSE();
void beginSWITCH();
void (*state_table[TOTAL_STATES])() = {beginSTART,beginINCREMENT,beginPAUSE,beginSWITCH};

// Allows us to keep track of present and previous states for comparison
state nowState;
state lastState;

// Group of functions associated with the timer
void timerDelay(int);
void tenSec(int subIter);
void sec(void);
unsigned int totalTime;
unsigned int iter = 0;
unsigned int subIter = 0;


void main(void)
{
    initializePorts();  // Sets up the ports

    nowState = START;   // Puts the timer in the start up state

    while(1){
        if(nowState == START){              // Calls Start function if in start state
            beginSTART();
        } else if(nowState == INCREMENT){   // Calls Increment function if in increment state
            beginINCREMENT();
        } else if(nowState == PAUSE){       // Calls Pause function if in pause state
            beginPAUSE();
        } else if(nowState == SWITCH){        // Calls SWITCH function if in switch state
            beginSWITCH();
        }
    }
}

// Initializes and properly sets up all the ports on the microcontroller
void initializePorts(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Turns off the watchdog timer

    gpioSetOP(GPIO_PORT_P1,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);  // Sets up all the ports as outputs expect for input on pin 1.3
    gpioSetOP(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6);  // Sets up all the ports as outputs

    P1REN |= BIT3;  // P1.3 Pull up resistor enabled
    P1IE |= BIT3;   // P1.3 interrupt enabled
    P1IES |= BIT3;  // P1.3 Hi/lo edge
    P1IFG &= ~BIT3; // P1.3 IFG cleared

    BCSCTL1 = CALBC1_1MHZ;  // Sets MCLK to 1 MHz
    DCOCTL  = CALDCO_1MHZ;  // Sets SMCLK to 1 MHz

    TACCR0 = 0;                        // Initially, Stop the Timer
    TACCTL0 |= CCIE;                   // Enable interrupt for CCR0.
    TACTL = TASSEL_2 + ID_0 + MC_1;    // Select SMCLK, SMCLK/1, Up Mode

    __enable_interrupt();              // Enables the interrupt protocol for the microcontroller
}

// Function that sets up the timer to operate at a specific rate i.e. 0.1 sec
void timerDelay(int tenDelay){
        totalTime = 0;                  // Sets the overall time back to 0
        TACCR0 = 1000-1;                // Start Timer, compare value for Up Mode to get 1ms sec delay per loop
                                        // Total count = TACCR0 + 1. Hence we need to subtract 1.

        while(totalTime <= tenDelay){   // Compares the total time to keep running the timer once started
            if(nowState == SWITCH){       // If the total time exceeds the delay time (.1 sec) then switch function acts accordingly
                break;
            }
        }

        TACCR0 = 0;                     // Turns the timer back off
}

// Function that allows the tenths decimals spot to count from 0 to 9 based on the seconds count
void tenSec(int subIter){
    while(nowState == INCREMENT){           // Increment the numbers while in the increment state
        if((subIter == 0)){
            P2OUT = 0x40;                   // Sets the output number to 0
            subIter++;
            timerDelay(100);                        // Wait 0.1 Secs
        } else if(subIter == 1){
            P2OUT = 0x79;                   // Sets the output number to 1
            subIter++;
            timerDelay(100);                        // Wait 0.1 Secs
        } else if(subIter == 2){
            P2OUT = 0x24;                   // Sets the output number to 2
            subIter++;
            timerDelay(100);                        // Wait 0.1 Secs
        } else if(subIter == 3){
            P2OUT = 0x30;                   // Sets the output number to 3
            subIter++;
            timerDelay(100);                        // Wait 0.1 Secs
        } else if(subIter == 4){
            P2OUT = 0x19;                   // Sets the output number to 4
            subIter++;
            timerDelay(100);                        // Wait 0.1 Secs
        } else if(subIter == 5){
            P2OUT = 0x12;                   // Sets the output number to 5
            subIter++;
            timerDelay(100);                        // Wait 0.1 Secs
        } else if(subIter == 6){
            P2OUT = 0x02;                   // Sets the output number to 6
            subIter++;
            timerDelay(100);                        // Wait 0.1 Secs
        } else if(subIter == 7){
            P2OUT = 0x78;                   // Sets the output number to 7
            subIter++;
            timerDelay(100);                        // Wait 0.1 Secs
        } else if(subIter == 8){
            P2OUT = 0x00;                   // Sets the output number to 8
            subIter++;
            timerDelay(100);                        // Wait 0.1 Secs
        } else if(subIter == 9){
            P2OUT = 0x90;                   // Sets the output number to 9
            subIter = 0;
            timerDelay(100);                        // Wait 0.1 Secs
            break;
        }
    }
}

// Function that allows the ones decimal spot to count from 0 to 9 sequentially
void sec(void){
    while(nowState == INCREMENT){           // Increment the numbers while in the increment state
        if (iter == 0){
        P1OUT = 0x88;                       // Sets the output number to 0
        tenSec(subIter);
        iter++;
        }
        else if (iter == 1){
        P1OUT = 0xF9;                       // Sets the output number to 1
        tenSec(subIter);
        iter++;
        }
        else if (iter == 2){
        P1OUT = 0x4C;                       // Sets the output number to 2
        tenSec(subIter);
        iter++;
        }
        else if (iter == 3){
        P1OUT = 0x68;                       // Sets the output number to 3
        tenSec(subIter);
        iter++;
        }
        else if (iter == 4){
        P1OUT = 0x39;                       // Sets the output number to 4
        tenSec(subIter);
        iter++;
        }
        else if (iter == 5){
        P1OUT = 0x2A;                       // Sets the output number to 5
        tenSec(subIter);
        iter++;
        }
        else if (iter == 6){
        P1OUT = 0x0A;                       // Sets the output number to 6
        tenSec(subIter);
        iter++;
        }
        else if (iter == 7){
        P1OUT = 0xF8;                       // Sets the output number to 7
        tenSec(subIter);
        iter++;
        }
        else if (iter == 8){
        P1OUT = 0x08;                       // Sets the output number to 8
        tenSec(subIter);
        iter++;
        }
        else if (iter == 9){
        P1OUT = 0x28;                       // Sets the output number to 9
        tenSec(subIter);
        iter = 0;
        }
    }
}

// Increment state function that allows the timer to count up to 9.9 seconds
void beginINCREMENT(void){
    while(nowState == INCREMENT){   // Increment the numbers while in the increment state
        sec();                      // Call the sec function which calls the tenSec function to display digits
    }
}

// Pause state function that stops the timer completely
void beginPAUSE(void){
    while(nowState == PAUSE);       // Pause on the current number in the pause state
}

// Switch state function that switches states based on present state
void beginSWITCH(void){
    timerDelay(300);
    if(lastState == START){             // Switches from Start state to Increment state
        nowState = INCREMENT;
    }else if(lastState == INCREMENT){   // Switches from Increment state to Pause state
        nowState = PAUSE;
    }else if(lastState == PAUSE){       // Switches from Pause state to Start state
        nowState = START;
    }
}

// Start state function that the program will boot up in
void beginSTART(void)
{
        P1OUT = 0x88;       // Outputs a zero on the LED
        P2OUT = 0x40;       // Outputs a zero on the LED
        totalTime = 0;      // Resets the total time back to zero
        iter = 0;           // Resets the seconds counter back to 0
        subIter = 0;        // Resets the tenths seconds counter back to 0
}

// Timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void)
{
    totalTime++;    // Increments the total time after a full iteration
}

// Button ISR to switch states
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    TACCR0 = 0;             // Turns the timer off
    totalTime = 0;          // Sets the total timer to zero
    lastState = nowState;   // Keeps track of the previous state before switch
    nowState = SWITCH;        // Goes to switch to new state

    P1IFG &= ~BIT3;         // Clears the interrupt flag
}
