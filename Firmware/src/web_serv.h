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

#define WEB_MODULE          "WEB"
#define WEB_METHOD_GET      "GET"
#define WEB_METHOD_POST     "POST"
#define WEB_SERVER_NAME "ESP Async Web Server"


extern AsyncWebServer web_server;

uint16_t web_serv_setup();
String status_req_handler();
String processor(const String& var);
void sendRebootingPage(AsyncWebServerRequest *webRequest);
void sendIndexPage(AsyncWebServerRequest *webRequest);

void handleRoot(AsyncWebServerRequest *request);
void handleStop(AsyncWebServerRequest *request);
void handleStart(AsyncWebServerRequest *request);
void handleReboot(AsyncWebServerRequest *request);
void handleSetBri(AsyncWebServerRequest *request);
void handleSetFreq(AsyncWebServerRequest *request);
void handleUpdateGET(AsyncWebServerRequest *request);
void handleUpdatePOST(AsyncWebServerRequest *request);
void handleGetSettings(AsyncWebServerRequest *request);
void handleDeviceStatus(AsyncWebServerRequest *request);
void handleSaveSettings(AsyncWebServerRequest *request);
void handleSettingsPage(AsyncWebServerRequest *request);
void handleUpdateProgress(AsyncWebServerRequest *request);
bool getSettingsRequestParam(AsyncWebServerRequest *request);
void firmwareUploadHandler(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);



#endif
