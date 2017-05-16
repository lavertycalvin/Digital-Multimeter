/*
 * adc.h
 *
 *  Created on: May 15, 2017
 *      Author: calvin
 */

#ifndef ADC_H_
#define ADC_H_



#define SCALE       1/16.384    /* 14-bit ADC MAX   */
#define CALIBR      6           /* Adjust for output voltages*/


void ADC14_IRQHandler(void);
void enableADCInt(void);
void setupADC(void);
void setSampleReady(void);
int  getSample(void);
void clearSampleReady(void);


#endif /* ADC_H_ */
