/*
settings.cpp - function for save and load 40HzGen settings


copyright Copyright (c) 2022 Onofrio Pagliarulo (oponyx@hotmail.it)

MIT License

Copyright (c) 2022 Pagliarulo Onofrio

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
#include "config_override.h"
#include "settings.h"
#include "log.h"
#include "error_codes.h"


// Struct containing the settings.
// Use SettingsWrite() and SettingsRead() to save and load Settings to and from Flash
TSettings Settings; 
 

/**
 * @brief Call this function to load Settings from Flash Memory
 * 
 * @return uint8_t 
 */
uint8_t SettingsRead(){
  EEPROM.begin(sizeof(TSettings));
  EEPROM.get(0,Settings);
  EEPROM.end();

  if(Settings.crc != SettingsCRC()){
      // Serial.println("CRC Settings doesn't match!!");
      return ERROR_SETTINGS_CRC;
  }
  //Serial.println("Configuration loaded");
  return NO_ERRORS;
}

/**
 * @brief Call this function to save Settings to the Flash Memory
 * 
 * @return uint8_t 
 */
uint8_t SettingsWrite(){
  EEPROM.begin(sizeof(TSettings));

  //add settings crc
  Settings.crc = SettingsCRC();
  EEPROM.put(0,Settings);
  EEPROM.commit();
  EEPROM.end();

  //Serial.println("Configuration saved");
  return NO_ERRORS;
}

/**
 * @brief Call this function to restore default settings to the Flash Memory
 * 
 * @return uint8_t 
 */
uint8_t restoreDefaultSettings(){
  Settings.version = SETTINGS_VERSION;
  Settings.settingFlags.autostart = AUTOSTART;
  strcpy(Settings.wifi_ssid,WIFI_DEFAULT_SSID);
  strcpy(Settings.wifi_psw, WIFI_DEFAULT_PASSW);
  strcpy(Settings.ap_ssid, AP_DEFAULT_SSID);
  strcpy(Settings.ap_psw, AP_DEFAULT_PASSW);
  Settings.light_freq = DEFAULT_LIGHT_FREQ;
  Settings.on_time = DEFAULT_ON_TIME;
  Settings.brightness = DEFAULT_BRIGHTNESS;
  Settings.pwm_freq = DEFAULT_PWM_FREQ;
  Settings.crc = SettingsCRC();
  SettingsWrite();
  return NO_ERRORS;
}

/**
 * @brief Calculates the CRC16 on the settings struct
 * 
 * @return uint16_t 
 */
uint16_t SettingsCRC(){
  CRC16 SettingsCRC; 
  char* buffer = reinterpret_cast<char*>(&Settings);
  for( uint16_t address = sizeof(Settings.crc); address < sizeof(TSettings); address++ ){
    SettingsCRC.add(buffer[address]);
  }
  return SettingsCRC.getCRC();
}

