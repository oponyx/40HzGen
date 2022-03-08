#include <Arduino.h>

#include "graphic.h"
#include "globals.h"
#include "settings.h"
#include "hw_config.h"
#include "config.h"
#include "functions.h"

bool bHalfSecond;             // Half a second beat

unsigned long start_millis;   
unsigned long mills_2;
unsigned long lastTrigger;
unsigned long rem_time;       // seconds left to the end of the cycle
uint16_t Command;             // the variable used to dispatch commands


extern deviceStatus_t Status;

#ifdef NEW_BUTTONS
/**
 * @brief buttons callback function
 * 
 * 
 */
void buttonShortPressed(uint8_t btnPin){
  Serial.printf("buttonShortPressed Callback function called. Argument is:%s\n" , String(btnPin).c_str());
  switch(btnPin){
    case OK_BUTTON_PIN:    
      if( Status == deviceStatus_t::STATUS_WORKING ){
        Command = CMD_STOP;
      }else{
        Command = CMD_START;
      }
    break;
    case CANCEL_BUTTON_PIN:   
      if( Status != deviceStatus_t::STATUS_WORKING ){
        unsigned long lst_millis=millis();
        while (millis() < lst_millis + INFO_PAGE_DISPLAY_TIME){
          dispInfoPage();
        }
        dispReadyPage();
      }
    break;
  }
}

void buttonPressed(uint8_t btnPin){
  Serial.printf("buttonPressed Callback function called. Argument is:%s\n" , String(btnPin).c_str());
}
void buttonReleased(uint8_t btnPin){
  Serial.printf("buttonReleased Callback function called. Argument is:%s\n" , String(btnPin).c_str());
}  
void buttonLongPressed(uint8_t btnPin){
  Serial.printf("buttonLongPressed Callback function called. Argument is:%s\n" , String(btnPin).c_str());

}
#endif

void test(){
  u_long n1= micros();
  //hertz2us(80);
  rem_time = (Settings.on_time * 60 - ((millis() - start_millis) / 1000));
  u_long n2 = micros();
  Serial.printf("usec hertz2us(80):%lu\n", n2-n1);
}

void testButtonHandle(){
  u_long n1= micros();
  //hertz2us(80);
  okButton.handle();
  u_long n2 = micros();
  Serial.printf("usec testButtonHandle():%lu\n", n2-n1);
}


uint16_t hertz2ms(uint8_t hertz){
  return (uint16_t)1000/(hertz*2);
}

u_long hertz2us(uint8_t hertz){
  return (u_long)1000000/(hertz*2);
}

/**
 * @brief Start Working
 * 
 */
void start(){
  start_millis = millis();
  Status = deviceStatus_t::STATUS_WORKING;
  Serial.printf("Freq:%u, Bri:%u, PWM freq:%u uS:%lu\n", Settings.light_freq, Settings.brightness, Settings.pwm_freq, hertz2us(Settings.light_freq));
  dispWorkingPage();
  Serial.println("Started!!");
}

/**
 * @brief Stops working 
 * 
 */
void stop(){
  Status = deviceStatus_t::STATUS_IDLE;
  rem_time = 0;
  digitalWrite(LIGHT_OUT_PIN, LOW);
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


#ifndef NEW_BUTTONS
/**
 * @brief Push Button control function
 * 
 */
void handleButtons(void){  

  bool okButtonStatus = digitalRead(OK_BUTTON_PIN);
  bool cancelButtonStatus = digitalRead(CANCEL_BUTTON_PIN);
  bool upButtonStatus = digitalRead(UP_BUTTON_PIN);
  bool downButtonStatus = digitalRead(DOWN_BUTTON_PIN);

  if(okButtonStatus){
    delay(10);
    while (digitalRead(OK_BUTTON_PIN))
    {
      ;// wait button release
    }
    
    if( Status == deviceStatus_t::STATUS_WORKING ){
      Command = CMD_STOP;
    }else{
      Command = CMD_START;
    }
  }

  if(cancelButtonStatus){
    if( Status != deviceStatus_t::STATUS_WORKING ){
      delay(10);
      while (digitalRead(CANCEL_BUTTON_PIN))
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
#endif