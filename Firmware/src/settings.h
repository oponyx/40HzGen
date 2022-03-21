/*
settings.h

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
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <stdint.h>
#include <EEPROM.h>
#include <CRC16.h>

#define SETTINGS_VERSION 0x00000010

#ifdef WIFI_DEFAULT_SSID_OVERRIDE
#define WIFI_DEFAULT_SSID WIFI_DEFAULT_SSID_OVERRIDE
#else 
#define WIFI_DEFAULT_SSID "SSID_NONE"
#define WIFI_SSID_NOT_CONFIGURED "NO-WIFI"
#endif

#ifdef WIFI_DEFAULT_PASSW_OVERRIDE
#define WIFI_DEFAULT_PASSW WIFI_DEFAULT_PASSW_OVERRIDE
#else
#define WIFI_DEFAULT_PASSW "12345678"
#endif

#ifdef AUTOSTART_OVERRIDE
#define AUTOSTART AUTOSTART_OVERRIDE
#else
#define AUTOSTART false
#endif

#define AP_DEFAULT_SSID "40Hz_Gen"
#define AP_DEFAULT_PASSW "12345678"
#define DEFAULT_LIGHT_FREQ 40
#ifndef DEFAULT_ON_TIME
#define DEFAULT_ON_TIME 60
#endif
#define DEFAULT_BRIGHTNESS 100
#define DEFAULT_PWM_FREQ 1000






uint8_t SettingsRead();
uint8_t restoreDefaultSettings();
uint8_t SettingsWrite();
uint16_t SettingsCRC();

typedef union {
  uint8_t data;
  struct {
  uint8_t type      : 3;
  uint8_t autostart : 1;
  uint8_t spare1    : 1;
  uint8_t spare2    : 1;
  uint8_t spare3    : 1;
  uint8_t spare4    : 1;
  };
} flags;

typedef struct {
  uint16_t      crc;                        // crc settings
  uint32_t      version;                    // Settings version  
  flags         settingFlags;                      // Flag Byte Stored in Settings
  char          wifi_ssid[24];              // Wifi SSID Stored in Setting
  char          wifi_psw[24];               // Wifi Password Stored in Setting
  char          ap_ssid[16];                // Access Poin Mode SSID name Stored in Setting
  char          ap_psw[16];                 // Access Poin Mode Password Stored in Setting
  uint8_t       light_freq;                 // Gamma Frequence Stored in Setting
  uint8_t       on_time;                    // OnTime Stored in Setting
  uint8_t       brightness;                 // light brightness 10-100%
  uint32_t      pwm_freq;                   // light pwm frequence
} TSettings;

extern TSettings Settings;


#endif