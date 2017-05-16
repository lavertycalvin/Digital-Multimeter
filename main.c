//*****************************************************************************
//
// Digital Multimeter Project
// Authors: Calvin Laverty, Nick Boring, Luke Letwin
//
//****************************************************************************

#include "msp.h"
#include "waveformCalcs.h"
#include "adc.h"
#include "uartControl.h"
#include "terminalControl.h"

#define MAXVOLT     3.3
#define MAXFREQ     1000


int sampleADC = 0;

int acVolt =    1650; /* holds voltage in mV */
int dcVolt =    330; /* holds voltage in mV */
int freq =      400;

int maxSample = 0;
int minSample = 5000;

void main(void)
{
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
	
    // Enable global interrupt
    __enable_irq();
    enableADCInt();


    setupADC();
    UART0_init();

    //init DMM
    initTerminal(acVolt, dcVolt, freq);


    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    while(1) {
        /* take 1 second sample */
        /* make measurements */
        while(!getReady()){} /* wait for the conversion to complete in ISR */
        sampleADC = getSample();
        //voltOut(sampleADC);
        //clearScreen();
        //print(acVolt, sampleADC, freq);

        // Start sampling/conversion
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
        __sleep();

        __no_operation();                   // For debugger

        //sample for 1 second
        //calculate DC value
        //find frequency from dc crossings
        //calc AC values
        //calc realDC value if too far off
    }
}
