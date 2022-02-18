#include <ArduinoJson.h>

#include "web_serv.h"
#include "graphic.h"
#include "settings.h"

AsyncWebServer web_server(80);

const char index_page[] PROGMEM = R"rawliteral(
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
    p { font-size: 2.0rem; }
  </style>
</head>
<body>
  <h2>40Hz Generator</h2>
  <p>Status page</p>
  <span class="param-labels">Lamp Status:</span> 
  <span id="LAMP_SATATUS">%LAMP_SATATUS%</span>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // get json data
      json_msg=data;
      console.log("Received data:" + data);
    }
  };
  xhttp.open("GET", "/device_status", true);
  xhttp.send();
}, 10000 ) ;

</script>
</html>
)rawliteral";

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
  <h2>Rebooting, please wait then go back...</h2>
</body></html>
)rawliteral";

const char settings_page[] PROGMEM = R"rawliteral(
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
    .units { font-size: 1.2rem; }
    .param-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>40Hz Generator Settings Page</h2>
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
    <span class="param-labels">LIGHT FREQ</span> 
    <input type="text" id="LIGHT_FREQ" name="LIGHT_FREQ" value="%LIGHT_FREQ%">
    <br>
    <span class="param-labels">AUDIO FREQ</span> 
    <input type="text" id="AUDIO_FREQ" name="AUDIO_FREQ" value="%AUDIO_FREQ%">
    <br>
    
    <br><br>
    <input type="submit" value="Submit">
  </form> 
</body>
</html>)rawliteral";


/*
const char HTML_Page[] PROGMEM = R"rawliteral(
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
    .units { font-size: 1.2rem; }
    .param-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>40Hz Generator Settings Page</h2>
  <p>
    <span class="param-labels">WIFI SSID</span> 
    <span id="WIFI_SSID">%WIFI_SSID%</span>
  </p>
  <p>
    <span class="param-labels">WIFI PASSWORD</span>
    <span id="WIFI_PSW">%WIFI_PSW%</span>
  </p>
  <p>
    <span class="param-labels">AP SSID</span> 
    <span id="AP_SSID">%AP_SSID%</span>
  </p>
  <p>
    <span class="param-labels">AP PASSWORD</span>
    <span id="AP_PSW">%AP_PSW%</span>
  </p>
  <p>
    <span class="param-labels">Light Freq</span>
    <span id="LIGHT_FREQ">%LIGHT_FREQ%</span>
    <sup class="units">Hz</sup>
  </p>
  <p>
    <span class="param-labels">Audio Freq</span>
    <span id="AUDIO_FREQ">%AUDIO_FREQ%</span>
    <sup class="units">Hz</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("WIFI_SSID").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/wifi_ssid", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("WIFI_PSW").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/wifi_psw", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("AP_SSID").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ap_ssid", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("AP_PSW").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ap_psw", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LIGHT_FREQ").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/light_freq", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("AUDIO_FREQ").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/audio_freq", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";
*/

String status_req_handler() {
  String json_message;
  
  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<500> jsonBuffer;

  // Create the root object
  JsonObject& root = jsonBuffer.createObject();

  root["LampStatus"] = digitalRead(A0);   //Put the output status
  root["LampTimeLeft"] = digitalRead(0);  //
  root["LampTotalTime"] = digitalRead(0); //

  root.printTo(json_message);  //Store JSON in String variable
  web_server.send(200, "text/html", json_message);
}


void web_serv_setup(){
    web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_page, processor);
    });
    web_server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_page, processor);
    });
    web_server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", settings_page, processor);
    });
    // send status json message
    web_server.on("/device_status", HTTP_GET, status_req_handler){
        //request->send_P(200, "text/plain", String(Settings.wifi_ssid).c_str());
    });
    // save settings on form submit
    web_server.on("/save_settings", HTTP_POST, [](AsyncWebServerRequest * request){
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
              Settings.light_freq =  p->value().toInt();
              bSettingsChanged=true;
            } else if ( p->name() == "AUDIO_FREQ" ){
              Settings.audio_freq =  p->value().toInt();
              bSettingsChanged=true;
            }
            ////////////////////////////////////////////////
        } else {
          Serial.printf("save_settings page received->GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      } // for(int i=0;i<params;i++)
      request -> send(200, "text/plain", rebooting_page);
      if(bSettingsChanged){
        SettingsWrite();
        system_restart();
      }
    }); // server.on

 
    web_server.begin();
}


// Replaces placeholder with DHT values
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
  else if(var == "AUDIO_FREQ"){
    return String(Settings.audio_freq);
  }
  return String();
}
