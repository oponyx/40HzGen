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

void check_flash(){
  // taken from ESP8266 CheckFlashConfig by Markus Sattler

  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

  m_log(true, "Flash real id:   %08X\n", ESP.getFlashChipId());
  m_log(true, "Flash real size: %u bytes\n\n", realSize);

  m_log(true, "Flash ide  size: %u bytes\n", ideSize);
  m_log(true, "Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
  m_log(true, "Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT"
                                                                    : ideMode == FM_DIO  ? "DIO"
                                                                    : ideMode == FM_DOUT ? "DOUT"
                                                                                         : "UNKNOWN"));

  if (ideSize != realSize) {
    m_log(true, "Flash Chip configuration wrong!\n");
  } else {
    m_log(true, "Flash Chip configuration ok.\n");
  }

}


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


  analogWriteFreq(Settings.pwm_freq);
  analogWriteRange(MAX_BRIGHTNESS);
}



extern bool bButtonChanged;

/**
 * @brief buttons callback function
 * 
 * 
 */
void buttonShortPressed(uint8_t btnPin){
#ifdef __DEBUG_BUTTONS__  
  m_log(true, "buttonShortPressed Callback function called. Argument is:%s\n" , String(btnPin).c_str());
#endif
  switch(btnPin){
    case OK_BUTTON_PIN:    
      if( Status == deviceStatus_t::STATUS_WORKING ){
        Command = CMD_STOP;
      }else{
        Command = CMD_START;
      }
    break;
#ifdef CANCEL_BUTTON_PIN
    case CANCEL_BUTTON_PIN:   
      if( Status != deviceStatus_t::STATUS_WORKING ){
        unsigned long lst_millis=millis();
#ifdef LCD_POPULATED
        while (millis() < lst_millis + INFO_PAGE_DISPLAY_TIME){
          dispInfoPage();
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
  m_log(true, "buttonLongPressed Callback function called. Argument is:%s\n" , String(btnPin).c_str());
#endif  
  bButtonChanged = false;
}

void buttonPressed(uint8_t btnPin){
  //m_log(true, "buttonPressed Callback function called. Argument is:%s\n" , String(btnPin).c_str());
}

void buttonReleased(uint8_t btnPin){
  //m_log(true, "buttonReleased Callback function called. Argument is:%s\n" , String(btnPin).c_str());
}  

void test(){
  u_long n1= micros();
  //hertz2us(80);
  rem_time = (Settings.on_time * 60 - ((millis() - start_millis) / 1000));
  u_long n2 = micros();
  m_log(true, "usec hertz2us(80):%lu\n", n2-n1);
}

void testButtonHandle(){
  u_long n1= micros();
  //hertz2us(80);
  okButton.handle();
  u_long n2 = micros();
  m_log(true, "usec testButtonHandle():%lu\n", n2-n1);
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
  m_log(true, "Freq:%u, Bri:%u, PWM freq:%u uS:%lu\n", Settings.light_freq, Settings.brightness, Settings.pwm_freq, semiPeriod);
#ifdef LCD_POPULATED
  dispWorkingPage();
#endif
  m_log(true, "Started!!\n");
}

/**
 * @brief Stops working 
 * 
 */
void stop(){
  Status = deviceStatus_t::STATUS_IDLE;
  rem_time = 0;
  digitalWrite(LIGHT_OUT_PIN, !LIGHT_OUT_ACTIVE_LVL);
  m_log(true, "Stopped!!\n");
#ifdef LCD_POPULATED
  dispReadyPage();
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
      if(getSettingsRequestParam(webRequest)){
        m_log(true, "Settings changed. Rebooting...\n");
        sendRebootingPage(webRequest);
        //webRequest -> send_P(200, "text/html", rebooting_html);
        Command = CMD_SAVE_SETTING_AND_REBOOT;
      }else{
          m_log(true, "Settings not changed. Redirecting to index page...\n");
          //webRequest->send_P(200, "text/html", index_html);
          sendIndexPage(webRequest);
      }
      break;
    case CMD_SAVE_SETTING_AND_REBOOT:
      m_log(true, "CMD Save Settings and Reboot detected\n");
      Command = CMD_NO_COMMANDS;
      delay(1000);
      SettingsWrite();
      ESP.restart();
      break;
    case CMD_REBOOT:
      m_log(true, "CMD Reboot detected\n");
      delay(1000);
      ESP.restart();
      break;
    case CMD_START:
      m_log(true, "CMD start detected\n");
      Command = CMD_NO_COMMANDS;
      start();
      break;
    case CMD_STOP:
      m_log(true, "CMD stop detected\n");
      Command = CMD_NO_COMMANDS;
      stop();
      break;
    default:
      return;
      break;
  }
}

