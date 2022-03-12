#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <stdint.h>
#include <EEPROM.h>
#include <CRC16.h>

#define SETTINGS_VERSION 0x00000010
#define DEFAULT_FLAG01 0x00

#ifndef WIFI_DEFAULT_SSID
#define WIFI_DEFAULT_SSID "SSID_NONE"
#endif

#ifndef WIFI_DEFAULT_PASSW
#define WIFI_DEFAULT_PASSW "12345678"
#endif

#define AP_DEFAULT_SSID "40Hz_Gen"
#define AP_DEFAULT_PASSW "12345678"
#define DEFAULT_LIGHT_FREQ 40
#define DEFAULT_ON_TIME 60
#define DEFAULT_BRIGHTNESS 100
#define DEFAULT_PWM_FREQ 1000






uint8_t SettingsRead();
uint8_t restoreDefaultSettings();
uint8_t SettingsWrite();
uint16_t SettingsCRC();

typedef union {
  uint8_t data;
  struct {
  uint8_t type : 3;
  uint8_t invert : 1;
  uint8_t spare4 : 1;
  uint8_t spare5 : 1;
  uint8_t spare6 : 1;
  uint8_t spare7 : 1;
  };
} flag01;

typedef struct {
  uint16_t      crc;                        // crc settings
  uint32_t      version;                    // Settings version  
  uint8_t       flag01;                     // Flag Byte Stored in Settings
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