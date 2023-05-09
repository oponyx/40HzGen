/*
globals.cpp - globals variables

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
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <ESPAsyncWebServer.h>

#define __DEBUG_40HZ_WEB__
#define __DEBUG_BUTTONS__

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
extern uint16_t LightPWMDutyVal;
#endif
