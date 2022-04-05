/*
main.cpp - 40HzGen main file

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


/*
python c:/Users/op/.platformio/packages/espota/espota.py -i 192.168.1.174  -f .pio/build/esp01_1m/firmware.bin  

*/
#include <Arduino.h>
#include <stdint.h>

//#include <ArduinoOTA.h>

#include "hw_config.h"
#include "config_override.h"
#include "globals.h"
#include "config.h"
#include "log.h"
#include "settings.h"
#include "wifi.h"
#include "web_serv.h"
#include "functions.h"
#include "error_codes.h"
#include "button.hpp"
#include "graphic.h"



bool bDisplayUpdated;         // 'Display is updated' flag
bool bLastLightStatus;
u_long lastLigtSwitch;
bool bButtonChanged=false;
deviceStatus_t Status;

Button okButton(OK_BUTTON_PIN, INPUT_PULLUP, OK_BUTT_ACTIVE_LVL);
#ifdef CANCEL_BUTTON_PIN
Button cancelButton(CANCEL_BUTTON_PIN, INPUT_PULLUP, CANCEL_BUTT_ACTIVE_LVL);
#endif
#ifdef UP_BUTTON_PIN
Button upButton(UP_BUTTON_PIN, INPUT_PULLUP, UP_BUTT_ACTIVE_LVL);
#endif
#ifdef DOWN_BUTTON_PIN
Button downButton(DOWN_BUTTON_PIN, INPUT_PULLUP, DOWN_BUTT_ACTIVE_LVL);
#endif


void IRAM_ATTR buttonChangedISR(){
 bButtonChanged = true;
}

/**
 * @brief Main setup function
 * 
 */
void setup() {
  Status = STATUS_IDLE;
  bHalfSecond=false;

  Serial.begin(115200);
  // load settings
  m_log(false, "\n");
  m_log(true, "Serial ok\n");
  
  // restore default settings if okButton pressed at powerup
  m_log(true, "Checking if button pressed for reset to default settings\n");
  delay(1000);
  if(digitalRead(OK_BUTTON_PIN) == OK_BUTT_ACTIVE_LVL){
    m_log(false, "Restoring default settings...");
    restoreDefaultSettings();
  }

  m_log(true, "Loading Settings..");
  uint8_t result = SettingsRead();
  if( result != 0 ){
    switch ( result )
    {
    case ERROR_SETTINGS_CRC:
        m_log(false, "\nError: Bad CRC on settings datas\nResoring default settings.\n");
        restoreDefaultSettings();
      break;
    
    default:
      break;
    }
  }else{
    m_log(false, "Ok\n");
  }

#ifdef LCD_POPULATED
  m_log(true, "Setting up Display...");
  if(display_init()){
    m_log(false, "FAIL!\n");
  }
  m_log(false, "Ok.\n");

  dispLogoPage();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner

  
  display.printf("Setting up WIFI...");
#endif
  m_log(true, "Setting up WIFI...");
  if(!wifi_setup()){
    m_log(false, "WIFI connection KO!\n");   
  }
  else{
    m_log(false, "OK!\n");
    m_log(true, "WIFI connection Successful\n");
    m_log(true, "WIFI IP Address is:%s\n", WiFi.localIP().toString().c_str() );
  }
  m_log(true, "Setting up AP...");
  if(!AP_setup()){  
    m_log(false, "KO!!");
  }
  else{
    m_log(false, "OK!\n");
    m_log(true, "AP IP address = %s\n" ,  WiFi.softAPIP().toString().c_str());
#ifdef LCD_POPULATED
    display.println(WiFi.softAPIP());
#endif
  }


  m_log(true, "Starting web server...");
  if (web_serv_setup() == NO_ERRORS) {
    m_log(false, "OK!\n");
  }else{
    m_log(false, "<<<<<<<<<<<<< ERROR!");
  }

  setupIO();
  digitalWrite(LIGHT_OUT_PIN,LIGHT_OUT_ACTIVE_LVL); // test output
#ifdef LCD_POPULATED
  dispInfoPage();
#endif
  delay(1000);
#ifdef LCD_POPULATED
  dispReadyPage();
#endif
  digitalWrite(LIGHT_OUT_PIN,!LIGHT_OUT_ACTIVE_LVL); // test output end
  m_log(true, "WIFI SSID:%s\n", Settings.wifi_ssid);
  m_log(true, "WIFI PWD:%s\n", Settings.wifi_psw);
  m_log(true, "AP SSID:%s\n", Settings.ap_ssid);
  m_log(true, "AP PWD:%s\n", Settings.ap_psw);
  m_log(true, "Light Freq:%u\n", Settings.light_freq);
  m_log(true, "On time:%u\n", Settings.on_time);
  m_log(true, "Brightness:%u\n", Settings.brightness);
  m_log(true, "PWM Freq:%u\n", Settings.pwm_freq);
  m_log(true, "Autostart:%s\n", (Settings.settingFlags.autostart == true ) ? "ON" : "OFF");

  attachInterrupt(digitalPinToInterrupt(OK_BUTTON_PIN), buttonChangedISR, CHANGE);

  if(Settings.settingFlags.autostart){
    Command = CMD_START;
  }
  
  /// tests
  //check_flash();
  //m_log(true, "Size of indexp:%d\n", sizeof(index_page));
  //test();
  //testButtonHandle();
}

/**
 * @brief Main Loop
 * 
 */
void loop() {
  if (bButtonChanged) {
    //bButtonChanged = false;
    okButton.handle();// only button handling always checked, other buttons only when not working
  }

  handleCommands();
  if((millis()-mills_2)>499){
    mills_2=millis();
    bHalfSecond=!bHalfSecond;
  }

  //flash status led
  #ifdef STATUS_LED
  digitalWrite(STATUS_LED, (bHalfSecond == true) ? STATUS_LED_ATIVE_LVL : !STATUS_LED_ATIVE_LVL );   
  #endif

  if( Status == deviceStatus_t::STATUS_WORKING ){
    // light switch
    u_long now = micros();
    u_long diff = now - lastLigtSwitch;
    
    if( diff >= semiPeriod ){
      (bLastLightStatus == false) ? analogWrite(LIGHT_OUT_PIN, (LIGHT_OUT_ACTIVE_LVL)  ? (int)(Settings.brightness) : 100 -(int)(Settings.brightness)) : digitalWrite(LIGHT_OUT_PIN, !LIGHT_OUT_ACTIVE_LVL);
      lightFreqErrorUs = diff - semiPeriod;  // calculate error
      if(lightFreqErrorUs > maxLightFreqErrorUs ) maxLightFreqErrorUs = lightFreqErrorUs;
      bLastLightStatus = !bLastLightStatus;
      lastLigtSwitch = now ; 
    }

    // check for working timeout
    rem_time = (Settings.on_time * 60 - ((millis() - start_millis) / 1000)); // 14 uS
    if(rem_time<=0){
      m_log(true, "Timed Out\n");
      stop();
    }  
  }
  else{ // not working
      okButton.handle();
#ifdef CANCEL_BUTTON_PIN      
      cancelButton.handle();
#endif      
#ifdef UP_BUTTON_PIN      
      upButton.handle();
#endif      
#ifdef DOWN_BUTTON_PIN      
      downButton.handle();
#endif      

      //ArduinoOTA.handle();
  }
  //display.display();
  
}

