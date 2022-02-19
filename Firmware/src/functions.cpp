#include <Arduino.h>
#include "graphic.h"
#include "settings.h"
#include "hw_config.h"
#include "config.h"


bool bWorking,bHalfSecond,okButtonStatus,cancelButtonStatus,upButtonStatus,downButtonStatus;
unsigned long start_millis, mills_2, lastTrigger,rem_time;
uint16_t Command; 

void start(){
  start_millis = millis();
  bWorking = true;
  Serial.println("Started!!");
}

void stop(){
  bWorking = false;
  rem_time = 0;
  digitalWrite(LIGHT_OUT, LOW);
  Serial.println("Stopped!!");
  dispReadyPage();
}

void getMinutesAndSeconds(unsigned long seconds, String &minutesSeconds) {
  unsigned long minutes;
  
  minutes = seconds / 60;
  seconds %= 60;

  if (minutes < 10) {
    minutesSeconds += "0";
  }
  minutesSeconds += String(minutes) + ":";

  if (seconds < 10) {
    minutesSeconds += "0";
  }
  minutesSeconds += String(seconds);
}


void handleCommands(){
  switch(Command){
    case CMD_REBOOT:
      Serial.println("CMD Reboot detected");
      delay(1000);
      ESP.restart();
      break;
    case CMD_START:
      Serial.println("CMD start detected");
      Command = NO_COMMANDS;
      start();
      break;
    case CMD_STOP:
      Serial.println("CMD stop detected");
      Command = NO_COMMANDS;
      stop();
      break;
    default:
      return;
      break;
  }
}

void handleButtons(void){
  okButtonStatus = digitalRead(OK_BUTTON);
  cancelButtonStatus = digitalRead(CANCEL_BUTTON);
  upButtonStatus = digitalRead(UP_BUTTON);
  downButtonStatus = digitalRead(DOWN_BUTTON);

  if(okButtonStatus){
    delay(10);
    while (digitalRead(OK_BUTTON))
    {
      ;// wait button release
    }
    
    if(bWorking){
      Command = CMD_STOP;
    }else{
      Command = CMD_START;
    }
  }

  if(cancelButtonStatus){
    if(!bWorking){
      delay(10);
      while (digitalRead(CANCEL_BUTTON))
      {
        ;// wait button release
      }    
      unsigned long lst_millis=millis();
      while (millis() < lst_millis + INFO_PAGE_DISPLAY_TIME){
        dispInfoPage();
      }
      dispReadyPage();
    }
  }
}
