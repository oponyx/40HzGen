#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_


extern bool bWorking,bHalfSecond,bReboot,okButtonStatus,cancelButtonStatus,upButtonStatus,downButtonStatus;;
extern unsigned long start_millis, mills_2, lastTrigger,rem_time;
extern uint16_t Command; 

void start();
void stop();
void getMinutesAndSeconds(unsigned long millis, String &minutesSeconds);
void handleCommands();
void handleButtons();

#endif