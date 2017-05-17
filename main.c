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




int sampleADC = 0;

int acVPP  =    0;   /* all held in mV */
int acRMS =     0;
int acTRUERMS = 0;

int dcVolt =    330; /* holds voltage in mV */
int freq =      400;

int maxSample = 0;
int minSample = 5000;
int totSample = 0; /* holds running total of samples */
int avgSample = 0; /*calculated to go to dcVolt */

int sampleRate = 0;
volatile int numSamples = 0;

int allSamples[2000] = {};

void main(void)
{
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
	
    // Enable global interrupt
    __enable_irq();

    setSampleRate(3000);

    setupADC();
    UART0_init();


    //init DMM
    initTerminal(acVPP, dcVolt, freq);

    setup_TA0_Int();
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    while(1) {
        /* Start NEXT sampling/conversion */
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
        /* take 2000, .005 ms samples */
        /* enable TA0 interrupts for samples of wave */
        resetSampleValues();

        NVIC_EnableIRQ(TA0_0_IRQn);      //enable timer A interrupt (A0)
        while(numSamples < 2000) {
            /* take samples handles by TA interrupt */
            /* update min and max values */
            if(sampleADC > maxSample) {
                maxSample = sampleADC;
            }
            if(sampleADC < minSample) {
                minSample = sampleADC;
            }

        }
        /*disable TA0 interrupts*/
        NVIC_DisableIRQ(TA0_0_IRQn); //done taking samples from TA0

        /*make dc calculations*/
        avgSample = getDCAverage(totSample); /* shouldn't be too costly */
        dcVolt = avgSample;

        acVPP = getACVPP(maxSample, minSample);

        /*calc DC crossings to find freq of the wave*/
        freq = getFreq(dcVolt, allSamples, acVPP, sampleRate); //will return 0 if VPP == 0


        /*write out all info to the screen*/
        updateTerminal(dcVolt, acVPP, 0, 0, freq);
        //calc AC values
        //calc realDC value if too far off
    }
}

/* enables timerA0_0 interrupt for waveforms */
void setup_TA0_Int(void) {
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled

    TIMER_A0->CCR[0] = sampleRate;

    NVIC_EnableIRQ(TA0_0_IRQn);      //enable timer A interrupt (A0)
}

/* handles TA0 Interrupt */
void TA0_0_IRQHandler(void) {
    while(!getReady()){} /* wait for the conversion to complete in ADC ISR */
    sampleADC = getSample();
    clearSampleReady();
    allSamples[numSamples] = sampleADC;
    totSample += sampleADC;

    numSamples++;
    /* Start NEXT sampling/conversion */
    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
    TIMER_A0->CCR[0] += sampleRate; //set to collect next sample
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear the flag
}


/* Nyquist Theorem --> at least twice frequency of max
 * MAXFREQ set in defines
 * takes in clock speeds in KHZ
 */
void setSampleRate(int clockSpeed) {
    sampleRate = (clockSpeed * 1000) / (MAXFREQ * 2);
}

/* prepare for next sample population */
void resetSampleValues(void) {
    maxSample = 0;      /*reset to find vals over the next sample taken*/
    minSample = 5000;
    numSamples = 0;
    totSample = 0;
    avgSample = 0;
}
