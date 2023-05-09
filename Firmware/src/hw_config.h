/*
hw_config.h - hardware configuration file

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
#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_

#include <Arduino.h>
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
#if !defined(ARDUINO_ESP8266_WEMOS_D1MINI) && !defined(ARDUINO_ESP8266_ESP01) && !defined(ARDUINO_ESP32_DEV)
#error "The board is not defined!"
#endif

#ifdef ARDUINO_ESP32_DEV
#define CANCEL_BUTTON_PIN       13  // D7 
#define OK_BUTTON_PIN           0  // 
#define UP_BUTTON_PIN           16  // D0
#define DOWN_BUTTON_PIN         12  // D6

#define OK_BUTT_ACTIVE_LVL      LOW // buttons active when low
#define CANCEL_BUTT_ACTIVE_LVL  LOW
#define UP_BUTT_ACTIVE_LVL      LOW
#define DOWN_BUTT_ACTIVE_LVL    LOW

#define LIGHT_OUT_PIN           GPIO_NUM_19  // 
#define LIGHT_OUT_ACTIVE_LVL    HIGH
#define LIGHT_PWM_CH            0
#define LIGHT_PWM_RES_BITS      8

#define AUDIO_OUT_PIN           GPIO_NUM_18   // 
#define AUDIO_OUT_ACTIVE_LVL    HIGH
#define AUDIO_PWM_CH            1
#define AUDIO_PWM_RES_BITS      8
#define AUDIO_FREQ              1000

#define STATUS_LED              2
#define STATUS_LED_ACTIVE_LVL   HIGH
#endif
// HW config for D1Mini And ESP12F
#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
#define CANCEL_BUTTON_PIN       D7  // D7 
#define OK_BUTTON_PIN           D5  // D5
#define UP_BUTTON_PIN           D0  // D0
#define DOWN_BUTTON_PIN         D6  // D6

#define OK_BUTT_ACTIVE_LVL      LOW // buttons active when low
#define CANCEL_BUTT_ACTIVE_LVL  LOW
#define UP_BUTT_ACTIVE_LVL      LOW
#define DOWN_BUTT_ACTIVE_LVL    LOW

#define LIGHT_OUT_PIN           D8  // D8
#define LIGHT_OUT_ACTIVE_LVL    HIGH
#define AUDIO_OUT_PIN           D6   
#define AUDIO_OUT_ACTIVE_LVL    HIGH
#define TONE_FREQ               1440
#define STATUS_LED              LED_BUILTIN
#define STATUS_LED_ACTIVE_LVL   HIGH

#endif //ARDUINO_ESP8266_WEMOS_D1MINI

// HW config for ESP01
#ifdef ARDUINO_ESP8266_ESP01
#define OK_BUTTON_PIN           0

#define OK_BUTT_ACTIVE_LVL      LOW // buttons active when low

#define LIGHT_OUT_PIN           2 
#define LIGHT_OUT_ACTIVE_LVL    LOW

#ifdef  STATUS_LED             
#undef STATUS_LED
#endif
#endif // ARDUINO_ESP8266_ESP01


#endif 
