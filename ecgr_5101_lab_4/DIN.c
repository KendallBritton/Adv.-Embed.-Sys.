/******************************************************************************/
/*               University of North Carolina at Charlotte                    */
/*                           ECE Department                                   */
/*                                                                            */
/*                                                                            */
/* File Name:   DIN.c                                                         */
/* Module Name: Basic Software                                                */
/* Project: M.Sc. Thesis: Autonomous ATV Control                              */
/* File Version:  1.0       Created by: Shantanu M                            */
/* Base_line:     Initial_Release                                             */
/* Created:       Sep 14, 2018                                                */
/*                                                                            */
/*                                                                            */
/* Description:                                                               */
/*    Version 1.0:                                                            */
/*     - GPIO set input function                                              */
/*     - GPIO get value function                                              */
/*     - Pin set input function                                               */
/*                                                                            */
/* References:http://www.ocfreaks.com/msp430-gpio-programming-tutorial/       */
/******************************************************************************/

/*
 * DIN.c
 *
 *  Created on: Sep 14, 2018
 *      Author: Shantanu
 */

#include "pin_common.h"
#include "DIN.h"

void pinSetIP(unsigned int port, int pin) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];
    (*((volatile int *)(portAddress + GPIO_DIR_REG_OFFSET))) &= ~pin;
}

void gpioSetIP(unsigned int port, int pin) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];
    pinSetIP(port, pin);
    (*((volatile int *)(portAddress + GPIO_SEL_REG_OFFSET))) &= ~pin;
}

int gpioGetVal(unsigned int port, int pin) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];

    unsigned int pinVal = (*((volatile int *)(portAddress + GPIO_IN_REG_OFFSET))) & pin;
    if (pinVal > 0) return GPIO_HIGH;
    else return GPIO_LOW;
}
