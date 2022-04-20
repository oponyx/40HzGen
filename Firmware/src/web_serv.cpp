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
#include <ArduinoJson.h>
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
const size_t CAPACITY = JSON_OBJECT_SIZE(1);


/**
 * @brief Function to init the web server
 * 
 */
uint16_t web_serv_setup(){


  web_server.on("/", HTTP_GET, handleRoot);
  web_server.on("/index.html", HTTP_GET, handleRoot);
  web_server.on("/settings", HTTP_GET, handleSettingsPage);
  web_server.on("/cmnd", HTTP_POST, onCmnd);

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
  // not found
  web_server.onNotFound(onNotFound);
// start the server
  web_server.begin();
  return NO_ERRORS;
}

void onNotFound(AsyncWebServerRequest *request){
  AsyncWebServerResponse *response = request->beginResponse(404, "text/plain", "Not Found!");
  response->addHeader("Server", WEB_SERVER_NAME);
  request->send(response);
}

void handleRoot(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  D_PRINTLN(true, "WEB: %s request '/' from %s", WEB_METHOD_GET, request->client()->remoteIP().toString().c_str());
#endif
  AsyncWebServerResponse* response = request->beginResponse_P(200, FPSTR(TEXTHTML), (const uint8_t *)index_html, index_html_len);
  response->addHeader("Content-Encoding", "gzip"); 
  request->send(response);
}


void handleDeviceStatus(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  D_PRINTLN(true, "WEB: %s request '/device_status' from %s", WEB_METHOD_GET, request->client()->remoteIP().toString().c_str());
#endif
  AsyncJsonResponse * resp = new AsyncJsonResponse();
  //resp->addHeader("Server", WEB_SERVER_NAME);
  JsonObject root = resp->getRoot();
  root["working"] = (Status == deviceStatus_t::STATUS_WORKING) ? true : false;
  root["light_status"] = (Status == deviceStatus_t::STATUS_WORKING) ? "ON" : "OFF";
  root["rem_time"] = (Status == deviceStatus_t::STATUS_WORKING) ? rem_time : 0;
#ifdef LIGHT_OUT_PIN
  root["audio_status"] = (Status == deviceStatus_t::STATUS_WORKING) ? "ON" : "OFF";
#else
  root["audio_status"] = "unknown";
#endif
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
    D_PRINTLN(true, "WEB: %s request '/save_settings' from %s",WEB_METHOD_GET, request->client()->remoteIP().toString().c_str());
#endif
    webRequest = request;
    Command = CMD_GET_SETTINGS_REQ_PARAMS;
  }

void handleUpdateProgress(AsyncWebServerRequest *request){
  // send progress
  AsyncJsonResponse * resp = new AsyncJsonResponse();
  //resp->addHeader("Server", WEB_SERVER_NAME);
  JsonObject root = resp->getRoot();
  root["progress"] = firmwareUpdateProgress;
  resp->setLength();
  request->send(resp);
  }

void handleGetSettings(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  D_PRINTLN(true, "WEB: %s request '/get_settings' from %s", WEB_METHOD_GET, request->client()->remoteIP().toString().c_str());
#endif
  AsyncJsonResponse * resp = new AsyncJsonResponse();
  //resp->addHeader("Server", WEB_SERVER_NAME);
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
  root["lightEnabled"] = (Settings.settingFlags.lightEnabled) ? "true" : "false";
  root["audioEnabled"] = (Settings.settingFlags.audioEnabled) ? "true" : "false";
  root["wifiEnabled"] = (Settings.settingFlags.wifiEnabled) ? "true" : "false";

  resp->setLength();
  request->send(resp);
}

void handleUpdateGET(AsyncWebServerRequest *request){
#ifdef __DEBUG_40HZ_WEB__
  D_PRINTLN(true, "WEB: %s request '/update' from %s", WEB_METHOD_GET, request->client()->remoteIP().toString().c_str());
#endif
  AsyncWebServerResponse* response = request->beginResponse_P(200, FPSTR(TEXTHTML), (const uint8_t *)update_html, update_html_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}


void handleUpdatePOST(AsyncWebServerRequest *request){
      /////////////////  ArRequestHandlerFunction START  //////////////////////
#ifdef __DEBUG_40HZ_WEB__
  D_PRINTLN(true, "WEB: %s request '/update' from %s", WEB_METHOD_POST, request->client()->remoteIP().toString().c_str());
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
    D_PRINTLN(true, "shouldReboot");
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
  D_PRINTLN(true, "WEB: %s request '/settings' from %s", WEB_METHOD_GET, request->client()->remoteIP().toString().c_str());
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
          D_PRINTLN(true, "Update begin error:");
          Update.printError(Serial);
#endif
      }
  }
///// len
  if(len){
#ifdef __DEBUG_40HZ_WEB__
    //D_PRINTLN("Len:%d index:%d", len, index);
#endif
  }
  /// hasError
  if (!Update.hasError()) {
#ifdef __DEBUG_40HZ_WEB__
    D_PRINT(false, ".");
#endif
    firmwareUpdateProgress = Update.progress(); // index + len;
    if (Update.write(data, len) != len) {
#ifdef __DEBUG_40HZ_WEB__
      D_PRINTLN(true, "Update error:");
      Update.printError(Serial);
#endif
    }
  }
  ////////////  final
  if (final) {
    if (Update.end(true)) {
          D_PRINTLN(true, "Update Success: %u B", index+len);
    } else {
#ifdef __DEBUG_40HZ_WEB__
      D_PRINTLN(true, "Update failed:");
      Update.printError(Serial);
#endif
    }
  }        ////////////////////// ArUploadHandlerFunction END  ////////////////////
}


void onCmnd(AsyncWebServerRequest * request){
#ifdef __DEBUG_40HZ_WEB__
  D_PRINTLN(true, "WEB: %s request '/cmnd' from %s", WEB_METHOD_POST, request->client()->remoteIP().toString().c_str());
#endif
  parseCommand(request);
}

/**
 * @brief Parse POST command data
 * 
 * @param request 
 */
void parseCommand(AsyncWebServerRequest *request)
{
  if(request->hasParam("cmnd", true)){
    if(!request->hasParam("value", true)){
      sendJsonError(request, "error", ERROR_INVALID_CMND, "Value param not received");
      return;
    }
    String cmnd = request->getParam("cmnd", true)->value();
    String value = request->getParam("value", true)->value();
    if (cmnd == "set_light_en") {         
      Settings.settingFlags.lightEnabled = (value == "true") ?  true : false;
    } else if (cmnd == "set_audio_en") {
      Settings.settingFlags.audioEnabled = (value == "true") ?  true : false;
    } else if (cmnd == "set_autostart") {
      Settings.settingFlags.autostart = (value == "true") ?  true : false;
    } else if (cmnd == "set_wifi") {
      Settings.settingFlags.wifiEnabled = (value == "true") ?  true : false;
    } else if (cmnd == "start") {
      Command = (value == "true") ? CMD_START : CMD_STOP;
    } else if (cmnd == "set_light_freq") {
      long val = request->getParam("value", true)->value().toInt();
      // check min/max 
      val = ( val < MIN_LIGHT_FREQ ) ? MIN_LIGHT_FREQ : val ;
      val = ( val > MAX_LIGHT_FREQ ) ? MAX_LIGHT_FREQ : val ;
      Settings.light_freq = val;
      AsyncJsonResponse * resp = new AsyncJsonResponse();
      //resp->addHeader("Server", WEB_SERVER_NAME);
      JsonObject root = resp->getRoot();
      root["result"] = "OK";
      root["error_code"] = NO_ERRORS;
      root["error_message"] = "No Errors";
      root["light_freq"] = Settings.light_freq;
      resp->setLength();
      request->send(resp);
      return;
    }  else if (cmnd == "set_bri") {
      long val = request->getParam("value", true)->value().toInt();
      // check min/max 
      val = ( val < MIN_LIGHT_FREQ ) ? MIN_LIGHT_FREQ : val ;
      val = ( val > MAX_LIGHT_FREQ ) ? MAX_LIGHT_FREQ : val ;
      Settings.brightness = val;
      AsyncJsonResponse * resp = new AsyncJsonResponse();
      //resp->addHeader("Server", WEB_SERVER_NAME);
      JsonObject root = resp->getRoot();
      root["result"] = "OK";
      root["error_code"] = NO_ERRORS;
      root["error_message"] = "No Errors";
      root["brightness"] = Settings.brightness;
      resp->setLength();
      request->send(resp);
      return;
    } else if(cmnd == "reboot"){
        request->onDisconnect([]() {
#ifdef __DEBUG_40HZ_WEB__
        D_PRINTLN(true, "WEB: /cmnd reboot disconnect from client. Rebooting...");
#endif
        //ESP.restart();
        Command = CMD_REBOOT; //reboot request
        return;
      });
      AsyncJsonResponse * resp = new AsyncJsonResponse();
      //resp->addHeader("Server", WEB_SERVER_NAME);
      JsonObject root = resp->getRoot();
      root["result"] = "OK";
      root["error_code"] = NO_ERRORS;
      root["error_message"] = "No Errors";
      root["message"] = "Rebooting...";
      resp->setLength();
      request->send(resp);
    }else {
      sendJsonError(request, "error", ERROR_INVALID_CMND, "Invalid command");
      return;
    }
  }
  else{
    sendJsonError(request, "error", ERROR_INVALID_CMND, "Command param not received");
    return;
  }
  sendJsonError(request, "OK", NO_ERRORS, "OK");
}


/**
 * @brief Send a json message containing the error data
 * 
 * @param request 
 * @param err_code 
 * @param error 
 */
void sendJsonError(AsyncWebServerRequest* request, String result, uint16_t err_code, String error_message){
  AsyncJsonResponse * resp = new AsyncJsonResponse();
  //resp->addHeader("Server", WEB_SERVER_NAME);
  JsonObject root = resp->getRoot();
  root["result"] = result;
  root["error_code"] = err_code;
  root["error_message"] = error_message;
  resp->setLength();
  request->send(resp);
}


/**
 * @brief Get the Settings Request Param object
 * 
 * @param request 
 * @return true 
 * @return false 
 */
bool getSettingsRequestParam(){
  long val;
  int params = webRequest->params();
  for( int i=0; i<params; i++ ){
    AsyncWebParameter* param = webRequest->getParam(i);
    if(param->isPost()){
#ifdef __DEBUG_40HZ_WEB__
      D_PRINTLN(true, "save_settings page received->POST[%s]: %s", param->name().c_str(), param->value().c_str());
#endif
      /// read form submit parameters and save settings
      ////////////////////////////////////////////////
      if( param->name() == "WIFI_SSID" ){
        strcpy(Settings.wifi_ssid,param->value().c_str());
      } else if( param->name() == "WIFI_PSW" ){
        strcpy(Settings.wifi_psw, param->value().c_str());
      } else if( param->name() == "AP_SSID" ){
        strcpy(Settings.ap_ssid, param->value().c_str());
      } else if( param->name() == "AP_PSW" ){
         strcpy(Settings.ap_psw, param->value().c_str());
      } else if ( param->name() == "LIGHT_FREQ" ){
        val = param->value().toInt();
        val = ( val < MIN_LIGHT_FREQ ) ? MIN_LIGHT_FREQ : val ;
        val = ( val > MAX_LIGHT_FREQ ) ? MAX_LIGHT_FREQ : val ;
        Settings.light_freq = val;
      } else if ( param->name() == "ON_TIME" ){
        val = param->value().toInt();
          // check min/max 
        val = ( val < MIN_ON_TIME ) ? MIN_ON_TIME : val ;
        val = ( val > MAX_ON_TIME ) ? MAX_ON_TIME : val ;  
        Settings.on_time =  val;
      } else if ( param->name() == "BRIGHTNESS" ){
        val =param->value().toInt();
          // check min/max 
        val = ( val < MIN_BRIGHTNESS ) ? MIN_BRIGHTNESS : val ;
        val = ( val > MAX_BRIGHTNESS ) ? MAX_BRIGHTNESS : val ;  
        Settings.brightness =  val;
      } else if ( param->name() == "PWM_FREQ" ){
        val = param->value().toInt();
        // check min/max 
        val = ( val < MIN_PWM_FREQ ) ? MIN_PWM_FREQ : val ;
        val = ( val > MAX_PWM_FREQ ) ? MAX_PWM_FREQ : val ;  
        Settings.pwm_freq =  val;
      } 
    } //if(p->isPost())
  } // for(int i=0;i<params;i++)
  return true;
}

void sendRebootingPage(){
  AsyncWebServerResponse* response = webRequest->beginResponse_P(200, FPSTR(TEXTHTML), (const uint8_t *)rebooting_html, rebooting_html_len);
  response->addHeader("Content-Encoding", "gzip");
  webRequest->send(response);
}

void sendIndexPage(){
  AsyncWebServerResponse* response = webRequest->beginResponse_P(200, FPSTR(TEXTHTML), (const uint8_t *)index_html, index_html_len);
  response->addHeader("Content-Encoding", "gzip");
  webRequest->send(response);
}