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

#include <Arduino.h>

#include "hw_config.h"
#include "config_override.h"
#include "globals.h"
#include "config.h"
#include "log.h"
#include "settings.h"
#include "wifi_func.h"
#include "web_serv.h"
#include "functions.h"
#include "error_codes.h"
#include "button.hpp"
#include "graphic.h"
#include "version.h"



bool bDisplayUpdated;         // 'Display is updated' flag
bool bOutputStatus;
u_long lastSwitch;
bool bButtonChanged=false;
uint16_t LightPWMDutyVal = 0;
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

#ifdef ESP32
  Serial.begin(115200);
#else
  Serial.begin(74880);
#endif
  // load settings
  D_PRINT(false, "\n");
  D_PRINTLN(true, "Serial ok");
  
  // restore default settings if okButton pressed at powerup
  D_PRINT(true, "Checking if button pressed for reset to default settings...");
  ulong ms = millis();
  while (millis() < (ms + 1000)){
    if(digitalRead(OK_BUTTON_PIN) == OK_BUTT_ACTIVE_LVL){
      D_PRINT(true, "Restoring default settings...");
      restoreDefaultSettings();
    }
  }
  D_PRINTLN(false, "Done");

  D_PRINT(true, "Loading Settings..");
  uint8_t result = SettingsRead();
  if( result != 0 ){
    switch ( result )
    {
    case ERROR_SETTINGS_CRC:
        D_PRINT(true, "\nError: Bad CRC on settings datas\nResoring default settings.\n");
        restoreDefaultSettings();
      break;
    
    default:
      break;
    }
  }else{
    D_PRINT(false, "Ok\n");
  }

#ifdef LCD_POPULATED
  D_PRINT(true, "Setting up Display...");
  if(display_init()){
    D_PRINTLN(false, "FAIL!");
  }
  D_PRINTLN(false, "Ok.");

  dispLogoPage();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner

  
  display.printf("Setting up WIFI...");
#endif

  if(Settings.settingFlags.wifiEnabled){
    D_PRINT(true, "Setting up WIFI...");
    if(!wifi_setup()){
      D_PRINTLN(false, "WIFI connection KO!");   
    }
    else{
      D_PRINTLN(false, "OK!");
      D_PRINTLN(true, "WIFI connection Successful");
      D_PRINTLN(true, "WIFI IP Address is:%s", WiFi.localIP().toString().c_str() );
    }
  }
  else{
    D_PRINTLN(true, "Wifi is NOT enabled!!");
  }

  D_PRINT(true, "Setting up AP...");
  if(!AP_setup()){  
    D_PRINTLN(false, "KO!!");
  }
  else{
    D_PRINTLN(false, "OK!");
    D_PRINTLN(true, "AP IP address = %s" ,  WiFi.softAPIP().toString().c_str());
#ifdef LCD_POPULATED
    display.println(WiFi.softAPIP());
#endif
  }

  D_PRINT(true, "Starting web server...");
  if (web_serv_setup() == NO_ERRORS) {
    D_PRINTLN(false, "OK!");
  }else{
    D_PRINTLN(true, "<<<<<<<<<<<<< ERROR!");
  }
  setupIO();
  // test output
#ifdef AUDIO_OUT_PIN
#ifdef ESP32 
  ledcWrite(LIGHT_PWM_CH, (LIGHT_OUT_ACTIVE_LVL)  ? pow(2,LIGHT_PWM_RES_BITS) -1 : 0);
  ledcWriteTone(AUDIO_PWM_CH,1000);
#else
  tone(AUDIO_OUT_PIN,TONE_FREQ);
  //digitalWrite(LIGHT_OUT_PIN,LIGHT_OUT_ACTIVE_LVL); 
#endif
#endif
#ifdef LCD_POPULATED
  dispInfoPage();
#endif
  delay(1000);
#ifdef LCD_POPULATED
  dispReadyPage();
#endif
// test output end
#ifdef ESP32
  ledcWrite(LIGHT_PWM_CH, (LIGHT_OUT_ACTIVE_LVL)  ? 0 : pow(2,LIGHT_PWM_RES_BITS)-1);
  ledcWriteTone(AUDIO_PWM_CH,0);
#else
  digitalWrite(LIGHT_OUT_PIN,!LIGHT_OUT_ACTIVE_LVL); 
  noTone(AUDIO_OUT_PIN);
#endif
  D_PRINTLN(true, "WIFI SSID:%s", Settings.wifi_ssid);
  D_PRINTLN(true, "WIFI PWD:%s", Settings.wifi_psw);
  D_PRINTLN(true, "AP SSID:%s", Settings.ap_ssid);
  D_PRINTLN(true, "AP PWD:%s", Settings.ap_psw);
  D_PRINTLN(true, "Light Freq:%u", Settings.light_freq);
  D_PRINTLN(true, "On time:%u", Settings.on_time);
  D_PRINTLN(true, "Brightness:%u", Settings.brightness);
  D_PRINTLN(true, "PWM Freq:%u", Settings.pwm_freq);
  D_PRINTLN(true, "Autostart:%s", (Settings.settingFlags.autostart == true ) ? "ON" : "OFF");
  D_PRINTLN(true, "lightEnabled:%s", (Settings.settingFlags.lightEnabled == true ) ? "ON" : "OFF");
  D_PRINTLN(true, "audioEnabled:%s", (Settings.settingFlags.audioEnabled == true ) ? "ON" : "OFF");

  attachInterrupt(digitalPinToInterrupt(OK_BUTTON_PIN), buttonChangedISR, CHANGE);

  if(Settings.settingFlags.autostart){
    Command = CMD_START;
  }  
  /// tests
  //check_flash();
  //D_PRINTLN(true, "Size of indexp:%d\n", sizeof(index_page));
  //test();
  //testButtonHandle();
  //system_phy_set_powerup_option(0);  // 3 to reset RF CALIBRATION
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
  digitalWrite(STATUS_LED, (bHalfSecond == true) ? STATUS_LED_ACTIVE_LVL : !STATUS_LED_ACTIVE_LVL );   
#endif

  if( Status == deviceStatus_t::STATUS_WORKING ){
    // light switch
    u_long now = micros();
    u_long diff = now - lastSwitch;
    
    if( diff >= semiPeriod ){
      if(Settings.settingFlags.lightEnabled){
#ifdef ESP32
      //(bOutputStatus == false) ? ledcWrite(LIGHT_PWM_CH, (LIGHT_OUT_ACTIVE_LVL)  ? (int)(Settings.brightness) : 100 -(int)(Settings.brightness)) : ledcWrite(LIGHT_PWM_CH, (LIGHT_OUT_ACTIVE_LVL)  ? 0 : 100);
      (bOutputStatus == true) ? ledcWrite(LIGHT_PWM_CH, LightPWMDutyVal) : ledcWrite(LIGHT_PWM_CH, (LIGHT_OUT_ACTIVE_LVL)  ? 0 :pow(2,LIGHT_PWM_RES_BITS)-1);
#else
        (bOutputStatus == true) ? analogWrite(LIGHT_OUT_PIN, (LIGHT_OUT_ACTIVE_LVL)  ? (int)(Settings.brightness) : 100 -(int)(Settings.brightness)) : digitalWrite(LIGHT_OUT_PIN, !LIGHT_OUT_ACTIVE_LVL);
        //if(lightFreqErrorUs > maxLightFreqErrorUs ) maxLightFreqErrorUs = lightFreqErrorUs;
#endif
      }
#ifdef AUDIO_OUT_PIN
      if(Settings.settingFlags.audioEnabled){
#ifdef ESP32
        (bOutputStatus) ? ledcWriteTone(AUDIO_PWM_CH,1000) : ledcWriteTone(AUDIO_PWM_CH,0);
#else
        (bOutputStatus) ? tone(AUDIO_OUT_PIN,TONE_FREQ) : noTone(AUDIO_OUT_PIN);
        //digitalWrite(AUDIO_OUT_PIN, (bOutputStatus) ? bOutputStatus : !bOutputStatus);
#endif
      }
#endif
      lightFreqErrorUs = diff - semiPeriod;  // calculate error
      bOutputStatus = !bOutputStatus;
      lastSwitch = now ; 
    }
    // check for working timeout
    rem_time = (Settings.on_time * 60 - ((millis() - start_millis) / 1000)); // 14 uS
    if(rem_time<=0){
      D_PRINTLN(true, "Timed Out");
      stop();
    }  
  }
  else{ // not working not in menu
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
  }  
  if(Status == deviceStatus_t::STATUS_MENU ){
#ifdef LCD_POPULATED
    menu();
#endif
  }
}
