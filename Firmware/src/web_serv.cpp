/*
web_server.cpp - 40HzGen functions and configurations for the web server 

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
#include <AsyncJson.h>
//#include <ArduinoJson.h>
#include <Updater.h>
#include <StreamString.h>

#include "web_serv.h"
#include "globals.h"
#include "settings.h"
#include "hw_config.h"
#include "functions.h"
#include "error_codes.h"
#include "config.h"
#include "log.h"

// compressed html pages
#include "index.html.h"
#include "rebooting.html.h"
#include "update.html.h"
#include "settings.html.h"
#include "redirect.html.h"


AsyncWebServer web_server(80); // the Generator web server
AsyncWebServerRequest *webRequest;

//const char TEXTJSON[] PROGMEM = "text/json";
const char TEXTHTML[] PROGMEM =     "text/html";
const char TEXTPLAIN[] PROGMEM =    "text/plain";
size_t firmwareUpdateProgress =     0;

/**
 * @brief Function to init the web server
 * 
 */
uint16_t web_serv_setup(){
  
  web_server.on("/", HTTP_GET, handleRoot);
  web_server.on("/index.html", HTTP_GET, handleRoot);
  web_server.on("/settings", HTTP_GET, handleSettingsPage);
  web_server.on("/reboot", HTTP_GET, handleReboot);
  web_server.on("/start", HTTP_GET, handleStart);
  web_server.on("/stop", HTTP_GET, handleStop);
  web_server.on("/set_freq", HTTP_POST, handleSetFreq);
  web_server.on("/set_bri", HTTP_POST, handleSetBri);
  // send status json message
  web_server.on("/device_status", HTTP_GET, handleDeviceStatus);
  // save settings on form submit
  web_server.on("/save_settings", HTTP_POST, handleSaveSettings); 
  //firmware update progress ///
  web_server.on(PSTR("/update_progress"), HTTP_GET, handleUpdateProgress); 
  /// get settings
  web_server.on(PSTR("/get_settings"), HTTP_GET, handleGetSettings);
  // Firmware Update Page
  web_server.on(PSTR("/update"), HTTP_GET, handleUpdateGET);
  ////// Firmware update data receive
  web_server.on(PSTR("/update"), HTTP_POST, handleUpdatePOST , firmwareUploadHandler);
  // start the server
  web_server.begin();
  return NO_ERRORS;
}

void handleRoot(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  AsyncWebServerResponse* response = request->beginResponse_P(200, FPSTR(TEXTHTML), (const uint8_t *)index_html, index_html_len);
  response->addHeader("Content-Encoding", "gzip"); 
  request->send(response);
}

void handleStop(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/stop' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  AsyncJsonResponse * resp = new AsyncJsonResponse();
  resp->addHeader("Server", WEB_SERVER_NAME);
  JsonObject root = resp->getRoot();
  root["result"] = "OK";
  root["Error_code"] = 0;
  root["Error_message"] = "No Errors";
  resp->setLength();
  request->send(resp);
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB sent response\n");
#endif
  Command = CMD_STOP; // Stop command
}

void handleReboot(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/reboot' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  //request->send_P(200, FPSTR(TEXTHTML), rebooting_page);
  AsyncJsonResponse * resp = new AsyncJsonResponse();
  resp->addHeader("Server", WEB_SERVER_NAME);
  JsonObject root = resp->getRoot();
  root["result"] = "OK";
  root["error_code"] = 0;
  root["error_message"] = "No Errors";
  root["message"] = "Rebooting...";
  resp->setLength();
  request->onDisconnect([]() {
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: /reboot disconnect from client. Rebooting...\n");
#endif
    //ESP.restart();
    Command = CMD_REBOOT; //reboot request
  });

  request->send(resp);
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB sent response\n");
#endif
}

void handleStart(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/start' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  AsyncJsonResponse * resp = new AsyncJsonResponse();
  resp->addHeader("Server", WEB_SERVER_NAME);
  JsonObject root = resp->getRoot();
  root["result"] = "OK";
  root["Error_code"] = 0;
  root["Error_message"] = "No Errors";
  resp->setLength();
  request->send(resp);
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB sent response\n");
#endif
  Command = CMD_START; // Start command
}

void handleSetFreq(AsyncWebServerRequest *request){
  long val;
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/set_freq' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  int params = request->params();
  for(int i=0;i<params;i++){
    AsyncWebParameter* p = request->getParam(i);
    if(p->isPost()){
#ifdef __DEBUG_40HZ_WEB__
      m_log(true, "set_freq page received->POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
#endif
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
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB sent response\n");
#endif
}

void handleSetBri(AsyncWebServerRequest *request){
  long val;
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/set_bri' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  int params = request->params();
  for(int i=0;i<params;i++){
    AsyncWebParameter* p = request->getParam(i);
    if(p->isPost()){
#ifdef __DEBUG_40HZ_WEB__
      m_log(true, "set_bri page received->POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
#endif
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
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB sent response\n");
#endif
}

void handleDeviceStatus(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/device_status' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
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
  root["RSSI"] = WiFi.RSSI();
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
}

void handleSaveSettings(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
    m_log(true, "WEB: HTTP-GET request '/save_settings' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
    webRequest = request;
    Command = CMD_GET_SETTINGS_REQ_PARAMS;
  }

void handleUpdateProgress(AsyncWebServerRequest *request){
  // send progress
  AsyncJsonResponse * resp = new AsyncJsonResponse();
  resp->addHeader("Server", WEB_SERVER_NAME);
  JsonObject root = resp->getRoot();
  root["progress"] = firmwareUpdateProgress;
  resp->setLength();
  request->send(resp);
  }

void handleGetSettings(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/get_settings' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  AsyncJsonResponse * resp = new AsyncJsonResponse();
  resp->addHeader("Server", WEB_SERVER_NAME);
  JsonObject root = resp->getRoot();
  root["crc"] = Settings.crc;
  root["version"] = Settings.version;
  root["wifi_ssid"] = Settings.wifi_ssid;
  root["wifi_psw"] = Settings.wifi_psw;
  root["ap_ssid"] = Settings.ap_ssid;
  root["ap_psw"] = Settings.ap_psw;
  root["light_freq"] = Settings.light_freq;
  root["on_time"] = Settings.on_time;
  root["brightness"] = Settings.brightness;
  root["pwm_freq"] = Settings.pwm_freq;
  root["autostart"] = (Settings.settingFlags.autostart) ? "true" : "false";
  
  resp->setLength();
  request->send(resp);
}

void handleUpdateGET(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/update' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  AsyncWebServerResponse* response = request->beginResponse_P(200, FPSTR(TEXTHTML), (const uint8_t *)update_html, update_html_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}


void handleUpdatePOST(AsyncWebServerRequest *request){
      /////////////////  ArRequestHandlerFunction START  //////////////////////
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-POST request '/update' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  bool shouldReboot = !Update.hasError();
  String s = "";
  if (Update.hasError()) {
    StreamString str;
    Update.printError(str);
    s += "An error during firmware update!\n%s";
    s += str;
  }
  else {
    s = "Firmware updated! Device is rebooting...";
  }
  AsyncWebServerResponse *response = request->beginResponse(200, FPSTR(TEXTPLAIN), s);
  if (shouldReboot){
#ifdef __DEBUG_40HZ_WEB__
    m_log(true, "shouldReboot\n");
#endif
    request->onDisconnect([]() {
        ESP.restart();
    });
  }
  response->addHeader(F("Connection"), F("close"));
  request->send(response);
    ///////////////   ArRequestHandlerFunction END   ///////////////////
}

void handleSettingsPage(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  m_log(true, "WEB: HTTP-GET request '/settings' from %s\n", request->client()->remoteIP().toString().c_str());
#endif
  AsyncWebServerResponse* response = request->beginResponse_P(200, FPSTR(TEXTHTML), (const uint8_t *)settings_html, settings_html_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response); //, processor
}

void firmwareUploadHandler(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        ////////////////////// ArUploadHandlerFunction START  ////////////////////
      if (!index) {
          firmwareUpdateProgress = 0;
          Update.runAsync(true);
          uint32_t maxSketchSize = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if (!Update.begin(maxSketchSize)) {
#ifdef __DEBUG_40HZ_WEB__
              m_log(true, "Update begin error:\n");
              Update.printError(Serial);
#endif
          }
      }
///// len
      if(len){
#ifdef __DEBUG_40HZ_WEB__
        //m_log(true, "Len:%d index:%d\n", len, index);
#endif
      }
      /// hasError
      if (!Update.hasError()) {
#ifdef __DEBUG_40HZ_WEB__
        m_log(false, ".");
#endif
        firmwareUpdateProgress = Update.progress(); // index + len;
        if (Update.write(data, len) != len) {
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "Update error:\n");
          Update.printError(Serial);
#endif
        }
      }
      ////////////  final
      if (final) {
        if (Update.end(true)) {
              m_log(true, "\nUpdate Success: %u B\n", index+len);
        } else {
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "\nUpdate failed:\n");
          Update.printError(Serial);
#endif
        }
      }
        ////////////////////// ArUploadHandlerFunction END  ////////////////////
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
#ifdef __DEBUG_40HZ_WEB__
      m_log(true, "save_settings page received->POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
#endif
      /// read form submit parameters and save settings
      ////////////////////////////////////////////////
      if( p->name() == "WIFI_SSID" ){
        if(String(Settings.wifi_ssid) != String(p->value())){
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "WIFI_SSID changed. old:%s new:%s\n", String(Settings.wifi_ssid).c_str(), p->value().c_str());
#endif
          strcpy(Settings.wifi_ssid,p->value().c_str());
          bSettingsChanged = true;
        }
      } else if( p->name() == "WIFI_PSW" ){
        if(String(Settings.wifi_psw) != String(p->value())){
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "WIFI_PSW changed. old:%s new:%s\n", String(Settings.wifi_psw).c_str(), p->value().c_str());
#endif
          strcpy(Settings.wifi_psw,p->value().c_str());
          bSettingsChanged = true;
        }
      } else if( p->name() == "AP_SSID" ){
        if(String(Settings.ap_ssid) != String(p->value())){
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "AP_SSID changed. old:%s new:%s\n", String(Settings.ap_ssid).c_str(), p->value().c_str());
#endif
          strcpy(Settings.ap_ssid,p->value().c_str());
          bSettingsChanged = true;
        }
      } else if( p->name() == "AP_PSW" ){
        if(String(Settings.ap_psw) != String(p->value())){
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "AP_PSW changed. old:%s new:%s\n", String(Settings.ap_psw).c_str(), p->value().c_str());
#endif
          strcpy(Settings.ap_psw,p->value().c_str());
          bSettingsChanged = true;
        }
      } else if ( p->name() == "LIGHT_FREQ" ){
        val = p->value().toInt();
        if( (uint8_t)val != Settings.light_freq ){
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "LIGHT_FREQ changed. old:%s new:%s\n", String(Settings.light_freq).c_str(), p->value().c_str());
#endif
          // check min/max 
          val = ( val < MIN_LIGHT_FREQ ) ? MIN_LIGHT_FREQ : val ;
          val = ( val > MAX_LIGHT_FREQ ) ? MAX_LIGHT_FREQ : val ;
          Settings.light_freq = val;
          bSettingsChanged = true;
        }
      } else if ( p->name() == "ON_TIME" ){
        val = p->value().toInt();
        if( (uint8_t)val != Settings.on_time ){
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "ON_TIME changed. old:%s new:%s\n", String(Settings.on_time).c_str(), p->value().c_str());
#endif
          // check min/max 
          val = ( val < MIN_ON_TIME ) ? MIN_ON_TIME : val ;
          val = ( val > MAX_ON_TIME ) ? MAX_ON_TIME : val ;  
          Settings.on_time =  val;
          bSettingsChanged = true;
        }
      } else if ( p->name() == "BRIGHTNESS" ){
        val = p->value().toInt();
        if( (uint8_t)val != Settings.brightness ){
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "BRIGHTNESS changed. old:%s new:%s\n", String(Settings.brightness).c_str(), p->value().c_str());
#endif
          // check min/max 
          val = ( val < MIN_BRIGHTNESS ) ? MIN_BRIGHTNESS : val ;
          val = ( val > MAX_BRIGHTNESS ) ? MAX_BRIGHTNESS : val ;  
          Settings.brightness =  val;
          bSettingsChanged = true;
        }
      } else if ( p->name() == "PWM_FREQ" ){
        val = p->value().toInt();
          if( (uint32_t)val != Settings.pwm_freq ){
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "PWM_FREQ changed. old:%s new:%s\n", String(Settings.pwm_freq).c_str(), p->value().c_str());
#endif
          // check min/max 
          val = ( val < MIN_PWM_FREQ ) ? MIN_PWM_FREQ : val ;
          val = ( val > MAX_PWM_FREQ ) ? MAX_PWM_FREQ : val ;  
          Settings.pwm_freq =  val;
          bSettingsChanged = true;
        }
      } else if ( p->name() == "AUTOSTART_SW" ){
        // if received parameter AUTOSTART_SW it means switch is checked..
#ifdef __DEBUG_40HZ_WEB__
        m_log(true, "AUTOSTART_SW param received. old setting:%s\n", String(Settings.settingFlags.autostart).c_str());
#endif
        bAutostartParamReceived = true;
        if(!Settings.settingFlags.autostart){
#ifdef __DEBUG_40HZ_WEB__
          m_log(true, "AUTOSTART_SW changed. old:%s new:true\n", String(Settings.settingFlags.autostart).c_str());
#endif
          Settings.settingFlags.autostart =  true;
          bSettingsChanged=true;
        }
      }

    } //if(p->isPost())
  } // for(int i=0;i<params;i++)
  // if not received parameter AUTOSTART_SW it means switch is NOT checked..
  if(!bAutostartParamReceived) {
#ifdef __DEBUG_40HZ_WEB__
    m_log(true, "AUTOSTART_SW param not received. old:%s\n", String(Settings.settingFlags.autostart).c_str());
#endif
    if(Settings.settingFlags.autostart){
#ifdef __DEBUG_40HZ_WEB__
      m_log(true, "AUTOSTART_SW changed. old:%s new:false\n", String(Settings.settingFlags.autostart).c_str());
#endif
      Settings.settingFlags.autostart =  false;
      bSettingsChanged=true;
    }
  }
  if(bSettingsChanged){
    return true;
  }
  return false;
}

void sendRebootingPage(AsyncWebServerRequest *webRequest){
  AsyncWebServerResponse* response = webRequest->beginResponse_P(200, FPSTR(TEXTHTML), (const uint8_t *)rebooting_html, rebooting_html_len);
  response->addHeader("Content-Encoding", "gzip");
  webRequest->send(response);
}

void sendIndexPage(AsyncWebServerRequest *webRequest){
  AsyncWebServerResponse* response = webRequest->beginResponse_P(200, FPSTR(TEXTHTML), (const uint8_t *)index_html, index_html_len);
  response->addHeader("Content-Encoding", "gzip");
  webRequest->send(response);
}