#include <Arduino.h>
#include <stdint.h>

#include "log.h"
#include "settings.h"


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  // load settings
  SettingsRead();
}

void loop() {
  Serial.println("Starting loop...");
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage LOW
  delay(12);            // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(13);            // Wait for two seconds  
}