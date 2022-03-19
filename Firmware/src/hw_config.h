#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_

/*
D0  16
D1  5   SCL
D2  4   SDA
D3  0
D4  2
D5  14
D6  12
D7  13
D8  15
A0  17
TX  1
RX  3

*/

// HW config for D1Mini And ESP12F
#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
#define LCD_POPULATED
#define CANCEL_BUTTON_PIN       13  // D7 
#define OK_BUTTON_PIN           14  // D5
#define UP_BUTTON_PIN           16  // D0
#define DOWN_BUTTON_PIN         12  // D6

#define OK_BUTT_ACTIVE_LVL      LOW // buttons active when low
#define CANCEL_BUTT_ACTIVE_LVL  LOW
#define UP_BUTT_ACTIVE_LVL      LOW
#define DOWN_BUTT_ACTIVE_LVL    LOW

#define LIGHT_OUT_PIN           15  // D8
#define AUDIO_OUT_PIN           4   // D2
#endif //ARDUINO_ESP8266_WEMOS_D1MINI

// HW config for ESP01
#ifdef ARDUINO_ESP8266_ESP01
#define OK_BUTTON_PIN           0

#define OK_BUTT_ACTIVE_LVL      LOW // buttons active when low

#define LIGHT_OUT_PIN           2  
#endif // ARDUINO_ESP8266_ESP01


#endif 
