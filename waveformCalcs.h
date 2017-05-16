/*
 * waveformCalcs.h
 *
 *  Created on: May 15, 2017
 *      Author: calvin
 */

#ifndef WAVEFORMCALCS_H_
#define WAVEFORMCALCS_H_

float trueRMS(float acVolt, float dcVolt);
float calcRMS(float trueRMS, float dcVolt);
float voltPP(float maxVolt, float minVolt);
int   getDCOffset(int max, int min);

#endif /* WAVEFORMCALCS_H_ */
