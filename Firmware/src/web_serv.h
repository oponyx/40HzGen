#ifndef _WEB_SERV_H_
#define _WEB_SERV_H_

#include <ESPAsyncWebServer.h>

extern AsyncWebServer web_server;
uint16_t web_serv_setup();
String processor(const String& var);
String status_req_handler();

#endif