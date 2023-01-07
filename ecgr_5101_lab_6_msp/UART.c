/*
 * UART.c
 *
 *  Created on: Feb 4, 2019
 *      Author: Shantanu
 */

#include "msp430g2553.h"
#include "common.h"
#include "pin_common.h"
#include "UART.h"

const struct PinDescriptor RX = {GPIO_PORT_P1, GPIO_PIN1};
const struct PinDescriptor TX  = {GPIO_PORT_P1, GPIO_PIN2};

void uartInit(void) {
    // Set values for clock
    DCOCTL = 0;                 // Clear DCO
    BCSCTL1 = CALBC1_1MHZ;      // Set to 1MHz
    DCOCTL = CALDCO_1MHZ;

    uartPinInit();
    uartConfigureCR();

}

void uartPinInit(void) {
    // Set SEL & SEL2 to 11 for USCI mode.
    pinSetSEL(RX.port, RX.pin | TX.pin | TX.pin);
    pinSetSEL2(RX.port, RX.pin | TX.pin | TX.pin);
}

void uartConfigureCR(void) {
    UCA0CTL1 |= UCSWRST;         // Disable USCI, reset mode
    UCA0CTL1 |= UCSSEL_2;        // SMCLK
    UCA0BR0 = 104;               // 1MHz 9600
    UCA0BR1 = 0;                 // 1MHz 9600
    UCA0MCTL = UCBRS0;           // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;        // Initialize USCI state machine
}

void uartCheckTxReady(void) {
    while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?
}

void uartCheckRxReady(void) {
    while (!(IFG2 & UCA0RXIFG)); // USCI_A0 RX Received?
}

uint8_t uartReceiveChar(void) {
    uartCheckRxReady();
    return UCA0RXBUF;
}

void uartTransmitChar(uint8_t data) {
    uartCheckTxReady();
    UCA0TXBUF = data;
//    while(UCA0STAT & UCBUSY);
//    return UCA0RXBUF;
}

