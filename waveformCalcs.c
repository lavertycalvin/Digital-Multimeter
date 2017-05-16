/* for the calculations necessary to display on Terminal
 *
 */

#include <math.h>

float trueRMS(float acVolt, float dcVolt) {
    return sqrt((acVolt * acVolt) + (dcVolt * dcVolt));
}

float calcRMS(float trueRMS, float dcVolt) {
    return trueRMS - dcVolt;
}

float voltPP(float maxVolt, float minVolt) {
    return maxVolt - minVolt;
}

//make sure that this returns an int
int getDCOffset(int max, int min) {
    return (max + min)/2;
}
