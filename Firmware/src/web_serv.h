/*
web_server.h - 40HzGen functions definitions and HTML pages for the web server 

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
#ifndef _WEB_SERV_H_
#define _WEB_SERV_H_

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

#define WEB_MODULE          "WEB"
#define WEB_METHOD_GET      "GET"
#define WEB_METHOD_POST     "POST"
#define WEB_SERVER_NAME     "ESP Async Web Server"


extern AsyncWebServer web_server;

uint16_t web_serv_setup();
String status_req_handler();
String processor(const String& var);
void sendRebootingPage();
void sendIndexPage();
void onNotFound(AsyncWebServerRequest *request);
void handleRoot(AsyncWebServerRequest *request);
void handleUpdateGET(AsyncWebServerRequest *request);
void handleUpdatePOST(AsyncWebServerRequest *request);
void handleGetSettings(AsyncWebServerRequest *request);
void handleDeviceStatus(AsyncWebServerRequest *request);
void handleSaveSettings(AsyncWebServerRequest *request);
void handleSettingsPage(AsyncWebServerRequest *request);
void handleUpdateProgress(AsyncWebServerRequest *request);
bool getSettingsRequestParam();
void firmwareUploadHandler(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
void sendJsonError(AsyncWebServerRequest* request, String result, uint16_t err_code, String error_message);
void parseCommand(AsyncWebServerRequest *request);
void onCmnd(AsyncWebServerRequest * request);

#endif
