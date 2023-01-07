/*
 * common.h
 *
 *  Created on: Sep 26, 2018
 *      Author: Shantanu
 */

#ifndef COMMON_H_
#define COMMON_H_

/*
 * Function: pinClearSEL
 * ----------------------------
 *   Clears the pin's SEL register.
 *
 *   port: port number of the pin.
 *   pin: pin number whose SEL register is to be cleared.
 *
 *   returns: clears SEL register, returns nothing.
 */
void pinClearSEL(unsigned int port, int pin);

/*
 * Function: pinSetSEL
 * ----------------------------
 *   Sets the pin's SEL register.
 *
 *   port: port number of the pin.
 *   pin: pin number whose SEL register is to be set.
 *
 *   returns: sets SEL register, returns nothing.
 */
void pinSetSEL(unsigned int port, int pin);

/*
 * Function: pinClearSEL2
 * ----------------------------
 *   Clears the pin's SEL2 register.
 *
 *   port: port number of the pin.
 *   pin: pin number whose SEL2 register is to be cleared.
 *
 *   returns: clears SEL2 register, returns nothing.
 */
void pinClearSEL2(unsigned int port, int pin);

/*
 * Function: pinSetSEL2
 * ----------------------------
 *   Sets the pin's SEL2 register.
 *
 *   port: port number of the pin.
 *   pin: pin number whose SEL2 register is to be set.
 *
 *   returns: sets SEL2 register, returns nothing.
 */
void pinSetSEL2(unsigned int port, int pin);

/*
 * Function: pinSetREN
 * ----------------------------
 *   Sets the pin's REN register.
 *
 *   port: port number of the pin.
 *   pin: pin number whose REN register is to be set.
 *
 *   returns: sets REN register, returns nothing.
 */
void pinSetREN(unsigned int port, int pin);

/*
 * Function: pinClearREN
 * ----------------------------
 *   Clears the pin's REN register.
 *
 *   port: port number of the pin.
 *   pin: pin number whose REN register is to be cleared.
 *
 *   returns: clears REN register, returns nothing.
 */
void pinClearREN(unsigned int port, int pin);

/*
 * Function: pinSetPull
 * ----------------------------
 *   Set's the pin as a pullup or pulldown.
 *
 *   port: port number of the pin.
 *   pin: pin number whose pin is to be set as pullup or pulldown.
 *   mode: pullup or pulldown mode.
 *
 *   returns: set's pin pull based on the mode passed, returns nothing.
 */
void pinSetPull(unsigned int port, int pin, int mode);

#endif /* COMMON_H_ */
