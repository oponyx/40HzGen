#ifndef _WEB_SERV_H_
#define _WEB_SERV_H_

#include <ESPAsyncWebServer.h>

extern AsyncWebServer web_server;
void web_serv_setup();
String processor(const String& var);

#endif