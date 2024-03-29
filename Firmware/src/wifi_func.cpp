/*
wifi.cpp - wifi setup functions 

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

#include "wifi_func.h"
#include "settings.h"
#include "globals.h"
#include "log.h"
#include "hw_config.h"

/**
 * @brief Call this function to start the AP mode
 * 
 * @return false if AP failed to start
 * @return true if AP started
 */
bool AP_setup(){
    IPAddress local_IP(192,168,16,4);
    IPAddress gateway(192,168,16,254);
    IPAddress subnet(255,255,255,0);

    if(!WiFi.softAPConfig(local_IP, gateway, subnet)){
        return false;
    }
    if(!WiFi.softAP(Settings.ap_ssid, Settings.ap_psw)){
        return false;
    }
    return true;
}

/**
 * @brief Call this function to start WIFI STAT mode
 * 
 * @return false if WIFI failed to connect
 * @return true if WIFI connected 
 */
bool wifi_setup(){
#ifdef STATUS_LED
        pinMode(STATUS_LED, OUTPUT);
#endif
    WiFi.begin(Settings.wifi_ssid, Settings.wifi_psw);
    for (int iRetry=0; iRetry<50; iRetry++)
    // while (WiFi.status() != WL_CONNECTED) 
    {
        if((WiFi.status() == WL_CONNECTED)){
            return true;
        }
        delay(250);
        D_PRINT(false, ".");
#ifdef STATUS_LED
        digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
#endif

    }
    //Serial.println("");
    return false;
}
