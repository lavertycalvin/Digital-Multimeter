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

int sampleRate = 0;
int numSamples = 0;

void main(void)
{
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
	
    // Enable global interrupt
    __enable_irq();
    enableADCInt();

    setSampleRate(3000);

    setupADC();
    UART0_init();
    setup_TA0_Int();

    //init DMM
    initTerminal(acVolt, dcVolt, freq);


    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    while(1) {
        /* take 1000, 1ms samples */
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

/* enables timerA0_0 interrupt for waveforms */
void setup_TA0_Int(void) {
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled

    TIMER_A0->CCR[0] = 600;

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31); //enable timer A interrupt (A0)
}

/* handles TA0 Interrrupt */
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear the flag

    TIMER_A0->CCR[0] += setSampleRate //set to collect next sample
}


/* Nyquist Theorem --> at least twice frequency of max
 * MAXFREQ set in defines
 * takes in clock speeds in KHZ
 */
void setSampleRate(int clockSpeed) {
    sampleRate = (clockSpeed * 1000) / (MAXFREQ * 2);
}
