/**
 * @file main.cpp
 * @author oponyx
 * @brief 
 * @version 0.1
 * @date 2022-01-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include <stdint.h>

#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

#include "config_override.h"
#include "globals.h"
#include "config.h"
#include "log.h"
#include "settings.h"
#include "wifi.h"
#include "web_serv.h"
#include "hw_config.h"
#include "functions.h"
#include "error_codes.h"
#include "button.hpp"
#ifdef LCD_POPULATED
#include "graphic.h"
#endif



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


void ICACHE_RAM_ATTR buttonChangedISR(){
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
  Serial.println("");
  Serial.println("Serial ok");
  // restore default settings if okButton pressed at powerup
  if(digitalRead(OK_BUTTON_PIN) == OK_BUTT_ACTIVE_LVL){
    Serial.println("Restoring default setup...");
    restoreDefaultSettings();
  }

  Serial.printf("Loading Settings..");
  uint8_t result = SettingsRead();
  if( result != 0 ){
    switch ( result )
    {
    case ERROR_SETTINGS_CRC:
        Serial.println("\nError: Bad CRC on settings datas\nResoring default settings.");
        restoreDefaultSettings();
      break;
    
    default:
      break;
    }
  }else{
    Serial.println("Ok");
  }

#ifdef LCD_POPULATED
  Serial.printf("Setting up Display...");
  if(display_init()){
    Serial.println("FAIL!");
  }
  Serial.println("Ok.");

  dispLogoPage();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner

  
  display.printf("Setting up WIFI...");
#endif
  Serial.printf("Setting up WIFI...");
  if(wifi_setup()){
    Serial.println("WIFI connection KO!");   
  }
  else{
    Serial.println("OK!");
    Serial.println("WIFI connection Successful");
    Serial.print("WIFI IP Address is: ");
    Serial.println(WiFi.localIP());// Print the IP address
  }
  Serial.println(F("Setting up AP..."));
  if(AP_setup()){  
    Serial.println(F("AP ko!!"));
  }
  else{
    Serial.print("AP IP address = ");
    Serial.println(WiFi.softAPIP());
#ifdef LCD_POPULATED
    display.println(WiFi.softAPIP());
#endif
  }


  Serial.printf("Starting web server...");
  if (web_serv_setup() == NO_ERRORS) {
    Serial.println("OK!");
  }else{
    Serial.println("ERROR!");
  }


  // per aggiornamenti OTA
  Serial.printf("Starting OTA service...");
  setupOTA();
  Serial.println("OK");
  
  setupIO();
  digitalWrite(LIGHT_OUT_PIN,HIGH); // test output
#ifdef LCD_POPULATED
  dispInfoPage();
  delay(1000);
  dispReadyPage();
#endif
  digitalWrite(LIGHT_OUT_PIN,LOW); // test output end
  Serial.printf("WIFI SSID:%s\n", Settings.wifi_ssid);
  Serial.printf("WIFI PWD:%s\n", Settings.wifi_psw);
  Serial.printf("AP SSID:%s\n", Settings.ap_ssid);
  Serial.printf("AP PWD:%s\n", Settings.ap_psw);
  Serial.printf("Light Freq:%u\n", Settings.light_freq);
  Serial.printf("On time:%u\n", Settings.on_time);
  Serial.printf("Brightness:%u\n", Settings.brightness);
  Serial.printf("PWM Freq:%u\n", Settings.pwm_freq);
  Serial.printf("Autostart:%s\n", (Settings.settingFlags.autostart == true ) ? "ON" : "OFF");

  attachInterrupt(digitalPinToInterrupt(OK_BUTTON_PIN), buttonChangedISR, CHANGE);

  if(Settings.settingFlags.autostart){
    Command = CMD_START;
  }
  if (!MDNS.begin("40HzGen")) {  //Start mDNS with name esp8266
    Serial.println("Error starting MDNS!");
  }
  Serial.printf("Size of indexp:%d\n", sizeof(index_page));
  test();
  testButtonHandle();
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
  digitalWrite(LED_BUILTIN, (bHalfSecond == true) ? LOW : HIGH );   
  

  if( Status == deviceStatus_t::STATUS_WORKING ){
    // light switch
    u_long now = micros();
    u_long diff = now - lastLigtSwitch;
    
    if( diff >= semiPeriod ){
      (bLastLightStatus == false) ? analogWrite(LIGHT_OUT_PIN, (int)(Settings.brightness)) : analogWrite(LIGHT_OUT_PIN, 0);
      lightFreqErrorUs = diff - semiPeriod;  // calculate error
      if(lightFreqErrorUs > maxLightFreqErrorUs ) maxLightFreqErrorUs = lightFreqErrorUs;
      bLastLightStatus = !bLastLightStatus;
      lastLigtSwitch = now ; 
    }

    // check for working timeout
    rem_time = (Settings.on_time * 60 - ((millis() - start_millis) / 1000)); // 14 uS
    if(rem_time<=0){
      Serial.println("Timed Out");
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

      ArduinoOTA.handle();
  }
  //display.display();
  
}

