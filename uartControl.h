/*
 * uartControl.h
 *
 *  Created on: May 15, 2017
 *      Author: calvin
 */

#ifndef UARTCONTROL_H_
#define UARTCONTROL_H_

/* UART Output defs */
#define NUMCHARS 4      /* Precision of UART output */
#define MAKEASCII 0x30  /* add to integer to make readable char */

/* methods */
void UART0_init(void);
void sendUART(char *sequence, int length);

#endif /* UARTCONTROL_H_ */
