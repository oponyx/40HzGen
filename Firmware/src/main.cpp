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

  pinMode(OK_BUTTON, INPUT_PULLDOWN_16);
  pinMode(CANCEL_BUTTON, INPUT);
  pinMode(UP_BUTTON, INPUT);
  pinMode(DOWN_BUTTON, INPUT);
}

void setup() {
  void setupIO();
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

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
  if(!AP_setup()){  
    Serial.print("AP IP address = ");
    Serial.println(WiFi.softAPIP());
    display.println(WiFi.softAPIP());
  }else{
    display.println(F("AP ko!!"));
  }
  
  display.println(F("Setting up WIFI..."));
  Serial.println(F("Setting up WIFI..."));
  if(!wifi_setup()){
    Serial.println("");
    Serial.println("WIFI connection Successful");
    Serial.print("WIFI IP Address is: ");
    Serial.println(WiFi.localIP());// Print the IP address
  }
  else{
     Serial.println("WIFI connection KO!");   
  }

  display.println(F("Starting web server"));
  web_serv_setup();


  // per aggiornamenti OTA
  setupOTA();

  delay(1000);
  display.println(F("Working..."));
  display.display();

}

void loop() {

  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage LOW
  delay(12);            // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(13);            // Wait for two seconds  
  ArduinoOTA.handle();	

}

