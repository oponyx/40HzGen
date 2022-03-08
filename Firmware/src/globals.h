#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// commands defines
#define CMD_NO_COMMANDS               0x0000
#define CMD_REBOOT                    0x0001
#define CMD_START                     0x0002
#define CMD_STOP                      0x0003
#define CMD_SAVE_SETTING_AND_REBOOT   0x0004

// device status
typedef enum {
    STATUS_IDLE, STATUS_WORKING, STATUS_MENU
} deviceStatus_t ;

extern deviceStatus_t Status;
#endif