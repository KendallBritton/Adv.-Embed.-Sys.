/*
 * Kendall Britton      ECGR 5101 Grad Project
 * This program utilizes an ultrasonic sensor
 * and 7-seg LEDs to measure the distance of
 * a particular room.
 */

#include <msp430.h>

int milliseconds;   // variable to store milliseconds
int distance;       // variable to store the overall distance in cm
long sensor;        // variable to store the sensor signal
int stopSig = 0;    // variable to freeze the signal pulse

void main(void)
{
    void initializePorts();     // Call to the function that initializes the board ports
    void displayValue(void);    // Call to the function that converts and displays the LED value
    void delayWait10ms(int n);  // Call to the function that provides a 10 ms delay

	initializePorts();  // Sets up the ports

	while(1)
	{
	    while(stopSig == 1)     // Puts into a forever loop if pause button is pressed
	    {
	        __no_operation();
	        P2IFG &= ~BIT4;         // Clears the interrupt flag
	    }

	    P1IE &= ~0x01;              // Disable interupt
	    P1DIR |= 0x02;              // Trigger pin as output
	    P1OUT |= 0x02;              // Generate pulse
	    __delay_cycles(10);         // Delay for 10us
	    P1OUT &= ~0x02;             // Stop pulse
	    P1DIR &= ~0x04;             // Make pin P1.2 input (ECHO)
	    P1IFG = 0x00;               // Clear flag just in case anything happened before
	    P1IE |= 0x04;               // Enable interupt on ECHO pin
	    P1IES &= ~0x04;             // Rising edge on ECHO pin
	    __delay_cycles(30000);      // Delay for 30ms (after this time echo times out if there is no object detected)
	    distance = sensor/58;       // Converting ECHO lenght into cm
	    delayWait10ms(2);           // Wait 20 ms to see LED value


	    if(distance < 1000 && distance != 0)    // Checks to see if distance is between 0 and meters
	    {
	        displayValue();     // Display the current distance value
	    }
	}
}

// This function sets up all the I/O ports on the board
void initializePorts(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Turns off the watchdog timer
    P1DIR |= 0xF1;              // Sets P1.0 as Output
    P2DIR |= 0xEF;              // Sets P2 as Output
    P1SEL |= 0x00;
    P2SEL |= 0x00;
    P2SEL &= ~BIT6;             // Enables the XIN (P2.7) for output

    P2REN |= BIT4;              // P1.3 Pull up resistor enabled
    P2IE |= BIT4;               // P1.3 interrupt enabled
    P2IES |= BIT4;              // P1.3 Hi/lo edge
    P2IFG &= ~BIT4;             // P1.3 IFG cleared

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;       // Make the sub-main clk 1 MHz

    CCTL0 = CCIE;               // CCR0 interrupt enabled
    CCR0 = 1000;                // 1ms at 1mhz
    TACTL = TASSEL_2 + MC_1;    // SMCLK, upmode

    P1IFG  = 0x00;              // Clear all interrupt flags

    _BIS_SR(GIE);               // Global interrupt enable

}

// This function converts the ADC value to the associated LED value
void displayValue(void)
{
    volatile int x, y;
    x = distance;
    y = 0;


    // Set of if statements to control the one's digit of LED
     if((x >= 0 * 10) && (x <= 9.9 * 10))
    {
        P1OUT = 0x0F;
        y = 0;

    } else if((x >= 10 * 10) && (x <= 19.9 * 10))
    {
        P1OUT = 0x1F;
        y = 1;

    } else if((x >= 20 * 10) && (x <= 29.9 * 10))
    {
        P1OUT = 0x4F;
        y = 2;

    } else if((x >= 30 * 10) && (x <= 39.9 * 10))
    {
        P1OUT = 0x5F;
        y = 3;

    } else if((x >= 40 * 10) && (x <= 49.9 * 10))
    {
        P1OUT = 0x2F;
        y = 4;

    } else if((x >= 50 * 10) && (x <= 59.9 * 10))
    {
        P1OUT = 0x3F;
        y = 5;

    } else if((x >= 60 * 10) && (x <= 69.9 * 10))
    {
        P1OUT = 0x6F;
        y = 6;

    } else if((x >= 70 * 10) && (x <= 79.9 * 10))
    {
        P1OUT = 0x7F;
        y = 7;

    } else if((x >= 80 * 10) && (x <= 89.9 * 10))
    {
        P1OUT = 0x8F;
        y = 8;

    } else if((x >= 90 * 10) && (x <= 99.9 * 10))
    {
        P1OUT = 0x9F;
        y = 9;

    } else {

        P1OUT = 0x0F;
    }


    // Set of if-statements to fix the sub-meter portion of LED
    if((x > 99.9) && (y == 1))
    {
        x = x - 100;
    }

    if((x > 99.9) && (y == 2))
    {
        x = x - 200;
    }

    if((x > 99.9) && (y == 3))
    {
        x = x - 300;
    }

    // Set of if statements to control the tenth's digit of LED
     if((x >= 0) && (x <= 9.9))
    {
        P2OUT = 0xF0;

    } else if((x >= 10) && (x <= 19.9))
    {
        P2OUT = 0xF1;

    } else if((x >= 20) && (x <= 29.9))
    {
        P2OUT = 0xF4;

    } else if((x >= 30) && (x <= 39.9))
    {
        P2OUT = 0xF5;

    } else if((x >= 40) && (x <= 49.9))
    {
        P2OUT = 0xF2;

    } else if((x >= 50) && (x <= 59.9))
    {
        P2OUT = 0xF3;

    } else if((x >= 60) && (x <= 69.9))
    {
        P2OUT = 0xF6;

    } else if((x >= 70) && (x <= 79.9))
    {
        P2OUT = 0xF7;

    } else if((x >= 80) && (x <= 89.9))
    {
        P2OUT = 0xF8;

    } else if((x >= 90) && (x <= 99.9))
    {
        P2OUT = 0xF9;

    } else {

        P2OUT = 0xF0;
    }

}

// Function to implement a 10 ms delay
void delayWait10ms(int n)
{
    volatile int time;
    while(n){
        time = 727240*2/91;
        while(time)
        {
            time--;
        }
        n--;
    }
}


// ISR to send pulses to ultrasonic
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IFG&0x04)               // Is there an interrupt pending?
        {
          if(!(P1IES&0x04))      // Is this the rising edge?
          {
            TACTL|=TACLR;        // Clears timer A
            milliseconds = 0;
            P1IES |= 0x04;       // Falling edge
          }
          else
          {
            sensor = (long)milliseconds*1000 + (long)TAR;    // Calculating ECHO length

          }
    P1IFG &= ~0x04;             // Clear flag
    }
}

// Timer ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  milliseconds++;
}

// Button ISR to switch states
#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
    P1OUT |= 0x40;
    if(stopSig == 1)
    {
        stopSig = 0;
    } else if (stopSig == 0)
    {
        stopSig = 1;
    }

    P2IFG &= ~BIT4;         // Clears the interrupt flag
}

