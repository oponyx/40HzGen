#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <stdint.h>
#include <EEPROM.h>

void SettingsRead();

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
  uint16_t      crc;                       // 000
  uint32_t      version;                   // 002  
  uint8_t       flag01;                    // 006
  char          wifi_ssid[16];             // 007
  char          wifi_psw[16];              // 017
  uint16_t      light_freq;                // 027
  uint16_t      audio_freq;                // 029
} TSettings;




#endif