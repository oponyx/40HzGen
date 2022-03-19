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