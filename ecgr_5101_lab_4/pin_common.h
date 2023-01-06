/*
 * pin_common.h
 *
 *  Created on: Sep 14, 2018
 *      Author: Shantanu
 *      References:http://www.ti.com/lit/ds/symlink/msp430g2553.pdf
 */

#ifndef PINCOMMON_H
#define PINCOMMON_H

/************************************************************
* GPIO VALUES
************************************************************/
#define GPIO_HIGH                                                     (1)
#define GPIO_LOW                                                      (0)

/************************************************************
* PIN VALUES
************************************************************/
#define PIN_HIGH                                                      (1)
#define PIN_LOW                                                       (0)

/************************************************************
* GPIO PORTS
************************************************************/
#define GPIO_PORT_P1                                                   (0)
#define GPIO_PORT_P2                                                   (1)
#define GPIO_PORT_P3                                                   (2)

/************************************************************
* GPIO PINS
************************************************************/
#define GPIO_PIN0                                                      (0x0001)
#define GPIO_PIN1                                                      (0x0002)
#define GPIO_PIN2                                                      (0x0004)
#define GPIO_PIN3                                                      (0x0008)
#define GPIO_PIN4                                                      (0x0010)
#define GPIO_PIN5                                                      (0x0020)
#define GPIO_PIN6                                                      (0x0040)
#define GPIO_PIN7                                                      (0x0080)

/************************************************************
* GPIO DIGITAL I/O REGISTERS
************************************************************/
#define GPIO_IN_REG_OFFSET                                             (0x00)
#define GPIO_OUT_REG_OFFSET                                            (0x01)
#define GPIO_DIR_REG_OFFSET                                            (0x02)
#define GPIO_IFG_REG_OFFSET                                            (0x03)
#define GPIO_IES_REG_OFFSET                                            (0x04)
#define GPIO_IE_REG_OFFSET                                             (0x05)
#define GPIO_SEL_REG_OFFSET                                            (0x06)
#define GPIO_REN_REG_OFFSET                                            (0x07)
#define GPIO_SEL2_REG_OFFSET                                           (0x40)

/************************************************************
* MSP430G2553 PORT BASE ADDRESS
************************************************************/
#define GPIO_BASE_PORT1                                                (0x20)
#define GPIO_BASE_PORT2                                                (0x28)
#define GPIO_BASE_PORT3                                                (0x18)

/************************************************************
* MSP430G2553 PORT ADDRESS TABLE MAPPING TABLE
************************************************************/
static const int GPIO_PORT_ADD_TABLE[] = { GPIO_BASE_PORT1,
                                           GPIO_BASE_PORT2,
                                           GPIO_BASE_PORT3 };

/************************************************************
* Structure to define Pin
************************************************************/
struct PinDescriptor {
    unsigned int port;
    unsigned int pin;
};

#endif
