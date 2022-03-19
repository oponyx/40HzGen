/*
functions.cpp - generic functions


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
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "hw_config.h"
#include "globals.h"
#include "settings.h"
#include "config.h"
#include "functions.h"
#include "web_serv.h"
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


/**
 * @brief OTA setup function
 * 
 */
void setupOTA(){
  // per aggiornamenti OTA
  ArduinoOTA.onStart([]() {  
    String type;  
    if (ArduinoOTA.getCommand() == U_FLASH) {  
      type = "sketch";  
    } else { // U_SPIFFS  
      type = "filesystem";  
    }  
    
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()  
    Serial.println("ArduinoOTA: Start updating " + type);  
  });  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nArduinoOTA: Update End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("ArduinoOTA: Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("ArduinoOTA: Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

/**
 * @brief Setup Input and Output pins
 * 
 * 
 * 
 */
void setupIO(){
  pinMode(LED_BUILTIN, OUTPUT);
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
  Serial.printf("buttonShortPressed Callback function called. Argument is:%s\n" , String(btnPin).c_str());
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
  Serial.printf("buttonLongPressed Callback function called. Argument is:%s\n" , String(btnPin).c_str());
  bButtonChanged = false;
}

void buttonPressed(uint8_t btnPin){
  //Serial.printf("buttonPressed Callback function called. Argument is:%s\n" , String(btnPin).c_str());
}

void buttonReleased(uint8_t btnPin){
  //Serial.printf("buttonReleased Callback function called. Argument is:%s\n" , String(btnPin).c_str());
}  

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
  maxLightFreqErrorUs = 0;
  semiPeriod = hertz2us(Settings.light_freq);
  start_millis = millis();
  Status = deviceStatus_t::STATUS_WORKING;
  Serial.printf("Freq:%u, Bri:%u, PWM freq:%u uS:%lu\n", Settings.light_freq, Settings.brightness, Settings.pwm_freq, semiPeriod);
#ifdef LCD_POPULATED
  dispWorkingPage();
#endif
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
        Serial.println("Settings changed. Rebooting...");
        webRequest -> send_P(200, "text/html", rebooting_page);
        Command = CMD_SAVE_SETTING_AND_REBOOT;
      }else{
          Serial.println("Settings not changed. Redirecting to index page...");
          webRequest->send_P(200, "text/html", index_page); 
      }
      break;
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

