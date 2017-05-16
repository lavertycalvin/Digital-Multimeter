/*
 * terminalControl.h
 *
 *  Created on: May 15, 2017
 *      Author: calvin
 */

#ifndef TERMINALCONTROL_H_
#define TERMINALCONTROL_H_

void printValues(int dc, int ac, int freq);
void moveCursor(int x, int y);
void moveRight(int spaces);
void moveDown(int spaces);
void moveHome(void);
void clearScreen(void);
void print(int ac, int dc, int freq);
void printTitle(void);
void printGraphs(int dc,int ac);
void voltOut(int writeOut);
void freqOut(int writeOut);

#endif /* TERMINALCONTROL_H_ */
