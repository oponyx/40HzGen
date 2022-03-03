#include <Arduino.h>

#include "graphic.h"
#include "settings.h"
#include "hw_config.h"
#include "config.h"


bool bWorking;                // Working flag: On if cycle is active 
bool bHalfSecond;             // Half a second beat
bool okButtonStatus;        
bool cancelButtonStatus;
bool upButtonStatus;
bool downButtonStatus;
unsigned long start_millis;   
unsigned long mills_2;
unsigned long lastTrigger;
unsigned long rem_time;       // seconds left to the end of the cycle
uint16_t Command;             // the variable used to dispatch commands

/**
 * @brief Start Working
 * 
 */
void start(){
  start_millis = millis();
  bWorking = true;
  Serial.println("Started!!");
}

/**
 * @brief Stops working 
 * 
 */
void stop(){
  bWorking = false;
  rem_time = 0;
  digitalWrite(LIGHT_OUT, LOW);
  Serial.println("Stopped!!");
  dispReadyPage();
}


/**
 * @brief Builds a Minutes and Seconds String from Seconds
 * 
 * @param seconds 
 * @param minutesSeconds 
 */
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


/**
 * @brief Execute commands
 * 
 */
void handleCommands(){
  switch(Command){
    case CMD_SAVE_SETTING_AND_REBOOT:
      Serial.println("CMD Save Settings and Reboot detected");
      Command = CMD_NO_COMMANDS;
      delay(1000);
      SettingsWrite();
      ESP.restart();
      break;
    case CMD_REBOOT:
      Serial.println("CMD Reboot detected");
      delay(1000);
      ESP.restart();
      break;
    case CMD_START:
      Serial.println("CMD start detected");
      Serial.printf("Freq:%u, Bri:%u, PWM freq:%u\n", Settings.light_freq, Settings.brightness, Settings.pwm_freq);
      Command = CMD_NO_COMMANDS;
      start();
      break;
    case CMD_STOP:
      Serial.println("CMD stop detected");
      Command = CMD_NO_COMMANDS;
      stop();
      break;
    default:
      return;
      break;
  }
}

/**
 * @brief Push Button control function
 * 
 */
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
