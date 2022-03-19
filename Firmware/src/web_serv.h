#ifndef _WEB_SERV_H_
#define _WEB_SERV_H_

#include <ESPAsyncWebServer.h>

extern AsyncWebServer web_server;
uint16_t web_serv_setup();
String processor(const String& var);
String status_req_handler();
bool getSettingsRequestParam(AsyncWebServerRequest *request);

#define WEB_SERVER_NAME "ESP Async Web Server"
// index page
// doesn't need to escape '%' chars (this page is not scanned by the processor..)
const char index_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <meta charset=utf-8>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      table, th, td {
        border: 1px solid;
      }

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
      h2 { font-size: 3.0rem; margin-block-start: 0.5em; margin-block-end: 0.5em;}
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
      .button-green {
        border: 0;
        border-radius: 0.3rem;
        background: #05ec24;
        width: 100%;
        color: #faffff;
        line-height: 2.4rem;
        font-size: 1.2rem;
        transition-duration: 0.4s;
        cursor: pointer;
      }
      .button-red {
        border: 0;
        border-radius: 0.3rem;
        background: #ee0909;
        width: 100%;
        color: #faffff;
        line-height: 2.4rem;
        font-size: 1.2rem;
        transition-duration: 0.4s;
        cursor: pointer;
      }
      .units { font-size: 0.7rem; }
      .param-labels{
        font-size: 1.0rem;
        vertical-align:middle;
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
    <center>
      <div style="width: 80%">  
        <h2>40Hz Generator</h2>
        <div>
          <table style="width: 100%"><tbody>
            <tr>
            <td style="width: 10%"></td>     
            <td  style="width: 40%">     
                <button disabled="true" id="BUTT_START" onclick="start()" class="button-green" style="width: 100%">Start</Button>
            </td>
            <td style="width: 40%">     
                <button disabled="true" id="BUTT_STOP" onclick="stop()" class="button-red" style="width: 100%">Stop</Button>
            </td>
            <td style="width: 10%"></td>     
            </tr>
          </tbody></table>
        </div>  
        <div>
            <p>Status:&nbsp;&nbsp;
            <span id="MESSAGE">Wait...</span></p>
        </div>  

      <table style="width: 100%;"><tbody>
        <tr>
            <td colspan="3" style="text-align:center";>     
                <span class="param-labels">Brightness:</span><span class="param-labels" id="BRIGHTNESS">val</span><span class="units">&nbsp%</span>
            </td>
        </tr>
        <tr>
            <td colspan="3" style="text-align:center";>
                <input style="width: 100%;" type="range" id="BRIGHTNESS_SLIDER" name="BRIGHTNESS_SLIDER" min="1" max="100" >
            </td>
        </tr>
        <tr>
            <td colspan="3" style="text-align:center";>
                <button id="BUTT_SUBMIT_BRI" disabled="true" onclick="set_bri()">Submit Bri</button>
            </td>
        </tr>
        <tr>
            <td colspan="3" style="text-align:center";>     
                <span class="param-labels">Frequence:</span><span class="param-labels" id="FREQUENCE">val</span><span class="units">&nbspHz</span>
            </td>
        </tr>
        <tr>
            <td colspan="3" style="text-align:center";>
                <input style="width: 100%;" type="range" id="FREQUENCE_SLIDER" name="FREQUENCE_SLIDER" min="1" max="100" >
            </td>
        </tr>
        <tr>
            <td colspan="3" style="text-align:center";>
                <button id="BUTT_SUBMIT_FREQ" disabled="true" onclick="set_freq()">Submit Freq</button>
            </td>
        </tr>
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
          <span class="param-labels">Error:</span> 
          <span id="LIGHT_ERROR">?</span><span class="units">&nbspuS</span>
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
        <tr>
          <td  colspan="3">     
          <form id=but1 style="display: block;" action='/settings' method='get'><button>Settings</button></form>
          </td>
        </tr>
      </tbody></table>
      <div>
        <table style="width:100%; display:none" >
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
    </center>
    <br>
    <div class="units">
      <span>Wifi ip:<span id="WIFI_IP"></span></span>
    </div>
    </body>
  <script>
    function start(){
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // get json data
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

    var brightness_slider = document.getElementById("BRIGHTNESS_SLIDER");  
    var frequence_slider = document.getElementById("FREQUENCE_SLIDER");  

    brightness_slider.oninput = function() {
      //document.getElementById("BRIGHTNESS").innerHTML = this.value;
      document.getElementById("BUTT_SUBMIT_BRI").disabled=false;
    } 

    frequence_slider.oninput = function() {
      //document.getElementById("LIGHT_FREQ").innerHTML = this.value;
      document.getElementById("BUTT_SUBMIT_FREQ").disabled=false;
    } 

    function set_bri(){
      var xhttp = new XMLHttpRequest();
      var bri_slider_value = brightness_slider.value;
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // get json data
          console.log("Received data:" + this.responseText);
          var json_msg = JSON.parse(this.responseText);
          bri_slider_value = json_msg.brightness;
          document.getElementById("BRIGHTNESS").innerHTML = json_msg.brightness;
          document.getElementById("BUTT_SUBMIT_BRI").disabled=true;
        }
      };
      var Data2Send = new FormData(); 
      Data2Send.append("brightness",  bri_slider_value);
      console.log("Sending data:" + JSON.stringify(Object.fromEntries(Data2Send)));
      xhttp.open("POST", "/set_bri", true);
      xhttp.send(Data2Send);
    }

    function set_freq(){
      var xhttp = new XMLHttpRequest();
      var freq_slider_value = frequence_slider.value;
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // get json data
          console.log("Received data:" + this.responseText);
          var json_msg = JSON.parse(this.responseText);
          freq_slider_value = json_msg.brightness;
          document.getElementById("FREQUENCE").innerHTML = json_msg.light_freq;
          document.getElementById("BUTT_SUBMIT_FREQ").disabled=true;
        }
      };
      var Data2Send = new FormData(); 
      Data2Send.append("light_freq",  freq_slider_value);
      console.log("Sending data:" + JSON.stringify(Object.fromEntries(Data2Send)));
      xhttp.open("POST", "/set_freq", true);
      xhttp.send(Data2Send);
    }

    setInterval(function ( ) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // get json data
          console.log("Received data:" + this.responseText);
          var json_msg = JSON.parse(this.responseText);
          document.getElementById("BRIGHTNESS").innerHTML = json_msg.brightness;
          document.getElementById("LIGHT_STATUS").innerHTML = json_msg.light_status;
          document.getElementById("REM_TIME").innerHTML = json_msg.rem_time;
          document.getElementById("AUDIO_STATUS").innerHTML = json_msg.audio_status;
          document.getElementById("LIGHT_ERROR").innerHTML  = json_msg.lightFreqErrorUs;
          document.getElementById("FREE_HEAP").innerHTML = json_msg.free_heap;
          document.getElementById("UP_TIME").innerHTML = json_msg.up_time;
          document.getElementById("FREQUENCE").innerHTML = json_msg.light_freq;
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
          document.getElementById("WIFI_IP").innerHTML = json_msg.wifi_ip;
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
<!DOCTYPE HTML>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="refresh" content="8;url=/index.html" />
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
      h2 { font-size: 3.0rem; margin-block-start: 0.5em; margin-block-end: 0.5em;}
      p { font-size: 2.0rem; }
    </style>
  </head>
  <body>
    <h2>Rebooting, please wait.</h2> 
    <p>You'll be redirected in 10 seconds...</p>
  </body>
</html>
)rawliteral";

const char redirect_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
    <meta http-equiv="refresh" content="0; URL='index.html'" />
</head>
<body>
</body>
</html>)rawliteral";

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
      h2 { font-size: 3.0rem; margin-block-start: 0.5em; margin-block-end: 0.5em;}
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
      }
      .switch {
        position: relative;
        display: inline-block;
        width: 60px;
        height: 34px;
      }
      /* Hide default HTML checkbox */
      .switch input {
        opacity: 0;
        width: 0;
        height: 0;
      }
      /* The slider */
      .slider {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        -webkit-transition: .4s;
        transition: .4s;
      }
      .slider:before {
        position: absolute;
        content: "";
        height: 26px;
        width: 26px;
        left: 4px;
        bottom: 4px;
        background-color: white;
        -webkit-transition: .4s;
        transition: .4s;
      }
      input:checked + .slider {
        background-color: #2196F3;
      }
      input:focus + .slider {
        box-shadow: 0 0 1px #2196F3;
      }

      input:checked + .slider:before {
        -webkit-transform: translateX(26px);
        -ms-transform: translateX(26px);
        transform: translateX(26px);
      }
      /* Rounded sliders */
      .slider.round {
        border-radius: 34px;
      }
      .slider.round:before {
        border-radius: 50%%;
      }
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
          <input type="number" id="LIGHT_FREQ" name="LIGHT_FREQ" min="1" max="100" value="%LIGHT_FREQ%"><span class="units">Hz</span>
          <br>
          <label for="ON_TIME" class="param-labels">ON TIME</label>
          <input type="number" id="ON_TIME" name="ON_TIME" min="2" max="100" value="%ON_TIME%"><span class="units">Min</span>
          <br>
          <label for="BRIGHTNESS" class="param-labels">Brightness</label>
          <input type="number" id="BRIGHTNESS" name="BRIGHTNESS" min="10" max="100" value="%BRIGHTNESS%"><span class="units">%%</span>
          <br>
          <label for="PWM_FREQ" class="param-labels">PWM Freq</label>
          <input type="number" id="PWM_FREQ" name="PWM_FREQ" min="1000" max="10000" value="%PWM_FREQ%"><span class="units">Hz</span>
          <br>
          <br>
          <label for="AUTOSTART_SW" class="param-labels">Autostart</label>
          <label class="switch">
            <input type="checkbox" id="AUTOSTART_SW" name="AUTOSTART_SW" value="Autostart" %AUTOSTART_CHECKED%>
            <span class="slider round"></span> 
          </label>
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


#endif
