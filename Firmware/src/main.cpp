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

#include "globals.h"
#include "config.h"
#include "log.h"
#include "graphic.h"
#include "settings.h"
#include "wifi.h"
#include "web_serv.h"
#include "hw_config.h"
#include "functions.h"
#include "error_codes.h"
#include "button.hpp"

// USING_TIM_DIV1 min 5hz Light Frequence could be used 
#define USING_TIM_DIV1                true           // for shortest and most accurate timer
#define USING_TIM_DIV16               false          // for medium time and medium accurate timer
#define USING_TIM_DIV256              false          // for longest timer but least accurate. Default


bool bDisplayUpdated;         // 'Display is updated' flag
bool bLastLightStatus;
u_long lastLigtSwitch;

deviceStatus_t Status;

#ifdef NEW_BUTTONS
Button okButton(OK_BUTTON_PIN, INPUT_PULLUP, HIGH);
Button cancelButton(CANCEL_BUTTON_PIN, INPUT_PULLUP, HIGH);
Button upButton(UP_BUTTON_PIN, INPUT_PULLUP, HIGH);
Button downButton(DOWN_BUTTON_PIN, INPUT_PULLUP, HIGH);

/*
  okButton = Button(OK_BUTTON_PIN, INPUT_PULLUP, HIGH);
  cancelButton = Button(CANCEL_BUTTON_PIN, INPUT_PULLUP, HIGH);
  upButton = Button(UP_BUTTON_PIN, INPUT_PULLUP, HIGH);
  downButton = Button(DOWN_BUTTON_PIN, INPUT_PULLUP, HIGH);
  */
#endif

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
    Serial.println("Start updating " + type);  
  });  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nUpdate End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
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
  pinMode(AUDIO_OUT_PIN, OUTPUT);

#ifdef NEW_BUTTONS

  okButton.onButtonShortPressed(buttonShortPressed);
  okButton.onButtonLongPressed(buttonLongPressed);
  okButton.onButtonPressed(buttonPressed);
  okButton.onButtonReleased(buttonReleased);

  cancelButton.onButtonShortPressed(buttonShortPressed);
  cancelButton.onButtonLongPressed(buttonLongPressed);

  upButton.onButtonShortPressed(buttonShortPressed);
  upButton.onButtonLongPressed(buttonLongPressed);

  downButton.onButtonShortPressed(buttonShortPressed);
  downButton.onButtonLongPressed(buttonLongPressed);

  okButton.begin();
  cancelButton.begin();
  upButton.begin();
  downButton.begin();

#else
  pinMode(OK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CANCEL_BUTTON_PIN, INPUT_PULLUP);
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
#endif

  analogWriteFreq(Settings.pwm_freq);
  analogWriteRange(MAX_BRIGHTNESS);
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
  
  if(digitalRead(OK_BUTTON_PIN)){
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

  display_init();
  dispLogoPage();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner

  
  display.printf("Setting up WIFI...");
  Serial.printf("Setting up WIFI...");
  if(wifi_setup()){
    Serial.println("WIFI connection KO!");   
    display.println(F("Setting up AP..."));
    if(AP_setup()){  
      display.println(F("AP ko!!"));
    }
    else{
      Serial.print("AP IP address = ");
      Serial.println(WiFi.softAPIP());
      display.println(WiFi.softAPIP());
    }
  }
  else{
    Serial.println("OK!");
    Serial.println("WIFI connection Successful");
    Serial.print("WIFI IP Address is: ");
    Serial.println(WiFi.localIP());// Print the IP address
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
  dispInfoPage();
  delay(1000);
  dispReadyPage();
  digitalWrite(LIGHT_OUT_PIN,LOW); // test output end
  Serial.printf("WIFI SSID:%s\n", Settings.wifi_ssid);
  Serial.printf("WIFI PWD:%s\n", Settings.wifi_psw);
  Serial.printf("AP SSID:%s\n", Settings.ap_ssid);
  Serial.printf("AP PWD:%s\n", Settings.ap_psw);
  Serial.printf("Light Freq:%u\n", Settings.light_freq);
  Serial.printf("On time:%u\n", Settings.on_time);
  Serial.printf("Brightness:%u\n", Settings.brightness);
  Serial.printf("PWM Freq:%u\n", Settings.pwm_freq);

  test();
  testButtonHandle();
}

/**
 * @brief Main Loop
 * 
 */
void loop() {
#ifndef NEW_BUTTONS
  handleButtons();
#else
  okButton.handle();
  cancelButton.handle();
  upButton.handle();
  downButton.handle();
#endif
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
    if( (diff) > hertz2us(Settings.light_freq) ){
      lastLigtSwitch = now ; 
      (bLastLightStatus == false) ? analogWrite(LIGHT_OUT_PIN, (int)(Settings.brightness)) : analogWrite(LIGHT_OUT_PIN, 0);
      bLastLightStatus = !bLastLightStatus;
    }
    // Display update
    /*
    if(bHalfSecond){
      display.clearDisplay();
      bDisplayUpdated=false;
    }else{
      if(!bDisplayUpdated){
        dispWorkingPage();
        bDisplayUpdated=true;
      }
    } //bHalfSecond
    */
    // check for working timeout
    rem_time = (Settings.on_time * 60 - ((millis() - start_millis) / 1000)); // 14 uS
    if(rem_time<=0){
      Serial.println("Timed Out");
      stop();
    }  
  }
  else{ // not working
      ArduinoOTA.handle();
  }
  display.display();
  
}

