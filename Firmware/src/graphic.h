#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include <SPI.h>
#include <Wire.h>


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
extern Adafruit_SSD1306 display;

#define OLED_I2C_ADRRESS 0x3c

#define LOGO_HEIGHT   64
#define LOGO_WIDTH    128


// '40hzGen', 128x64px


void display_init() ;


#endif // _GRAPHIC_H_