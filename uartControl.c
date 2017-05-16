/*
 * uartControl.c
 *
 *  Created on: May 15, 2017
 *      Author: calvin
 */
#include "msp.h"
#include "uartControl.h"

/* Set up UART0 port */
void UART0_init(void) {
    EUSCI_A0->CTLW0 |= 1;       /* put in reset mode for config */
    EUSCI_A0->MCTLW = 0;        /* disable oversampling */
    EUSCI_A0->CTLW0 = 0x0081;   /* 1 stop bit, no parity, SMCLK, 8-bit data */
    EUSCI_A0->BRW = 26;         /* 3,000,000 / 115200 = 26 */
    P1->SEL0 |= (BIT2 | BIT3);  /* P1.3, P1.2 for UART */
    P1->SEL1 &= ~(BIT2 | BIT3);
    EUSCI_A0->CTLW0 &= ~1;      /* take UART out of reset mode */
}

/* takes in sequence of chars to send to UART
 * and length of the sequence
 * writes out the sequence to the UART then returns
 */
void sendUART(char *sequence, int length) {
    int i = 0;
    for (; i < length; i++) {
        while(!(EUSCI_A0->IFG & 0x02)) {}                      /* wait for transmit buffer empty */
        EUSCI_A0->TXBUF = sequence[i];
    }
}
