#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <ESPAsyncWebServer.h>

// commands defines
#define CMD_NO_COMMANDS               0x0000
#define CMD_REBOOT                    0x0001
#define CMD_START                     0x0002
#define CMD_STOP                      0x0003
#define CMD_SAVE_SETTING_AND_REBOOT   0x0004
#define CMD_GET_SETTINGS_REQ_PARAMS   0x0005

// device status
typedef enum {
    STATUS_IDLE, STATUS_WORKING, STATUS_MENU
} deviceStatus_t ;

extern deviceStatus_t Status;
extern AsyncWebServerRequest *webRequest;
#endif