/******************************************************************************/
/*               University of North Carolina at Charlotte                    */
/*                           ECE Department                                   */
/*                                                                            */
/*                                                                            */
/* File Name:   common.c                                                      */
/* Module Name: Basic Software                                                */
/* Project: M.Sc. Thesis: Autonomous ATV Control                              */
/* File Version:  1.0       Created by: Shantanu M                            */
/* Base_line:     Initial_Release                                             */
/* Created:       Sep 26, 2018                                                */
/*                                                                            */
/*                                                                            */
/* Description:                                                               */
/*    Version 1.0:                                                            */
/*     - Clear and Set functions for SEL, SEL2 and REN registers              */
/*     - Set pin pullup or pulldown function                                  */
/*                                                                            */
/* References: http://www.argenox.com/library/msp430/                         */
/*                  general-purpose-input-output-gpio-chapter-5/              */
/******************************************************************************/

/*
 * common.c
 *
 *  Created on: Sep 26, 2018
 *      Author: Shantanu
 */

#include "pin_common.h"
#include "DIN.h"
#include "DOUT.h"
#include "common.h"

void pinClearSEL(unsigned int port, int pin) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];
    (*((volatile int *)(portAddress + GPIO_SEL_REG_OFFSET))) &= ~pin;
}

void pinSetSEL(unsigned int port, int pin) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];
    (*((volatile int *)(portAddress + GPIO_SEL_REG_OFFSET))) |= pin;
}

void pinClearSEL2(unsigned int port, int pin) {
    (*((volatile int *)(port + 1 + GPIO_SEL2_REG_OFFSET))) &= ~pin;
}

void pinSetSEL2(unsigned int port, int pin) {
    (*((volatile int *)(port + 1 + GPIO_SEL2_REG_OFFSET))) |= pin;
}

void pinSetREN(unsigned int port, int pin) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];
    (*((volatile int *)(portAddress + GPIO_REN_REG_OFFSET))) |= pin;
}

void pinClearREN(unsigned int port, int pin) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];
    (*((volatile int *)(portAddress + GPIO_REN_REG_OFFSET))) &= ~pin;
}

//When the pin is in input mode and REN is enabled, P1OUT register selects whether the resistors is a pull-up (1) or pull-down (0)
void pinSetPull(unsigned int port, int pin, int mode) {
    pinSetIP(port, pin);
    pinSetSEL2(port, pin);
    if (mode) gpioSetVal(port, pin, PIN_HIGH);
    else gpioSetVal(port, pin, PIN_LOW);
}
