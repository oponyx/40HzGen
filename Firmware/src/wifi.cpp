#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wifi.h"
#include "settings.h"


bool AP_setup(){

    IPAddress local_IP(192,168,16,4);
    IPAddress gateway(192,168,16,9);
    IPAddress subnet(255,255,255,0);

    Serial.print("Setting up AP Configuration... ");
    if(!WiFi.softAPConfig(local_IP, gateway, subnet)){
        Serial.println("Failed!");
        return true;
    }
    Serial.println( "OK");

    Serial.print("Setting up AP... ");
    if(!WiFi.softAP(Settings.ap_ssid, Settings.ap_psw)){
        Serial.println("Failed!");
        return true;
    }
    Serial.println("OK");
    return false;
}

bool wifi_setup(){
  WiFi.begin(Settings.wifi_ssid, Settings.wifi_psw);
  for (int iRetry=0; iRetry<50; iRetry++)
 // while (WiFi.status() != WL_CONNECTED) 
  {
    if((WiFi.status() == WL_CONNECTED)){
        return false;
    }
     delay(500);
     Serial.print("*");
  }
  
  Serial.println("");
  return true;
}