/*
functions.h


copyright Copyright (c) 2022 Onofrio Pagliarulo (oponyx@hotmail.it)

MIT License

Copyright (c) 2022 Pagliarulo Onofrio

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "web_serv.h"
#include "button.hpp"


extern bool bHalfSecond,bReboot;
extern u_long start_millis, mills_2, lastTrigger,rem_time, semiPeriod, lightFreqErrorUs,maxLightFreqErrorUs;
extern uint16_t Command; 
extern Button okButton;
extern Button cancelButton;
extern Button upButton;
extern Button downButton;

void start();
void stop();
void getMinutesAndSeconds(unsigned long millis, String &minutesSeconds);
void handleCommands();
uint16_t hertz2ms(uint8_t hertz);
u_long hertz2us(uint8_t hertz);
void buttonShortPressed(uint8_t btnPin);
void buttonLongPressed(uint8_t btnPin);
void buttonPressed(uint8_t btnPin);
void buttonReleased(uint8_t btnPin);
void test();
void testButtonHandle();
void setupIO();
void setupOTA();
#endif