/*
 * adc.c
 *
 *  Created on: May 15, 2017
 *      Author: calvin
 */

#include "msp.h"

static int sample;
static int finishedSampling = 1;
/* ADC14 interrupt service routine */
void ADC14_IRQHandler(void) {
    sample = ADC14->MEM[0]; /* get the sample */
    setSampleReady();
}

void enableADCInt(void) {
    // Enable ADC interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);
}

void setupADC(void) {
    // Sampling time, S&H=16, ADC14 on
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    ADC14->CTL1 = ADC14_CTL1_RES_3;         // Use sampling timer, 14-bit conversion results

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1;   // A1 ADC input select; Vref=AVCC
    ADC14->IER0 |= ADC14_IER0_IE0;          // Enable ADC conv complete interrupt
}

void setSampleReady(void) {
    finishedSampling = 1;
}

void clearSampleReady(void) {
    finishedSampling = 0;
}

int getSample(void) {
    return sample;
}

int getReady(void) {
    return finishedSampling;
}


