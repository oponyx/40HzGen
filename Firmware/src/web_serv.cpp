#include <AsyncJson.h>
#include <ArduinoJson.h>

#include "web_serv.h"
#include "globals.h"
#include "settings.h"
#include "hw_config.h"
#include "functions.h"
#include "error_codes.h"
#include "config.h"

AsyncWebServer web_server(80); // the Generator web server
AsyncWebServerRequest *webRequest;


/**
 * @brief Function to init the web server
 * 
 */
uint16_t web_serv_setup(){
  web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("WEB: HTTP-GET request '/' from %s\n", request->client()->remoteIP().toString().c_str());
    request->send_P(200, "text/html", index_page);
  });
  web_server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("WEB: HTTP-GET request '/index.html' from %s\n", request->client()->remoteIP().toString().c_str());
    request->send_P(200, "text/html", index_page);
  });
  web_server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.printf("WEB: HTTP-GET request '/settings' from %s\n", request->client()->remoteIP().toString().c_str());
      request->send_P(200, "text/html", settings_page, processor);
  });
  web_server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.printf("WEB: HTTP-GET request '/reboot' from %s\n", request->client()->remoteIP().toString().c_str());
    //request->send_P(200, "text/html", rebooting_page);
    AsyncJsonResponse * resp = new AsyncJsonResponse();
    resp->addHeader("Server", WEB_SERVER_NAME);
    JsonObject root = resp->getRoot();
    root["result"] = "OK";
    root["Error_code"] = 0;
    root["Error_message"] = "No Errors";
    resp->setLength();
    request->send(resp);
    Serial.printf("WEB sent response\n");
    Command = CMD_REBOOT; //reboot request
  });
  web_server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("WEB: HTTP-GET request '/start' from %s\n", request->client()->remoteIP().toString().c_str());
    AsyncJsonResponse * resp = new AsyncJsonResponse();
    resp->addHeader("Server", WEB_SERVER_NAME);
    JsonObject root = resp->getRoot();
    root["result"] = "OK";
    root["Error_code"] = 0;
    root["Error_message"] = "No Errors";
    resp->setLength();
    request->send(resp);
    Serial.printf("WEB sent response\n");
    Command = CMD_START; // Start command
    });
  web_server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("WEB: HTTP-GET request '/stop' from %s\n", request->client()->remoteIP().toString().c_str());
    AsyncJsonResponse * resp = new AsyncJsonResponse();
    resp->addHeader("Server", WEB_SERVER_NAME);
    JsonObject root = resp->getRoot();
    root["result"] = "OK";
    root["Error_code"] = 0;
    root["Error_message"] = "No Errors";
    resp->setLength();
    request->send(resp);
    Serial.printf("WEB sent response\n");
    Command = CMD_STOP; // Stop command
  });

  
    web_server.on("/set_freq", HTTP_POST, [](AsyncWebServerRequest *request){
    long val;
    Serial.printf("WEB: HTTP-GET request '/set_freq' from %s\n", request->client()->remoteIP().toString().c_str());
    int params = request->params();
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()){
        Serial.printf("set_freq page received->POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        if ( p->name() == "light_freq" ){
          val = p->value().toInt();
          // check min/max 
          val = ( val < MIN_LIGHT_FREQ ) ? MIN_LIGHT_FREQ : val ;
          val = ( val > MAX_LIGHT_FREQ ) ? MAX_LIGHT_FREQ : val ;
          Settings.light_freq = val;
        }
      }
    }
    AsyncJsonResponse * resp = new AsyncJsonResponse();
    resp->addHeader("Server", WEB_SERVER_NAME);
    JsonObject root = resp->getRoot();
    root["result"] = "OK";
    root["Error_code"] = 0;
    root["Error_message"] = "No Errors";
    root["light_freq"] = Settings.light_freq;
    resp->setLength();
    request->send(resp);
    Serial.printf("WEB sent response\n");
  });

  web_server.on("/set_bri", HTTP_POST, [](AsyncWebServerRequest *request){
    long val;
    Serial.printf("WEB: HTTP-GET request '/set_bri' from %s\n", request->client()->remoteIP().toString().c_str());
    int params = request->params();
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()){
        Serial.printf("set_bri page received->POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        if ( p->name() == "brightness" ){
          val = p->value().toInt();
          // check min/max 
          val = ( val < MIN_BRIGHTNESS ) ? MIN_BRIGHTNESS : val ;
          val = ( val > MAX_BRIGHTNESS ) ? MAX_BRIGHTNESS : val ;
          Settings.brightness = val;
        }
      }
    }
    AsyncJsonResponse * resp = new AsyncJsonResponse();
    resp->addHeader("Server", WEB_SERVER_NAME);
    JsonObject root = resp->getRoot();
    root["result"] = "OK";
    root["Error_code"] = 0;
    root["Error_message"] = "No Errors";
    root["brightness"] = Settings.brightness;
    resp->setLength();
    request->send(resp);
    Serial.printf("WEB sent response\n");
  });
  // send status json message
  web_server.on("/device_status", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("WEB: HTTP-GET request '/device_status' from %s\n", request->client()->remoteIP().toString().c_str());
    AsyncJsonResponse * resp = new AsyncJsonResponse();
    resp->addHeader("Server", WEB_SERVER_NAME);
    JsonObject root = resp->getRoot();
    root["working"] = (Status == deviceStatus_t::STATUS_WORKING) ? true : false;
    root["light_status"] = (Status == deviceStatus_t::STATUS_WORKING) ? "ON" : "OFF";
    root["rem_time"] = (Status == deviceStatus_t::STATUS_WORKING) ? rem_time : 0;
    root["audio_status"] = "unknown";
    root["light_freq"] = Settings.light_freq;
    root["free_heap"] = ESP.getFreeHeap();
    root["WiFi_ssid"] = WiFi.SSID();
    root["up_time"] = millis();
    root["brightness"] = Settings.brightness;
    root["pwm_freq"] = Settings.pwm_freq;
    root["okButtonStatus"] = ( okButton.status() == true ) ? "true" : "false";
#ifdef CANCEL_BUTTON_PIN
    root["cancelButtonStatus"] = ( cancelButton.status() == true ) ? "true" : "false";
#endif
#ifdef UP_BUTTON_PIN
    root["upButtonStatus"] = ( upButton.status() == true ) ? "true" : "false";
#endif
#ifdef DOWN_BUTTON_PIN
    root["downButtonStatus"] = ( downButton.status() == true ) ? "true" : "false";
#endif
    root["lightFreqErrorUs"] = (Status == deviceStatus_t::STATUS_WORKING) ? lightFreqErrorUs : 0;
    root["wifi_ip"] = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : "Not connected";
    resp->setLength();
    request->send(resp);
  });

  // save settings on form submit
  web_server.on("/save_settings", HTTP_POST, [](AsyncWebServerRequest * request){
    Serial.printf("WEB: HTTP-GET request '/save_settings' from %s\n", request->client()->remoteIP().toString().c_str());
    webRequest = request;
    Command = CMD_GET_SETTINGS_REQ_PARAMS;
  }); // server.on
  
  // start the server
  web_server.begin();
  return NO_ERRORS;
}

/**
 * @brief processor function to set the fields in HTML pages
 * 
 * @param var 
 * @return String 
 */
String processor(const String& var){
  //Serial.println(var);
  if(var == "WIFI_SSID"){
    return String(Settings.wifi_ssid);
  }
  else if(var == "WIFI_PSW"){
    return String(Settings.wifi_psw);
  }
  else if(var == "AP_SSID"){
    return String(Settings.ap_ssid);
  }
  else if(var == "AP_PSW"){
    return String(Settings.ap_psw);
  }
  else if(var == "LIGHT_FREQ"){
    return String(Settings.light_freq);
  }
  else if(var == "ON_TIME"){
    return String(Settings.on_time);
  }
  else if(var == "BRIGHTNESS"){
    return String(Settings.brightness);
  }
  else if(var == "PWM_FREQ"){
    return String(Settings.pwm_freq);
  }
  else if(var == "AUTOSTART_CHECKED"){
    return (Settings.settingFlags.autostart == true) ? String("checked='true'") : String("");
  }
  return String();
}

/**
 * @brief Get the Settings Request Param object
 * 
 * @param request 
 * @return true 
 * @return false 
 */
bool getSettingsRequestParam(AsyncWebServerRequest * request){
  long val;
  bool bSettingsChanged = false;
  int params = request->params();
  bool bAutostartParamReceived = false;

  for( int i=0; i<params; i++ ){
    AsyncWebParameter* p = request->getParam(i);
    if(p->isPost()){
      Serial.printf("save_settings page received->POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      /// read form submit parameters and save settings
      ////////////////////////////////////////////////
      if( p->name() == "WIFI_SSID" ){
        if(String(Settings.wifi_ssid) != String(p->value())){
          Serial.printf("WIFI_SSID changed. old:%s new:%s\n", String(Settings.wifi_ssid).c_str(), p->value().c_str());
          strcpy(Settings.wifi_ssid,p->value().c_str());
          bSettingsChanged = true;
        }
      } else if( p->name() == "WIFI_PSW" ){
        if(String(Settings.wifi_psw) != String(p->value())){
          Serial.printf("WIFI_PSW changed. old:%s new:%s\n", String(Settings.wifi_psw).c_str(), p->value().c_str());
          strcpy(Settings.wifi_psw,p->value().c_str());
          bSettingsChanged = true;
        }
      } else if( p->name() == "AP_SSID" ){
        if(String(Settings.ap_ssid) != String(p->value())){
          Serial.printf("AP_SSID changed. old:%s new:%s\n", String(Settings.ap_ssid).c_str(), p->value().c_str());
          strcpy(Settings.ap_ssid,p->value().c_str());
          bSettingsChanged = true;
        }
      } else if( p->name() == "AP_PSW" ){
        if(String(Settings.ap_psw) != String(p->value())){
          Serial.printf("AP_PSW changed. old:%s new:%s\n", String(Settings.ap_psw).c_str(), p->value().c_str());
          strcpy(Settings.ap_psw,p->value().c_str());
          bSettingsChanged = true;
        }
      } else if ( p->name() == "LIGHT_FREQ" ){
        val = p->value().toInt();
        if( (uint8_t)val != Settings.light_freq ){
          Serial.printf("LIGHT_FREQ changed. old:%s new:%s\n", String(Settings.light_freq).c_str(), p->value().c_str());
          // check min/max 
          val = ( val < MIN_LIGHT_FREQ ) ? MIN_LIGHT_FREQ : val ;
          val = ( val > MAX_LIGHT_FREQ ) ? MAX_LIGHT_FREQ : val ;
          Settings.light_freq = val;
          bSettingsChanged = true;
        }
      } else if ( p->name() == "ON_TIME" ){
        val = p->value().toInt();
        if( (uint8_t)val != Settings.on_time ){
          Serial.printf("ON_TIME changed. old:%s new:%s\n", String(Settings.on_time).c_str(), p->value().c_str());
          // check min/max 
          val = ( val < MIN_ON_TIME ) ? MIN_ON_TIME : val ;
          val = ( val > MAX_ON_TIME ) ? MAX_ON_TIME : val ;  
          Settings.on_time =  val;
          bSettingsChanged = true;
        }
      } else if ( p->name() == "BRIGHTNESS" ){
        val = p->value().toInt();
        if( (uint8_t)val != Settings.brightness ){
          Serial.printf("BRIGHTNESS changed. old:%s new:%s\n", String(Settings.brightness).c_str(), p->value().c_str());
          // check min/max 
          val = ( val < MIN_BRIGHTNESS ) ? MIN_BRIGHTNESS : val ;
          val = ( val > MAX_BRIGHTNESS ) ? MAX_BRIGHTNESS : val ;  
          Settings.brightness =  val;
          bSettingsChanged = true;
        }
      } else if ( p->name() == "PWM_FREQ" ){
        val = p->value().toInt();
          if( (uint32_t)val != Settings.pwm_freq ){
          Serial.printf("PWM_FREQ changed. old:%s new:%s\n", String(Settings.pwm_freq).c_str(), p->value().c_str());
          // check min/max 
          val = ( val < MIN_PWM_FREQ ) ? MIN_PWM_FREQ : val ;
          val = ( val > MAX_PWM_FREQ ) ? MAX_PWM_FREQ : val ;  
          Settings.pwm_freq =  val;
          bSettingsChanged = true;
        }
      } else if ( p->name() == "AUTOSTART_SW" ){
        // if received parameter AUTOSTART_SW it means switch is checked..
        Serial.printf("AUTOSTART_SW param received. old setting:%s\n", String(Settings.settingFlags.autostart).c_str());
        bAutostartParamReceived = true;
        if(!Settings.settingFlags.autostart){
          Serial.printf("AUTOSTART_SW changed. old:%s new:true\n", String(Settings.settingFlags.autostart).c_str());
          Settings.settingFlags.autostart =  true;
          bSettingsChanged=true;
        }
      }

    } //if(p->isPost())
  } // for(int i=0;i<params;i++)
  // if not received parameter AUTOSTART_SW it means switch is NOT checked..
  if(!bAutostartParamReceived) {
    Serial.printf("AUTOSTART_SW param not received. old:%s\n", String(Settings.settingFlags.autostart).c_str());
    if(Settings.settingFlags.autostart){
      Serial.printf("AUTOSTART_SW changed. old:%s new:false\n", String(Settings.settingFlags.autostart).c_str());
      Settings.settingFlags.autostart =  false;
      bSettingsChanged=true;
    }
  }
  if(bSettingsChanged){
    return true;
  }
  return false;
}
