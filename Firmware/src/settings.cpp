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
  Settings.flag01 = DEFAULT_FLAG01;
//  ((String)WIFI_DEFAULT_SSID).toCharArray(Settings.wifi_ssid,sizeof(WIFI_DEFAULT_SSID));
  strcpy(Settings.wifi_ssid,WIFI_DEFAULT_SSID);
//  ((String)WIFI_DEFAULT_PASSW).toCharArray(Settings.wifi_psw,sizeof(WIFI_DEFAULT_PASSW));
  strcpy(Settings.wifi_psw, WIFI_DEFAULT_PASSW);
//  ((String)AP_DEFAULT_SSID).toCharArray(Settings.ap_ssid,sizeof(AP_DEFAULT_SSID));
  strcpy(Settings.ap_ssid, AP_DEFAULT_SSID);
//  ((String)AP_DEFAULT_PASSW).toCharArray(Settings.ap_psw,sizeof(AP_DEFAULT_PASSW));
  strcpy(Settings.ap_psw, AP_DEFAULT_PASSW);
  Settings.light_freq = DEFAULT_LIGHT_FREQ;
  Settings.on_time = DEFAULT_ON_TIME;
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

