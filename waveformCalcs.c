/* for the calculations necessary to display on Terminal
 *
 */


#include <math.h>

#define MAXVOLT     3.3
#define MAXFREQ     1000
#define MINVPP      500 /*mV*/
#define NUMSAMPLES  2000

int trueRMS(int acVolt, int dcVolt) {
    return (int)sqrt((acVolt * acVolt) + (dcVolt * dcVolt));
}

int calcRMS(int trueRMS, int dcVolt) {
    return trueRMS - dcVolt;
}

float voltPP(float maxVolt, float minVolt) {
    return maxVolt - minVolt;
}

//make sure that this returns an int
int getDCOffset(int max, int min) {
    return (max + min)/2;
}

/* if Vpp < 500 mV, the waveform is DC */
int isDC(int maxVal, int minVal) {
    if(maxVal - minVal < 500) {
        return 1;
    }
    return 0;
}

int getDCAverage(int total) {
    return (int)total/NUMSAMPLES;
}

int getACVPP(int maxSample, int minSample) {
    int acVPP = maxSample - minSample;
    if(acVPP < MINVPP) { /* isn't AC Waveform */
        acVPP = 0;
    }
    return acVPP;

}

/* checks dc Crossings to find half the period
 * if VPP is 0, signal is DC
 * sampleRate will determine the freq as well
 */
int getFreq(int dcVal, int *samples, int VPP, int sampleRate) {
    int i = 1;
    double halfPeriod = 1.0/sampleRate;
    int crossingsFound = 0;
    int first = 0, second = 0;
    int lastVal = samples[0];
    if(VPP == 0) {
        return 0;
    }
    for(i = 1; i < NUMSAMPLES; i++){
        if(lastVal > dcVal && samples[i] < dcVal) {
            crossingsFound++;
            first = i;
        }
        else if(lastVal < dcVal && samples[i] > dcVal) {
            crossingsFound++;
            second = i;
        }

        /* check to see if we found both of them */
        if(crossingsFound == 2) {
            break;
        }
        lastVal = samples[i]; /* update for next comparison */
    }
    halfPeriod = halfPeriod * abs(first-second);
    return (int)(1/(2 * halfPeriod));
}
