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

#include "log.h"
#include "graphic.h"
#include "settings.h"
#include "wifi.h"
#include "web_serv.h"
#include "hw_config.h"
#include "functions.h"
#include "error_codes.h"

// USING_TIM_DIV1 min 5hz Light Frequence could be used 
#define USING_TIM_DIV1                true           // for shortest and most accurate timer
#define USING_TIM_DIV16               false          // for medium time and medium accurate timer
#define USING_TIM_DIV256              false          // for longest timer but least accurate. Default
#include <ESP8266TimerInterrupt.h>



ESP8266TimerInterrupt InterruptTimer; // Timer Interrupt used to call the ISR
bool bDisplayUpdated;         // 'Display is updated' flag
bool bLastLightStatus;

/**
 * @brief Timed Interrupt Service Routine
 * 
 * Toggles the outputs
 * 
 * @return IRAM_ATTR 
 */
IRAM_ATTR void TimedISR() {
  lastTrigger = millis();
  if(bWorking){
    //switch the light on/off
    //digitalWrite(LIGHT_OUT, !digitalRead(LIGHT_OUT));
    (bLastLightStatus == false) ? analogWrite(LIGHT_OUT, Settings.brightness) : analogWrite(LIGHT_OUT, 0);
    bLastLightStatus = !bLastLightStatus;
  }
}

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
  pinMode(LIGHT_OUT, OUTPUT);
  pinMode(AUDIO_OUT, OUTPUT);

  pinMode(OK_BUTTON, INPUT_PULLUP);
  pinMode(CANCEL_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);

  analogWriteFreq(Settings.pwm_freq);
  analogWriteRange(MAX_BRIGHTNESS);
}

/**
 * @brief Main setup function
 * 
 */
void setup() {
  bWorking = false;
  bHalfSecond=false;

  Serial.begin(115200);
  // load settings
  Serial.println("");
  Serial.println("Serial ok");
  
  if(digitalRead(OK_BUTTON)){
    Serial.println("Restoring default setup...");
    restoreDefaultSettings();
  }

  Serial.println("Loading Settings");
  uint8_t result = SettingsRead();
  if( result != 0 ){
    switch ( result )
    {
    case ERROR_SETTINGS_CRC:
        Serial.println("Error: Bad CRC on settings datas\nResoring default settings.");
        restoreDefaultSettings();
      break;
    
    default:
      break;
    }
  }else{
    Serial.println("Settings loaded.");
  }
  Serial.printf("WIFI SSID:%s\n", Settings.wifi_ssid);
  Serial.printf("WIFI PWD:%s\n", Settings.wifi_psw);
  Serial.printf("AP SSID:%s\n", Settings.ap_ssid);
  Serial.printf("AP PWD:%s\n", Settings.ap_psw);
  Serial.printf("On time:%u\n", Settings.on_time);
  Serial.printf("Brightness:%u\n", Settings.brightness);
  Serial.printf("PWM Freq:%u\n", Settings.pwm_freq);

  display_init();
  dispLogoPage();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner

  display.println(F("Setting up AP..."));
  if(AP_setup()){  
    display.println(F("AP ko!!"));
  }
  else{
    Serial.print("AP IP address = ");
    Serial.println(WiFi.softAPIP());
    display.println(WiFi.softAPIP());
  }
  
  display.printf("Setting up WIFI...");
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

  // setup interrupt
    Serial.printf("Setting up interrupt timer...");
if (InterruptTimer.attachInterrupt(Settings.light_freq * 2, TimedISR)){
    Serial.println("OK, time = " + String(millis()));
  }
  else{
    Serial.println("Error setting Timed Interrupt. Bad freq?");
  }
  
  setupIO();
  digitalWrite(LIGHT_OUT,HIGH); // test output
  dispInfoPage();
  delay(1000);
  dispReadyPage();
  digitalWrite(LIGHT_OUT,LOW); // test output end
}

/**
 * @brief Main Loop
 * 
 */
void loop() {
  handleButtons();
  handleCommands();
  ArduinoOTA.handle();

  if((millis()-mills_2)>499){
    mills_2=millis();
    bHalfSecond=!bHalfSecond;
  }

  //flash status led
  digitalWrite(LED_BUILTIN, (bHalfSecond == true) ? LOW : HIGH );   


  if(bWorking){
    // Display update
    if(bHalfSecond){
      display.clearDisplay();
      bDisplayUpdated=false;
    }else{
      if(!bDisplayUpdated){
        dispWorkingPage();
        bDisplayUpdated=true;
      }
    } //bHalfSecond
    // check for working timeout
    int millis_count = millis() - start_millis;
    if((Settings.on_time * 60 * 1000)<millis_count){
      Serial.println("Timed Out");
      stop();
    }  
  }
  display.display();
  
}

