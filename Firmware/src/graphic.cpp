/*
graphic.cpp - functions for the thisplay


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
#include "graphic.h"
#include "functions.h"
#include "settings.h"
#include "wifi.h"
#include "config.h"
#include "logo.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/**
 * @brief LCD Init function
 * 
 */
bool display_init() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADRRESS, false)) { 
    return true;
  }
  return false;
}

/**
 * @brief Display the 'Ready Page' on the LCD
 * 
 */
void dispReadyPage(){
  display.clearDisplay();
  display.setCursor(0,1);
  display.setTextSize(3);            
  display.println("Ready");
  display.display();
}

/**
 * @brief Displays the 'Info Page' on the LCD
 * 
 */
void dispInfoPage(){
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.printf("AP IP:%s\n", WiFi.softAPIP().toString().c_str());
  display.printf("AP SSID:%s\n", Settings.ap_ssid);
  display.printf("WIFI SSID:%s\n", Settings.wifi_ssid);
  display.printf("WIFI IP:%s\n", WiFi.localIP().toString().c_str());
  display.printf("Light freq:%d Hz\n", Settings.light_freq);
  display.printf("Light timer:%d min.\n", Settings.on_time);
  display.display();
}

/**
 * @brief Displays the 'Working Page' on the LCD
 * 
 */
void dispWorkingPage(){
  display.clearDisplay();
//  rem_time = (Settings.on_time * 60 - ((millis() - start_millis) / 1000));
//  String minutesSecondsRem;
//  getMinutesAndSeconds(rem_time, minutesSecondsRem);
  display.setCursor(0,0);
  display.setTextSize(3);            
  display.println("Working");
//  display.println(minutesSecondsRem);
  display.display();
}

void dispLogoPage(){
  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}