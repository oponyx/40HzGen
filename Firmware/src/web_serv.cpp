#include <AsyncJson.h>
#include <ArduinoJson.h>

#include "web_serv.h"
#include "graphic.h"
#include "settings.h"
#include "hw_config.h"
#include "functions.h"

AsyncWebServer web_server(80); // the Generator web server

// index page
// don't need to escape '%' chars (this page is not scanned by the processor..)
const char index_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <meta charset=utf-8>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body {
        text-align: center;
        font-family: verdana,sans-serif;
        background: #252525;
      }
      html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
        color: azure;
      }
      h2 { font-size: 3.0rem; }
      p { font-size: 2.0rem; }
      button {
        border: 0;
        border-radius: 0.3rem;
        background: #1fa3ec;
        width: 100%;
        color: #faffff;
        line-height: 2.4rem;
        font-size: 1.2rem;
        transition-duration: 0.4s;
        cursor: pointer;
      }
      .units { font-size: 1.0rem; }
      .param-labels{
        font-size: 1.0rem;
        vertical-align:middle;
        padding-bottom: 15px;
      }
      button[disabled]:active, button[disabled],
      input[type="button"][disabled]:active,
      input[type="button"][disabled],
      input[type="submit"][disabled]:active,
      input[type="submit"][disabled] ,
      button[disabled]:hover,
      input[type="button"][disabled]:hover,
      input[type="submit"][disabled]:hover
      {
        border: 2px outset ButtonFace;
        color: GrayText;
        cursor: inherit;
        background-color: #ddd;
        background: #ddd;
      }
    </style>
  </head>
  <body>
    <h2>40Hz Generator</h2>
    <div>
      <table style="width: 100%"><tbody>
        <tr>
          <td style="width: 10%"></td>     
          <td  style="width: 40%">     
            <button disabled="true" id="BUTT_START" onclick="start()" class="button" style="background: #f50404; width: 100%">Start</Button>
          </td>
          <td style="width: 40%">     
            <button disabled="true" id="BUTT_STOP" onclick="stop()" class="button" style="background: #03df28;width: 100%">Stop</Button>
          </td>
          <td style="width: 10%"></td>     
        </tr>
      </tbody></table>
    </div>  
    <div>
        <p>Status:&nbsp;&nbsp;
        <span id="MESSAGE">Wait...</span></p>
    </div>  

    <table style="width: 100%">
      <tbody></tbody>
        <tr>
          <td>     
            <span class="param-labels">Light Status:</span> 
            <span id="LIGHT_STATUS">?</span>
        </td>
        <td>
            <span class="param-labels">Audio Status:</span> 
          <span id="AUDIO_STATUS">?</span>
        </td>
        <td>
          <span class="param-labels">Frequence:</span> 
          <span id="LIGHT_FREQ">?</span><span class="units">&nbspHz</span>
        </td>
      </tr>
      <tr>
        <td>
          <span class="param-labels">Rem. Time:</span> 
          <span id="REM_TIME">?</span><span class="units">&nbspSec</span>
        </td>
        <td>
          <span class="param-labels">UP TIME:</span> 
          <span id="UP_TIME">?</span>
        </td>
        <td>
          <span class="param-labels">Free Heap:</span> 
          <span id="FREE_HEAP">?</span>
        </td>
      </tr>  
    </tbody></table>
    <br>
    <table style="width: 100%"><tbody>
      <tr>
        <td style="width: 10%"></td>
        <td  style="width: 80%">     
          <form id=but1 style="display: block;" action='/settings' method='get'><button>Settings</button></form>
        </td>
        <td style="width: 10%"></td>
      </tr>
    <tr>
    </tbody></table>
    <div>
      <table style="width:100%">
        <tr>
          <td width="25%">OK</td>
          <td width="25%">UP</td>
          <td width="25%">DOWN</td>
          <td width="25%">CANCEL</td>
        </tr>
        <tr>
          <td id="OK_BUTTON">?</td>
          <td id="UP_BUTTON">?</td>
          <td id="DOWN_BUTTON">?</td>
          <td id="CANCEL_BUTTON">?</td>
        </tr>
      </table>
    </div>
    <script>
      function start(){
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            // get json data
            //json_msg=data;
            console.log("Received data:" + this.responseText);
            var json_msg = JSON.parse(this.responseText);
            if(json_msg.result=="OK"){
              document.getElementById("BUTT_START").disabled=true;
              document.getElementById("BUTT_STOP").disabled=false;
            }
          }
        };
        xhttp.open("GET", "/start", true);
        xhttp.send();
      }

      function stop(){
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            // get json data
            //json_msg=data;
            console.log("Received data:" + this.responseText);
            var json_msg = JSON.parse(this.responseText);
            if(json_msg.result=="OK"){
              document.getElementById("BUTT_START").disabled=false;
              document.getElementById("BUTT_STOP").disabled=true;
            }
          }
        };
        xhttp.open("GET", "/stop", true);
        xhttp.send();
      }

      setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            // get json data
            //json_msg=data;
            console.log("Received data:" + this.responseText);
            var json_msg = JSON.parse(this.responseText);
            document.getElementById("LIGHT_STATUS").innerHTML = json_msg.light_status;
            document.getElementById("REM_TIME").innerHTML = json_msg.rem_time;
            document.getElementById("AUDIO_STATUS").innerHTML = json_msg.audio_status;
            document.getElementById("LIGHT_FREQ").innerHTML  = json_msg.light_freq;
            document.getElementById("FREE_HEAP").innerHTML = json_msg.free_heap;
            document.getElementById("UP_TIME").innerHTML = json_msg.up_time;

            if(json_msg.working == true){
              document.getElementById("MESSAGE").innerHTML = "ON";
              document.getElementById("BUTT_START").disabled=true;
              document.getElementById("BUTT_STOP").disabled=false;
            }else{
              document.getElementById("MESSAGE").innerHTML = "Ready";
              document.getElementById("BUTT_START").disabled=false;
              document.getElementById("BUTT_STOP").disabled=true;
            }
            document.getElementById("OK_BUTTON").innerHTML = json_msg.okButtonStatus;
            document.getElementById("UP_BUTTON").innerHTML = json_msg.upButtonStatus;
            document.getElementById("DOWN_BUTTON").innerHTML = json_msg.downButtonStatus;
            document.getElementById("CANCEL_BUTTON").innerHTML = json_msg.cancelButtonStatus;
          }
        };
        xhttp.open("GET", "/device_status", true);
        xhttp.send();
      }, 3000 ) ;

    </script>
  </body>
</html>
)rawliteral";

// Rebooting HTML Page
const char rebooting_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
  </style>
</head>
<body>
  <h2>Rebooting, please wait, you'll be redirected...</h2>
  <script>
    $(document).ready(function () {
        window.setTimeout(function () {
            location.href = "/index.html";
        }, 5000);
    });  
  </script>
</body></html>
)rawliteral";

// Settings HTML Page
const char settings_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
      body {
        text-align: center;
        font-family: verdana,sans-serif;
        background: #252525;
      }
      html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
        color: azure;
      }
      h2 { font-size: 3.0rem; }
      p { font-size: 2.0rem; }
      button ,input[type=submit] {
        border: 0;
        border-radius: 0.3rem;
        background: #1fa3ec;
        width: 100%%;
        color: #faffff;
        line-height: 2.4rem;
        font-size: 1.2rem;
        transition-duration: 0.4s;
        cursor: pointer;
      }
      .units { font-size: 1.0rem; }
      .param-labels{
        font-size: 1.0rem;
        vertical-align: middle;
        padding-bottom: 0px;
    
  </style>
</head>
<body>
  <h2>40Hz Generator</h2>
  <p> Settings Page</p>
        <form action="/save_settings" method="POST">
          <span class="param-labels">WIFI SSID</span> 
          <input type="text" id="WIFI_SSID" name="WIFI_SSID" value="%WIFI_SSID%">
          <br>
          <span class="param-labels">WIFI PSW</span> 
          <input type="text" id="WIFI_PSW" name="WIFI_PSW" value="%WIFI_PSW%">
          <br>
          <span class="param-labels">AP SSID</span> 
          <input type="text" id="AP_SSID" name="AP_SSID" value="%AP_SSID%">
          <br>
          <span class="param-labels">AP PSW</span> 
          <input type="text" id="AP_PSW" name="AP_PSW" value="%AP_PSW%">
          <br>
          <label for="LIGHT_FREQ" class="param-labels">LIGHT_FREQ</label>
          <input type="number" id="LIGHT_FREQ" name="LIGHT_FREQ" min="5" max="100" value="%LIGHT_FREQ%"><span class="units">Hz</span>
          <br>
          <label for="ON_TIME" class="param-labels">ON TIME</label>
          <input type="number" id="ON_TIME" name="ON_TIME" min="2" max="100" value="%ON_TIME%"><span class="units">Min</span>
          <br>
          <br>

        <input type="submit" value="Submit" style="width:80%%;">
  </form> 
  <br>
  <br>
  <table style="width: 100%%"><tbody>
    <tr>
      <td style="width: 10%%"></td>
      <td  style="width: 80%%">     
        <form id=but1 style="display: block; width: 100%%;" action='/index.html' method='get'><button>Back to Main Page</button></form>
      </td>
      <td style="width: 10%%"></td>
    </tr>
      <td style="width: 10%%"></td>
      <td  style="width: 80%%">     
        <form id=but2 style="display: block;" action=''><button style="background: #f50404;" onclick="reboot()">Reboot device</button></form>
      </td>
      <td style="width: 10%%"></td>
    </tr>
  </tbody></table>
  <script>
      function reboot(){
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            // get json data
            //json_msg=data;
            console.log("Received data:" + this.responseText);
            var json_msg = JSON.parse(this.responseText);
            if(json_msg.result=="OK"){
              alert("System is rebooting.\n\nPlease wait...");
            }
          }
        };
        xhttp.open("GET", "/reboot", true);
        xhttp.send();
      }
  </script>
</body>
</html>
)rawliteral";

//          <span class="param-labels">ON TIME</span> 
//          <input type="text" id="ON_TIME" name="ON_TIME" value="%ON_TIME%"><span class="units">Min</span>

//          <span class="param-labels">LIGHT FREQ</span> 
//          <input type="text" id="LIGHT_FREQ" name="LIGHT_FREQ" value="%LIGHT_FREQ%"><span class="units">Hz</span>

/**
 * @brief Function to init the web server
 * 
 */
void web_serv_setup(){
    web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
       Serial.printf("WEB: HTTP-GET request '/' from %s\n", request->client()->remoteIP().toString().c_str());
      request->send_P(200, "text/html", index_page);
    });
    web_server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.printf("WEB: HTTP-GET request '/index.html' from %s\n", request->client()->remoteIP().toString().c_str());
      request->send_P(200, "text/html", index_page);
    });
    web_server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
       Serial.printf("WEB: HTTP-GET request '/settings' from %s\n", request->client()->remoteIP().toString().c_str());
       request->send_P(200, "text/html", settings_page, processor);
    });
    web_server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
       Serial.printf("WEB: HTTP-GET request '/reboot' from %s\n", request->client()->remoteIP().toString().c_str());
      //request->send_P(200, "text/html", rebooting_page);
      AsyncJsonResponse * resp = new AsyncJsonResponse();
      resp->addHeader("Server", "ESP Async Web Server");
      JsonObject root = resp->getRoot();
      root["result"] = "OK";
      root["Error_code"] = 0;
      root["Error_message"] = "No Errors";
      resp->setLength();
      request->send(resp);
      Serial.printf("WEB sent response\n");
      Command = CMD_REBOOT; //reboot request
    });
    web_server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.printf("WEB: HTTP-GET request '/start' from %s\n", request->client()->remoteIP().toString().c_str());
      AsyncJsonResponse * resp = new AsyncJsonResponse();
      resp->addHeader("Server", "ESP Async Web Server");
      JsonObject root = resp->getRoot();
      root["result"] = "OK";
      root["Error_code"] = 0;
      root["Error_message"] = "No Errors";
      resp->setLength();
      request->send(resp);
      Serial.printf("WEB sent response\n");
      Command = CMD_START; // Start command
     });
    web_server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.printf("WEB: HTTP-GET request '/stop' from %s\n", request->client()->remoteIP().toString().c_str());
      AsyncJsonResponse * resp = new AsyncJsonResponse();
      resp->addHeader("Server", "ESP Async Web Server");
      JsonObject root = resp->getRoot();
      root["result"] = "OK";
      root["Error_code"] = 0;
      root["Error_message"] = "No Errors";
      resp->setLength();
      request->send(resp);
      Serial.printf("WEB sent response\n");
      Command = CMD_STOP; // Stop command
    });
    // send status json message
    web_server.on("/device_status", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.printf("WEB: HTTP-GET request '/device_status' from %s\n", request->client()->remoteIP().toString().c_str());
      AsyncJsonResponse * resp = new AsyncJsonResponse();
      resp->addHeader("Server", "ESP Async Web Server");
      JsonObject root = resp->getRoot();
      root["working"] = bWorking;
      root["light_status"] = (bWorking == true) ? "ON" : "OFF";
      root["rem_time"] = (bWorking == true) ? rem_time : 0;
      root["audio_status"] = "unknown";
      root["light_freq"] = Settings.light_freq;
      root["free_heap"] = ESP.getFreeHeap();
      root["WiFi_ssid"] = WiFi.SSID();
      root["up_time"] = millis();
      root["okButtonStatus"] = okButtonStatus;
      root["cancelButtonStatus"] = cancelButtonStatus;
      root["upButtonStatus"] = upButtonStatus;
      root["downButtonStatus"] = downButtonStatus;

      resp->setLength();
      request->send(resp);
    });

    // save settings on form submit
    web_server.on("/save_settings", HTTP_POST, [](AsyncWebServerRequest * request){
      long val;

      Serial.printf("WEB: HTTP-GET request '/save_settings' from %s\n", request->client()->remoteIP().toString().c_str());
      bool bSettingsChanged = false;
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isFile()){ //p->isPost() is also true
          Serial.printf("save_settings page received->FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
        } else if(p->isPost()){
          Serial.printf("save_settings page received->POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            /// read form submit parameters and save settings
            ////////////////////////////////////////////////
            if( p->name() == "WIFI_SSID" ){
              strcpy(Settings.wifi_ssid,p->value().c_str());
              bSettingsChanged=true;
            } else if( p->name() == "WIFI_PSW" ){
              strcpy(Settings.wifi_psw,p->value().c_str());
              bSettingsChanged=true;
            } else if( p->name() == "AP_SSID" ){
              strcpy(Settings.ap_ssid,p->value().c_str());
              bSettingsChanged=true;
            } else if( p->name() == "AP_PSW" ){
              strcpy(Settings.ap_psw,p->value().c_str());
              bSettingsChanged=true;
            } else if ( p->name() == "LIGHT_FREQ" ){
              val = p->value().toInt();
              // check min/max 
              val = ( val < MIN_LIGHT_FREQ ) ? MIN_LIGHT_FREQ : val ;
              val = ( val > MAX_LIGHT_FREQ ) ? MAX_LIGHT_FREQ : val ;
              Settings.light_freq = val;
              bSettingsChanged=true;
            } else if ( p->name() == "ON_TIME" ){
              val = p->value().toInt();
              // check min/max 
              val = ( val < MIN_ON_TIME ) ? MIN_ON_TIME : val ;
              val = ( val > MAX_ON_TIME ) ? MAX_ON_TIME : val ;  
              Settings.on_time =  val;
              bSettingsChanged=true;
            }
            ////////////////////////////////////////////////
        } else {
          Serial.printf("save_settings page received->GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      } // for(int i=0;i<params;i++)
      request -> send(200, "text/html", rebooting_page);
      if(bSettingsChanged){
        Command = CMD_SAVE_SETTING_AND_REBOOT;
      }
    }); // server.on
    // start the server
    web_server.begin();
}

/**
 * @brief processor function to set the fields in HTML pages
 * 
 * @param var 
 * @return String 
 */
String processor(const String& var){
  //Serial.println(var);
  if(var == "WIFI_SSID"){
    return String(Settings.wifi_ssid);
  }
  else if(var == "WIFI_PSW"){
    return String(Settings.wifi_psw);
  }
  else if(var == "AP_SSID"){
    return String(Settings.ap_ssid);
  }
  else if(var == "AP_PSW"){
    return String(Settings.ap_psw);
  }
  else if(var == "LIGHT_FREQ"){
    return String(Settings.light_freq);
  }
  else if(var == "ON_TIME"){
    return String(Settings.on_time);
  }
  return String();
}
