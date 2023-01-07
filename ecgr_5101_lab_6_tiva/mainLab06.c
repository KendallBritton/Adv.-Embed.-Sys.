/*
 * Kendall Britton      ECGR 5101 Lab 6 MSP
 * This program test the functionality of the
 * MSP code which takes in inputs and decodes the
 * input to return its corresponding output
 * message.
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include <stdio.h>
#include <string.h>

void intializePorts(void);              // Initializes all the ports
void tostring(char str[], int num);     // Allows outputs to the console
void testShort(int n);                  // Code to send 4 bits to the MSP
void normalOperations(int n);           // Code to test normal operation of MSP
void delay(void);                       // Delay function to allow boards to update
void testLong(int n);                   // Code to send 8 bits to the MSP
void testOppPorts(void);                // Code to test opposite ports


int main(void)
{
    intializePorts();   // Function call to setup ports

    unsigned int i, x;

    puts("Test Lower 4 Bits:");     // Code to run test of first 4 bits

    for(i = 0; i < 16; i++){
        x = i % 16;
        testShort(x);
        delay();
        normalOperations(x);
    }

    puts("Test 8 Bits:");           // Code to run test of all 8 bits

        for(i = 0; i < 16; i++){
            x = i % 16;
            testLong(x);
            delay();
            normalOperations(x);
        }

    puts("Test Opp Ports:");        // Code to run test of switched ports

        for(i = 0; i < 16; i++){
            x = i % 16;
            testOppPorts();
            testShort(x);
            delay();
            normalOperations(x);
            }
}

// This functions allows us to write to the console
void tostring(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }

    for(i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i+1)] = rem + '0';
    }
    str[len] = '\0';
}

// This function sets up and initializes all the ports for the program
void intializePorts(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_16 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // Sets the clock rate

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


    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7);        // Sets up output for Port B
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);                                                                          // Sets up input for Port B

    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);         // Sets up input for Port A

    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);                                                            // Sets up output for Port E
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);                                                                          // Sets up input for Port E

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, 0x00);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);

}

// This code send 4 bits to the MSP to test normal functionality
void testShort(int n)
{
    switch(n)
    {
        case 0:     // Send 0000

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, 0x00);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);

            break;

        case 1:     // Send 0001

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_5);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);

            break;

        case 2:     // Send 0010

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);

            break;

        case 3:     // Send 0011

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_5 | GPIO_PIN_0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);

            break;

        case 4:     // Send 0100

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);

            break;

        case 5:     // Send 0101

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_5 | GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);

            break;

        case 6:     // Send 0110

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0 | GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);

            break;

        case 7:     // Send 0111

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);

            break;

        case 8:     // Send 1000

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, 0x00);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

            break;

        case 9:     // Send 1001

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_5);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

            break;

        case 10:    // Send 1010

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

            break;

        case 11:    // Send 1011

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_5 | GPIO_PIN_0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

            break;

        case 12:    // Send 1100

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

            break;

        case 13:    // Send 1101

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_5 | GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

            break;

        case 14:    // Send 1110

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0 | GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

            break;

        case 15:    // Send 1111

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

            break;

        default:

            // Output default value

            puts("Error default case");

            break;
    }
}

// This code sends 8 bits to the MSP to test normal functionality
void testLong(int n)
{
    switch(n)
    {
        case 0:     // Send 0000

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, 0x00 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | 0x00);

            break;

        case 1:     // Send 0001

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | 0x00);

            break;

        case 2:     // Send 0010

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | 0x00);

            break;

        case 3:     // Send 0011

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | 0x00);

            break;

        case 4:     // Send 0100

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | 0x00);

            break;

        case 5:     // Send 0101

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | 0x00);

            break;

        case 6:     // Send 0110

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | 0x00);

            break;

        case 7:     // Send 0111

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | 0x00);

            break;

        case 8:     // Send 1000

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, 0x00 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | GPIO_PIN_4);

            break;

        case 9:     // Send 1001

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | GPIO_PIN_4);

            break;

        case 10:    // Send 1010

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | GPIO_PIN_4);

            break;

        case 11:    // Send 1011

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | GPIO_PIN_4);

            break;

        case 12:    // Send 1100

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | GPIO_PIN_4);

            break;

        case 13:    // Send 1101

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | GPIO_PIN_4);

            break;

        case 14:    // Send 1110

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | GPIO_PIN_4);

            break;

        case 15:    // Send 1111

            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_5 | GPIO_PIN_4);

            break;

        default:

            // Output default value

            puts("Error default case");

            break;
    }
}

// This code switches the Ports to test erroneous outputs
void testOppPorts(void){
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);

    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1, 0x00);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
}

// This codes test the normal operations of the MSP
void normalOperations(int n)
{
    switch(n)
    {
        case 0:     // Checks 0000 case

            if((GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4) == 0xFC)
            {
                if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIO_PIN_2) == 0x04)
                {
                    if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0)) & (GPIO_PIN_0) == 0x01)
                    {
                    // Output Pass Test
                    int c = 0b0000;
                    char cc[8];
                    tostring(cc,c);
                    puts("Sent: ");
                    puts(cc);


                    c = 0b11111111;
                    tostring(cc,c);
                    puts("Exp: ");
                    puts(cc);

                    c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                    tostring(cc, c);
                    puts("Rec: ");
                    puts(cc);

                    puts("");

                } else
            {
                // Output Fail Test
                //int c = GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4) | GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) | GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0);
                //char cc[8];
                //tostring(cc,c);
                puts("Error wrong Ports");


            }
                    }
                }


            break;

        case 1:     // Checks 0001 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x00)
            {
                    // Output Pass Test
                int c = 0b0001;
                char cc[8];
                tostring(cc,c);
                puts("Sent: ");
                puts(cc);


                c = 0b11111111;
                tostring(cc,c);
                puts("Exp: ");
                puts(cc);

                c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                tostring(cc, c);
                puts("Rec: ");
                puts(cc);

                puts("");

            } else {
                // Outputs Fail Test
                puts("Error wrong Ports");
            }

            break;

        case 2:     // Checks 0010 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x00)
            {
                    // Output Pass Test
                int c = 0b0010;
                char cc[8];
                tostring(cc,c);
                puts("Sent: ");
                puts(cc);


                c = 0b11111111;
                tostring(cc,c);
                puts("Exp: ");
                puts(cc);

                c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                tostring(cc, c);
                puts("Rec: ");
                puts(cc);

                puts("");

                } else
            {
                // Output Fail Test
                puts("Error wrong Ports");

            }

            break;

        case 3:     // Checks 0011 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x00)
            {
                    // Output Pass Test
                int c = 0b0011;
                char cc[8];
                tostring(cc,c);
                puts("Sent: ");
                puts(cc);


                c = 0b11111111;
                tostring(cc,c);
                puts("Exp: ");
                puts(cc);

                c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                tostring(cc, c);
                puts("Rec: ");
                puts(cc);

                puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }

            break;

        case 4:     // Checks 0100 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x00)
            {
                    // Output Pass Test
                int c = 0b0100;
                char cc[8];
                tostring(cc,c);
                puts("Sent: ");
                puts(cc);


                c = 0b11111111;
                tostring(cc,c);
                puts("Exp: ");
                puts(cc);

                c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                tostring(cc, c);
                puts("Rec: ");
                puts(cc);

                puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }

            break;

        case 5:     // Checks 0101 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x00)
            {
                    // Output Pass Test
                int c = 0b0101;
                char cc[8];
                tostring(cc,c);
                puts("Sent: ");
                puts(cc);


                c = 0b11111111;
                tostring(cc,c);
                puts("Exp: ");
                puts(cc);

                c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                tostring(cc, c);
                puts("Rec: ");
                puts(cc);

                puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }

            break;

        case 6:     // Checks 0110 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x00)
            {
                    // Output Pass Test
                int c = 0b0110;
                char cc[8];
                tostring(cc,c);
                puts("Sent: ");
                puts(cc);


                c = 0b11111111;
                tostring(cc,c);
                puts("Exp: ");
                puts(cc);

                c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                tostring(cc, c);
                puts("Rec: ");
                puts(cc);

                puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }

            break;

        case 7:     // Checks 0111 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x00)
            {
                    // Output Pass Test
                int c = 0b0111;
                char cc[8];
                tostring(cc,c);
                puts("Sent: ");
                puts(cc);


                c = 0b11111111;
                tostring(cc,c);
                puts("Exp: ");
                puts(cc);

                c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                tostring(cc, c);
                puts("Rec: ");
                puts(cc);

                puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }

            break;

        case 8:     // Checks 1000 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x10)
            {
                    // Output Pass Test
                int c = 0b1000;
                char cc[8];
                tostring(cc,c);
                puts("Sent: ");
                puts(cc);


                c = 0b00000001;
                tostring(cc,c);
                puts("Exp: ");
                puts(cc);

                c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                tostring(cc, c);
                puts("Rec: ");
                puts(cc);

                puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }

            break;

        case 9:     // Checks 1001 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x10)
            {
                if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5) & (GPIO_PIN_5)) == 0x20)
                {
                    // Output Pass Test
                    int c = 0b1001;
                    char cc[8];
                    tostring(cc,c);
                    puts("Sent: ");
                    puts(cc);


                    c = 0b00000010;
                    tostring(cc,c);
                    puts("Exp: ");
                    puts(cc);

                    c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                    tostring(cc, c);
                    puts("Rec: ");
                    puts(cc);

                    puts("");
                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }
            }

            break;

        case 10:     // Checks 1010 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x10)
            {
                if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0) & (GPIO_PIN_0)) == 0x01)
                {
                    // Output Pass Test
                    int c = 0b1010;
                    char cc[8];
                    tostring(cc,c);
                    puts("Sent: ");
                    puts(cc);


                    c = 0b00000100;
                    tostring(cc,c);
                    puts("Exp: ");
                    puts(cc);

                    c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                    tostring(cc, c);
                    puts("Rec: ");
                    puts(cc);

                    puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }
            }

            break;

        case 11:     // Checks 1011 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x10)
            {
                if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0) & (GPIO_PIN_5 | GPIO_PIN_0)) == 0x21)
                {
                    // Output Pass Test
                    int c = 0b1011;
                    char cc[8];
                    tostring(cc,c);
                    puts("Sent: ");
                    puts(cc);


                    c = 0b00001000;
                    tostring(cc,c);
                    puts("Exp: ");
                    puts(cc);

                    c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                    tostring(cc, c);
                    puts("Rec: ");
                    puts(cc);

                    puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }
           }

            break;

        case 12:     // Checks 1100 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x10)
            {
                if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1) & (GPIO_PIN_1)) == 0x2)
                {
                    // Output Pass Test
                    int c = 0b1100;
                    char cc[8];
                    tostring(cc,c);
                    puts("Sent: ");
                    puts(cc);


                    c = 0b00010000;
                    tostring(cc,c);
                    puts("Exp: ");
                    puts(cc);

                    c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                    tostring(cc, c);
                    puts("Rec: ");
                    puts(cc);

                    puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }
           }

            break;

        case 13:     // Checks 1101 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x10)
            {
                if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_1) & (GPIO_PIN_5 | GPIO_PIN_1)) == 0x22)
                {
                    // Output Pass Test
                    int c = 0b1101;
                    char cc[8];
                    tostring(cc,c);
                    puts("Sent: ");
                    puts(cc);


                    c = 0b00100000;
                    tostring(cc,c);
                    puts("Exp: ");
                    puts(cc);

                    c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                    tostring(cc, c);
                    puts("Rec: ");
                    puts(cc);

                    puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }
           }

            break;

        case 14:     // Checks 1110 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x10)
            {
                if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1 | GPIO_PIN_0) & (GPIO_PIN_1 | GPIO_PIN_0)) == 0x03)
                {
                    // Output Pass Test
                    int c = 0b1110;
                    char cc[8];
                    tostring(cc,c);
                    puts("Sent: ");
                    puts(cc);


                    c = 0b01000000;
                    tostring(cc,c);
                    puts("Exp: ");
                    puts(cc);

                    c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                    tostring(cc, c);
                    puts("Rec: ");
                    puts(cc);

                    puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }
           }

            break;

        case 15:     // Checks 1111 case

            if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & (GPIO_PIN_4)) == 0x10)
            {
                if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1) & (GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1)) == 0x23)
                {
                    // Output Pass Test
                    int c = 0b1111;
                    char cc[8];
                    tostring(cc,c);
                    puts("Sent: ");
                    puts(cc);


                    c = 0b10000000;
                    tostring(cc,c);
                    puts("Exp: ");
                    puts(cc);

                    c |= (GPIOPinRead(GPIO_PORTA_BASE,  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)) & (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)) & (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
                    tostring(cc, c);
                    puts("Rec: ");
                    puts(cc);

                    puts("");

                } else
            {
                // Output Fail Test
                    puts("Error wrong Ports");

            }
           }

            break;

        default:

            // Output the default case

            puts("Error default case");

            break;
    }
}

// Delay functions that allows the boards to process information
void delay(void)
{
    volatile uint32_t ui32Loop;
    for(ui32Loop = 0; ui32Loop < 100000; ui32Loop++)
    {

    }
}

