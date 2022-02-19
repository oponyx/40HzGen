#include "graphic.h"
#include "functions.h"
#include "settings.h"
#include "wifi.h"
#include "config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void display_init() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADRRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void dispReadyPage(){
  display.clearDisplay();
  display.setCursor(0,1);
  display.setTextSize(3);            
  display.println("Ready");
  display.display();
}

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

void dispWorkingPage(){
  display.clearDisplay();
  unsigned long rem_time = (Settings.on_time * 60 - ((millis() - start_millis) / 1000));
  String minutesSecondsRem;
  getMinutesAndSeconds(rem_time, minutesSecondsRem);
  display.setCursor(0,0);
  display.setTextSize(3);            
  display.println("Working");
  display.println(minutesSecondsRem);
  //Serial.println(String(rem_time) + " " + minutesSecondsRem);
  display.display();
}