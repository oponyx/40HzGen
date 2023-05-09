/*
functions.cpp - generic functions

MIT License

Copyright (c) 2022 Pagliarulo Onofrio (oponyx@hotmail.it)

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
#include <Arduino.h>
//#include <ArduinoOTA.h>
#include "hw_config.h"
#include "globals.h"
#include "settings.h"
#include "config.h"
#include "functions.h"
#include "web_serv.h"
#include "log.h"


#ifdef LCD_POPULATED
#include "graphic.h"
#endif

bool bHalfSecond;             // Half a second beat

u_long start_millis;   
u_long mills_2;
u_long semiPeriod;
u_long lightFreqErrorUs;
u_long maxLightFreqErrorUs;
u_long lastTrigger;
u_long rem_time;       // seconds left to the end of the cycle
uint16_t Command;             // the variable used to dispatch commands


extern deviceStatus_t Status;


int indexvReal = 0;
/*
void check_flash(){
  // taken from ESP8266 CheckFlashConfig by Markus Sattler

  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

  D_PRINTLN(true, "Flash real id:   %08X", ESP.getFlashChipId());
  D_PRINTLN(true, "Flash real size: %u bytes", realSize);

  D_PRINTLN(true, "Flash ide  size: %u bytes", ideSize);
  D_PRINTLN(true, "Flash ide speed: %u Hz", ESP.getFlashChipSpeed());
  D_PRINTLN(true, "Flash ide mode:  %s", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT"
                                                                    : ideMode == FM_DIO  ? "DIO"
                                                                    : ideMode == FM_DOUT ? "DOUT"
                                                                                         : "UNKNOWN"));

  if (ideSize != realSize) {
    D_PRINTLN(true, "Flash Chip configuration wrong!");
  } else {
    D_PRINTLN(true, "Flash Chip configuration ok.");
  }

}
*/

/**
 * @brief Setup Input and Output pins
 * 
 * 
 * 
 */
void setupIO(){
#ifdef STATUS_LED
  pinMode(STATUS_LED, OUTPUT);
#endif
  pinMode(LIGHT_OUT_PIN, OUTPUT);
#ifdef AUDIO_OUT_PIN
  pinMode(AUDIO_OUT_PIN, OUTPUT);
#endif


  okButton.onButtonShortPressed(buttonShortPressed);
  okButton.onButtonLongPressed(buttonLongPressed);
  okButton.onButtonPressed(buttonPressed);
  okButton.onButtonReleased(buttonReleased);

#ifdef CANCEL_BUTTON_PIN
  cancelButton.onButtonShortPressed(buttonShortPressed);
  cancelButton.onButtonLongPressed(buttonLongPressed);
#endif

#ifdef UP_BUTTON_PIN
  upButton.onButtonShortPressed(buttonShortPressed);
  upButton.onButtonLongPressed(buttonLongPressed);
#endif

#ifdef DOWN_BUTTON_PIN
  downButton.onButtonShortPressed(buttonShortPressed);
  downButton.onButtonLongPressed(buttonLongPressed);
#endif

  okButton.begin();
#ifdef CANCEL_BUTTON_PIN
  cancelButton.begin();
#endif
#ifdef UP_BUTTON_PIN
  upButton.begin();
#endif
#ifdef DOWN_BUTTON_PIN
  downButton.begin();
#endif

  lightPWMSetup();
  audioPWMSetup();
}


void lightPWMSetup(){
#ifdef ESP32
 ledcSetup(LIGHT_PWM_CH, Settings.pwm_freq, LIGHT_PWM_RES_BITS);
 ledcAttachPin(LIGHT_OUT_PIN, LIGHT_PWM_CH);
#else
  analogWriteFreq(Settings.pwm_freq);
  analogWriteRange(MAX_BRIGHTNESS);
#endif
}

void audioPWMSetup(){
#ifdef ESP32
 ledcSetup(AUDIO_PWM_CH, 5000, AUDIO_PWM_RES_BITS);
 ledcAttachPin(AUDIO_OUT_PIN, AUDIO_PWM_CH);
#endif
}

extern bool bButtonChanged;

/**
 * @brief buttons callback function
 * 
 * 
 */
void buttonShortPressed(uint8_t btnPin){
#ifdef __DEBUG_BUTTONS__  
  D_PRINTLN(true, "buttonShortPressed Callback function called. Argument is:%s" , String(btnPin).c_str());
#endif
  switch(btnPin){
    case OK_BUTTON_PIN:  
      switch (Status){
        case deviceStatus_t::STATUS_WORKING:
          Command = CMD_STOP;
        break;
        case deviceStatus_t::STATUS_IDLE:
          Command = CMD_START;
        break;
        case deviceStatus_t::STATUS_MENU:
#ifdef LCD_POPULATED
          menuOkPressed();
#endif
        break;
      }  
    break;
#ifdef CANCEL_BUTTON_PIN
    case CANCEL_BUTTON_PIN:   
      if( Status != deviceStatus_t::STATUS_WORKING ){
#ifdef LCD_POPULATED
        if(Status == deviceStatus_t::STATUS_MENU){ // menu is active
          menuCancelPressed();
        }
        else{
          dispInfoPage();
          unsigned long lst_millis=millis();
          while (millis() < lst_millis + INFO_PAGE_DISPLAY_TIME){
            ;    
          }
        }
        dispReadyPage();
#endif
      }
    break;
#endif    
  }
  bButtonChanged = false;
}

void buttonLongPressed(uint8_t btnPin){
#ifdef __DEBUG_BUTTONS__  
  D_PRINTLN(true, "buttonLongPressed Callback function called. Argument is:%s" , String(btnPin).c_str());
#endif
  switch(btnPin){
    case CANCEL_BUTTON_PIN:
      if(Status == deviceStatus_t::STATUS_IDLE){
        Status = deviceStatus_t::STATUS_MENU;
      }
    break;
  }
  bButtonChanged = false;
}

void buttonPressed(uint8_t btnPin){
  //D_PRINTLN(true, "buttonPressed Callback function called. Argument is:%s" , String(btnPin).c_str());
}

void buttonReleased(uint8_t btnPin){
  //D_PRINTLN(true, "buttonReleased Callback function called. Argument is:%s" , String(btnPin).c_str());
}  

void test(){
  u_long n1= micros();
  //hertz2us(80);
  rem_time = (Settings.on_time * 60 - ((millis() - start_millis) / 1000));
  u_long n2 = micros();
  D_PRINTLN(true, "usec hertz2us(80):%lu", n2-n1);
}

void testButtonHandle(){
  u_long n1= micros();
  //hertz2us(80);
  okButton.handle();
  u_long n2 = micros();
  D_PRINTLN(true, "usec testButtonHandle():%lu", n2-n1);
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
  maxLightFreqErrorUs = 0;
  semiPeriod = hertz2us(Settings.light_freq);
  start_millis = millis();
  Status = deviceStatus_t::STATUS_WORKING;
  D_PRINTLN(true, "Freq:%u, Bri:%u, PWM freq:%u uS:%lu", Settings.light_freq, Settings.brightness, Settings.pwm_freq, semiPeriod);
#ifdef LCD_POPULATED
  dispWorkingPage();
#endif
#ifdef ESP32
  CalculateLightPWMDutyVal();
  if(Settings.settingFlags.audioEnabled){
    ledcSetup(AUDIO_PWM_CH, AUDIO_FREQ, AUDIO_PWM_RES_BITS);
    ledcAttachPin(AUDIO_OUT_PIN, AUDIO_PWM_CH);
  }
#endif
  D_PRINTLN(true, "Started!!");
}

/**
 * @brief Stops working 
 * 
 */
void stop(){
  Status = deviceStatus_t::STATUS_IDLE;
  rem_time = 0;
#ifdef ESP32
  ledcWrite(LIGHT_PWM_CH, (LIGHT_OUT_ACTIVE_LVL)  ? 0 : pow(2,LIGHT_PWM_RES_BITS)-1);
#ifdef AUDIO_OUT_PIN
  ledcWrite(AUDIO_PWM_CH, (AUDIO_OUT_ACTIVE_LVL)  ? 0 : pow(2,AUDIO_PWM_RES_BITS)-1);
#endif  
#else
  digitalWrite(LIGHT_OUT_PIN, !LIGHT_OUT_ACTIVE_LVL);
#ifdef AUDIO_OUT_PIN
  noTone(AUDIO_OUT_PIN);
#endif
#endif
  D_PRINTLN(true, "Stopped!!");
#ifdef LCD_POPULATED
  dispReadyPage();
#endif
}

void CalculateLightPWMDutyVal(){
#ifdef ESP32  
  LightPWMDutyVal = (LIGHT_OUT_ACTIVE_LVL == true) ? (uint16_t)(Settings.brightness * (pow(2,LIGHT_PWM_RES_BITS) -1) / 100) : (uint16_t)(100 - (Settings.brightness * (pow(2,LIGHT_PWM_RES_BITS) -1) / 100));
  D_PRINTLN(true, "LightPWMDutyVal:%u", LightPWMDutyVal);
#endif
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
    case CMD_GET_SETTINGS_REQ_PARAMS:
      Command = CMD_NO_COMMANDS;
      if(getSettingsRequestParam()){
        D_PRINTLN(true, "Settings changed. Rebooting...");
        sendRebootingPage();
        //webRequest -> send_P(200, "text/html", rebooting_html);
        Command = CMD_SAVE_SETTING_AND_REBOOT;
      }else{
          D_PRINTLN(true, "Settings not changed. Redirecting to index page...");
          //webRequest->send_P(200, "text/html", index_html);
          sendIndexPage();
      }
      break;
    case CMD_SAVE_SETTING_AND_REBOOT:
      D_PRINTLN(true, "CMD Save Settings and Reboot detected");
      Command = CMD_NO_COMMANDS;
      delay(1000);
      SettingsWrite();
      ESP.restart();
      break;
    case CMD_REBOOT:
      D_PRINTLN(true, "CMD Reboot detected");
      delay(1000);
      ESP.restart();
      break;
    case CMD_START:
      D_PRINTLN(true, "CMD start detected");
      Command = CMD_NO_COMMANDS;
      start();
      break;
    case CMD_STOP:
      D_PRINTLN(true, "CMD stop detected");
      Command = CMD_NO_COMMANDS;
      stop();
      break;
    default:
      return;
      break;
  }
}

