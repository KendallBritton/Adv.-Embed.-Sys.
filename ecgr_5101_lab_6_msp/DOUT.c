/******************************************************************************/
/*               University of North Carolina at Charlotte                    */
/*                           ECE Department                                   */
/*                                                                            */
/*                                                                            */
/* File Name:   DOUT.c                                                        */
/* Module Name: Basic Software                                                */
/* Project: M.Sc. Thesis: Autonomous ATV Control                              */
/* File Version:  1.0       Created by: Shantanu M                            */
/* Base_line:     Initial_Release                                             */
/* Created:       Sep 14, 2018                                                */
/*                                                                            */
/*                                                                            */
/* Description:                                                               */
/*    Version 1.0:                                                            */
/*     - GPIO set output function                                             */
/*     - GPIO set value function                                              */
/*     - Pin set output function                                              */
/*                                                                            */
/* References:http://www.ocfreaks.com/msp430-gpio-programming-tutorial/       */
/******************************************************************************/

/*
 * DOUT.c
 *
 *  Created on: Sep 14, 2018
 *      Author: Shantanu
 */

#include "pin_common.h"
#include "DOUT.h"

void pinSetOP(unsigned int port, int pin) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];
    (*((volatile int *)(portAddress + GPIO_DIR_REG_OFFSET))) |= pin;
}

void gpioSetOP(unsigned int port, int pin) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];
    pinSetOP(port, pin);
    pinClearSEL(port, pin);
}

void gpioSetVal(unsigned int port, int pin, int val) {
    int portAddress = GPIO_PORT_ADD_TABLE[port];
    if (val) (*((volatile int *)(portAddress + GPIO_OUT_REG_OFFSET))) |= pin;
    else (*((volatile int *)(portAddress + GPIO_OUT_REG_OFFSET))) &= ~pin;
}
