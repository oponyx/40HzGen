/*
graphic.cpp - functions for the thisplay

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

#ifdef LCD_POPULATED

#include "graphic.h"
#include "functions.h"
#include "settings.h"
#include "wifi_func.h"
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

const uint8_t main_menu_items_count = 2;
const char* main_menu_items[] = {
    "Info",
    "Settings"
};

const uint8_t settings_menu_items_count = 12;
const char* settings_menu_items[] = {
    "wifi ssid",
    "wifi psw",
    "ap ssid",
    "ap psw",
    "light freq",
    "on time",
    "brightness",
    "pwm_freq",
    "autostart",
    "wifiEnabled",
    "lightEnabled",
    "audioEnabled"
};

int8_t menu_page      = -1;
uint8_t item_selected = 0;
int8_t item_entered   = -1;
bool okPressed        = false;
bool cancelPressed    = false;
bool upPressed        = false;
bool downPressed      = false;

void menu(){
  if(upPressed) {
    item_selected = item_selected - 1;
    upPressed = false;
  };
  
  if(downPressed) {
    if (item_selected > 0){
    item_selected = item_selected + 1;
    downPressed = false;
    }
  };

  if(okPressed) {
    item_entered = item_selected;
    okPressed = false;
  };

  if(cancelPressed) {
    item_entered = -1;
    cancelPressed = false;
  };

  if(item_entered == -1) { // main
    display.clearDisplay();
    display.setTextSize(1);             
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
//    display.println(F("Menu"));
//    display.println("");
    for(int i=0;i < settings_menu_items_count; i++) {
      if(i == item_selected) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println(settings_menu_items[i]);
      } else if(i != item_selected){
        display.setTextColor(SSD1306_WHITE);
        display.println(settings_menu_items[i]);
      }
    }
  }
  display.display();

}

void menuCancelPressed(){
  cancelPressed = true;
}

void menuOkPressed(){
  okPressed = true;
}
void menuUpPressed(){
  upPressed = true;
}

void menuDownPressed(){
  downPressed = true;
}

#endif
