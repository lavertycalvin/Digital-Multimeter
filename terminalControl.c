#include "msp.h"

/* Location Defs for printing*/
#define TITLEX      30
#define TITLEY      0
#define DCVOLTX     8
#define DCVOLTY     5
#define ACVOLTX     50
#define ACVOLTY     5
#define DCGRAPHX    0
#define DCGRAPHY    10
#define ACGRAPHX    40
#define ACGRAPHY    10
#define FREQX       50
#define FREQY       6

#define GRAPHLENGTH 20

#define DCVOLTSAMPLEX DCVOLTX + 12
#define DCVOLTSAMPLEY DCVOLTY
#define ACVOLTSAMPLEX ACVOLTX + 12
#define ACVOLTSAMPLEY ACVOLTY


/* UART Output defs */
#define ESC         0x1B    /* macro for the esc key */
#define NUMCHARS    5      /* Precision of UART output */
#define MAKEASCII   0x30  /* add to integer to make readable char */




/* takes in position to move cursor and writes out esc code to terminal */
void moveCursor(char x, char y) {
    moveHome();
    moveRight(x);
    moveDown(y);
}

/* spaces needs to be 0-99 */
void moveRight(char spaces) {
    int length = 2, multiplier = 10, singleNum = 0;
    char sequence[9] = { ESC, '['};

    while(multiplier > 0) {
        singleNum = spaces / multiplier;
        sequence[length++] = singleNum + MAKEASCII;
        spaces -= singleNum * multiplier;
        multiplier /= 10;
    }
    sequence[length++] = 'C';
    sendUART(sequence, length);
}

/* spaces needs to be 0-99 */
void moveDown(int spaces) {
    int length = 2, multiplier = 10, singleNum = 0;
    char sequence[9] = { ESC, '['};

    while(multiplier > 0) {
        singleNum = spaces / multiplier;
        sequence[length++] = singleNum + MAKEASCII;
        spaces -= singleNum * multiplier;
        multiplier /= 10;
    }
    sequence[length++] = 'B';
    sendUART(sequence, length);
}
void moveHome(void) {
    char sequence[4] = { ESC, '[', 'H'};
    sendUART(sequence, 3);
}

void clearScreen(void){
    char sequence[4] = {ESC, '[', '2', 'J'};
    sendUART(sequence, 4);
}

/* set up interface for the terminal */
void initTerminal(int ac, int dc, int freq) {
    clearScreen();
    printTitle();
    printDCVolt(dc);
    printACVolt(ac);
    printDCGraph(dc);
    printACGraph(ac);
    printFreq(freq);
    //printGraphs(dc,ac);
}

void printTitle(void){
    moveCursor(TITLEX, TITLEY);
    sendUART("~~~~ NOT BORING DMM ~~~~ ", 25);
}

void printDCVolt(int dc) {
    moveCursor(DCVOLTX, DCVOLTY);
    sendUART("DC Voltage: ", 12);
    voltOut(dc);
}

void printACVolt(int ac) {
    moveCursor(ACVOLTX, ACVOLTY);
    sendUART("AC Voltage: ", 12);
    voltOut(ac);
}

void printDCGraph(int dc) {
    int count;
    int DC_bar = (int)((dc /3300.0) * GRAPHLENGTH);
    moveCursor(DCGRAPHX, DCGRAPHY);
    sendUART(" |", 2);
    for(count = 0;count < DC_bar; count++){
       sendUART("#", 1);
    }
    for(count = DC_bar; count < GRAPHLENGTH; count++) {
        sendUART(" ", 1);
    }
    sendUART("|" , 1);
    moveCursor(DCGRAPHX, DCGRAPHY + 1);
    sendUART("0.0", 3);
    for(count = 0; count < GRAPHLENGTH-2; count++) {
        sendUART(" ", 1);
    }
    sendUART("3.3", 3);
}

void printACGraph(int ac) {
    int count;
    int AC_bar = (int)((ac /3300.0) * GRAPHLENGTH);
    moveCursor(ACGRAPHX, ACGRAPHY);
    sendUART(" |", 2);
    for(count = 0;count < AC_bar; count++){
       sendUART("#", 1);
    }
    for(count = AC_bar; count < GRAPHLENGTH; count++) {
        sendUART(" ", 1);
    }
    sendUART("|" , 1);
    moveCursor(ACGRAPHX, ACGRAPHY + 1);
    sendUART("0.0", 3);
    for(count = 0; count < GRAPHLENGTH-2; count++) {
        sendUART(" ", 1);
    }
    sendUART("3.3", 3);
}

void printFreq(int freq) {
    moveCursor(FREQX, FREQY);
    sendUART("Frequency :  ", 13);
    freqOut(freq);
}

void voltOut(int writeOut) {
    char volt[5] = {};
    int multiplier = 1000; /*controls writeOut 8*/
    int j = 0, singleNum = 0;
    for (j = 0; j < NUMCHARS + 1; j++) { /* +1 for the decimal point */
        if(j == 1) {
            volt[j] = '.';           /* send decimal to TX Buffer */
        }
        else {
            singleNum = (int)(writeOut /multiplier);
            volt[j] = singleNum + MAKEASCII;           /* send number to TX Buffer */
            writeOut -= (int)singleNum * multiplier; /* take off last wrote out value */
            multiplier /= 10; /*shift over by 10 */
        }
    }
    sendUART(volt, 5);
}

void freqOut(int writeOut) {
    char freq[4] = {};
    int multiplier = 100; /*controls writeOut */
    int j = 0, singleNum = 0;
    for (j = 0; j < 3; j++) { /* +1 for the decimal point */
            singleNum = (int)(writeOut /multiplier);
            freq[j] = singleNum + MAKEASCII;           /* send number to TX Buffer */
            writeOut -= (int)singleNum * multiplier; /* take off last wrote out value */
            multiplier /= 10; /*shift over by 10 */
    }
    sendUART(freq, 4);
}

void updateTerminal(int dcVolt, int acVPP, int acRMSVolt, int acTRUERMSVolt, int freq) {
    printDCVolt(dcVolt);
    printACVolt(acVPP);
    printDCGraph(dcVolt);
    printACGraph(acVPP);
    printFreq(freq);
    //printGraphs(dc,ac);
}
