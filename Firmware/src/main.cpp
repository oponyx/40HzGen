#include <Arduino.h>
#include <stdint.h>

#include <ArduinoOTA.h>

#include "log.h"
#include "graphic.h"
#include "settings.h"
#include "logo.h"
#include "wifi.h"
#include "web_serv.h"
#include "hw_config.h"
#include "functions.h"

// USING_TIM_DIV1 min 10hz Light Frequence could be used 
#define USING_TIM_DIV1                true           // for shortest and most accurate timer
#define USING_TIM_DIV16               false          // for medium time and medium accurate timer
#define USING_TIM_DIV256              false          // for longest timer but least accurate. Default
#include <ESP8266TimerInterrupt.h>



ESP8266TimerInterrupt ITimer;
bool bDisplayUpdated;



IRAM_ATTR void TimedISR() {
  lastTrigger = millis();
  if(bWorking){
    //switch the light on/off
    digitalWrite(LIGHT_OUT, !digitalRead(LIGHT_OUT));
  }
}


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
    Serial.println("\nEnd");
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

void setupIO(){

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LIGHT_OUT, OUTPUT);
  pinMode(AUDIO_OUT, OUTPUT);

  pinMode(OK_BUTTON, INPUT_PULLUP);
  pinMode(CANCEL_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);


}

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
  if(SettingsRead()){

  }
  Serial.printf("WIFI SSID:%s\n", Settings.wifi_ssid);
  Serial.printf("WIFI PWD:%s\n", Settings.wifi_psw);
  Serial.printf("AP SSID:%s\n", Settings.ap_ssid);
  Serial.printf("AP PWD:%s\n", Settings.ap_psw);

  display_init();
  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
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
  
  display.println(F("Setting up WIFI..."));
  Serial.println(F("Setting up WIFI..."));
  if(wifi_setup()){
    Serial.println("WIFI connection KO!");   
  }
  else{
    Serial.println("");
    Serial.println("WIFI connection Successful");
    Serial.print("WIFI IP Address is: ");
    Serial.println(WiFi.localIP());// Print the IP address
  }

  display.println(F("Starting web server"));
  web_serv_setup();


  // per aggiornamenti OTA
  setupOTA();

  // setup interrupt
  if (ITimer.attachInterrupt(Settings.light_freq * 2, TimedISR)){
    Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
  }
  else{
    Serial.println("Can't set ITimer. Select another freq. or timer");
  }
  
  setupIO();
  digitalWrite(LIGHT_OUT,HIGH); // test output
  dispInfoPage();
  delay(1000);
  dispReadyPage();
  digitalWrite(LIGHT_OUT,LOW); // test output end
}

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

