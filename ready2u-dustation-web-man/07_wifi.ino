#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <ESPmDNS.h>

WebServer server(80);

String authFailResponse = "Authentication Failed";

void wifiapSetup() {
  Serial.println("Configuring access point...");
  Serial.println("APssid");
  Serial.println(storageGetString("APssid"));

  Serial.println("APpassword");
  Serial.println(storageGetString("APpassword"));

  WiFi.softAP(storageGetString("APssid").c_str(), storageGetString("APpassword").c_str());

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  String dis = "";
  dis.concat("IP: ");
  dis.concat(myIP.toString());
  dis.concat("\n");
  dis.concat("SSID: ");
  dis.concat(storageGetString("APssid"));
  dis.concat("\n");
  dis.concat("PW: ");
  dis.concat(storageGetString("APpassword"));
  dis.concat("\n");
  logString = dis;

  server.on("/", configForm);
  server.on("/saveConfig", saveConfig);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("AP mode started");
}

unsigned long previousMillis = 0;
unsigned long interval = 30000;
void webserverSetup() {
  unsigned long currentMillis = millis();
  Serial.println("WiFissid");
  Serial.println(storageGetString("WiFissid"));
  Serial.println("WiFipassword");
  Serial.println(storageGetString("WiFipassword"));

  WiFi.mode(WIFI_STA);
  WiFi.begin(storageGetString("WiFissid").c_str(), storageGetString("WiFipassword").c_str());

  //Wait for connection
  while (WiFi.status() != WL_CONNECTED && (currentMillis - previousMillis >= interval)) {
    //Serial.println(WiFi.status());
    delay(1000);
    previousMillis = currentMillis;
  }

  Serial.println("");
  if (MDNS.begin(storageGetString("deviceName").c_str())) {
    Serial.println("MDNS responder started");
    Serial.println(storageGetString("deviceName").c_str());
  }

  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void webserverLoop() {

  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  while ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    //Serial.println(WiFi.status());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    delay(1000);
    previousMillis = currentMillis;
  }

  if (WiFi.status() == WL_CONNECTED) {
    WiFiRSSI = WiFi.RSSI();
    localIP = WiFi.localIP().toString();

    Serial.print("Connected to ");
    Serial.println(storageGetString("WiFissid"));
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void clientLoop() {

  server.handleClient();
  // Serial.println("Client loop");
  delay(10);  // allow the cpu to switch to other tasks
}

void handleRoot() {

  char html[4000];
  String pm25word,relayText, maxTempClass, maxHumiClass, maxPM1Class, maxPM2Class, maxPM10Class, icon;
  pm25word = pm25lev.word.c_str();

  relayText = "OFF";
  if (TEMP > maxTemp) {
    maxTempClass = "alert";
    relayText = "ON";
  } else
    maxTempClass = "normal";

  if (HUMI > maxHumi) {
    maxHumiClass = "alert";
    relayText = "ON";
  } else
    maxHumiClass = "normal";

  if (PM1 > maxPM1) {
    maxPM1Class = "alert";
    relayText = "ON";
  } else
    maxPM1Class = "normal";

  if (PM2 > maxPM2) {
    maxPM2Class = "alert";
    relayText = "ON";
  } else
    maxPM2Class = "normal";

  if (PM10 > maxPM10) {
    maxPM10Class = "alert";
    relayText = "ON";
  } else
    maxPM10Class = "normal";

  if (pm25lev.icon == "'") {
    icon = "&#128567;";
  } else if (pm25lev.icon == "%") {
    icon = "&#128577;";
  } else if (pm25lev.icon == "$") {
    icon = "&#128528;";
  } else if (pm25lev.icon == "!") {
    icon = "&#128578;";
  } else if (pm25lev.icon == " ") {
    icon = "&#128512;";
  }else{
    icon = " ";
  }

  snprintf(html, 4000,
           "<!DOCTYPE html>\
<html>\
<head>\
<meta charset=\"UTF-8\" />\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
<meta http-equiv='refresh' content='10' />\
<style>.max-val span,h3{text-align:center}#pm25-num sup,body{font-family:Arial,Helvetica,sans-serif}html{width:100vw;height:100vh}body{margin:0 30px;background:#001213;background:linear-gradient(126deg,#001213 0,#005157 100%%);color:#eee}.val{font-size:30px;display:block;margin-bottom:10px}.pm-other .val{text-align:right}.max-val span{display:inline-block;background-color:#ffffff21;border-radius:5px;padding:5px;font-size:10px;margin:3px;}.alert{background-color:#00be8e!important}h1{font-size:16px;margin:0 0 3px;padding:0;color:#89ffae}h3{font-size:22px;margin-top:0}*{box-sizing:border-box}.flex-container{display:flex;flex-direction:row;text-align:left}.flex-item-left{padding:10px 20px 10px 0;flex:50%%;border-right:1px solid #ffffff52;position:relative}.flex-item-right{padding:10px 0 10px 20px;flex:50%%}.flex-item-right sup{font-size:15px}#pm25-num{font-size:100px;line-height:100px}#pm25-num sup{font-size:20px}.emoticon{font-size:10rem;filter:grayscale(1);opacity:.2;position:absolute;right:0}@media (max-width:660px){.emoticon{top:0}.flex-container{flex-direction:column}.flex-item-left{padding:10px 0;border-right:0;border-bottom:1px solid #ffffff52}.flex-item-right{padding:10px 0}}.container{max-width:800px;margin:auto}</style>\
</head>\
<body>\
<div class=\"container\">\
<h3 style=\"display: flex;align-items: center;column-gap: 10px;justify-content: center;\"><span style='font-size:50px;filter: grayscale(1);'>&#127780;</span> %s</h3>\
<div class=\"flex-container\">\
<div class=\"flex-item-left\">\
<div class=\"emoticon\">%s</div>\
<div class=\"pm25\" style=\"display: flex;flex-direction: row;justify-content: space-between;flex-wrap: nowrap;\">\
<div class=\"\">\
<h1>PM2.5</h1>\
<div id=\"pm25-num\" class=\"val\">%d\
</div>\
<h1>Thai AQI</h1>\
<span class=\"val\">%d</span>\
<span>%s</span>\
</div>\
<div class=\"pm-other\" style=\"display: flex; flex-direction: column;\">\
<div>\
<h1>PM1.0</h1><span class=\"val\">%d</span>\
</div>\
<div>\
<h1>PM10</h1><span class=\"val\">%d</span>\
</div>\
<div style=\"text-align: right;\"><span>&#181;g/m<sup>3</sup></span></div>\
</div>\
</div>\
</div>\
<div class=\"flex-item-right\">\
<div class=\"\">\
<div class=\"\" style=\"display: flex;flex-direction: column;\">\
<div style=\"display: flex;flex-wrap: nowrap;justify-content: space-between;\">\
<div style=\"text-align:left;\">\
<h1><span style=\"line-height: 10px;\">&#127777;</span>TEMP</h1> <span class=\"val\">%.2f <sup>&#8451;</sup></span>\
</div>\
<div style=\"text-align:left;\">\
<h1><span style=\"line-height: 10px;\">&#9729;</span> HUMI</h1> <span class=\"val\">%.2f <sup>&percnt;</sup></span>\
</div>\
</div>\
</div>\
</div>\
<div class=\"relay-box\" style=\"text-align:center; padding-top:30px;\">\
<div>\
<h1>RELAY</h1>\
<span class=\"val\">%s</span>\
</div>\
<div class=\"max-val\">\
<span class=\"%s\">TEMP > %d</span>\
<span class=\"%s\">HUMI > %d</span>\
<span class=\"%s\">PM1 > %d</span>\
<span class=\"%s\">PM2.5 > %d</span>\
<span class=\"%s\">PM10 > %d</span>\
</div>\
</div>\
</div>\
</div>\
<p style=\"text-align: center;\"><b>Domain:</b> %s.local</p>\
</div>\
</body>\
</html>",
           storageGetString("webTitle").c_str(), icon.c_str(), PM2, AQI, pm25word.c_str(), PM1, PM10, TEMP, HUMI, relayText.c_str(), maxTempClass.c_str(), maxTemp, maxHumiClass.c_str(), maxHumi, maxPM1Class.c_str(), maxPM1, maxPM2Class.c_str(), maxPM2, maxPM10Class.c_str(), maxPM10, storageGetString("deviceName").c_str());
  server.send(200, "text/html", html);
}


void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void configForm() {
  char html[3000];

  snprintf(html, 3000,

           "<html>\
<head>\
<meta charset=\"UTF-8\" />\
<title>System configuration.</title>\
<style>\
   html{width:100%%;height:100%%}body{margin:0 30px;background:#005157;font-family:Arial,Helvetica,sans-serif;font-size:1.8rem;color:#eee}input:not([type=checkbox]){padding:20px;font-size:20px;width:100%%;margin:10px 0;font-weight:700;font-size:2rem;border:0;border-radius:5px;}legend h1{font-size:2.5rem;;color:#7fffd4}input[type=submit]{background-color:#32ab4e;}\
</style>\
</head>\
<body>\
<div style=\"width:95vw;margin:auto;\">\
<h1>System configuration.</h1>\
<form action=\"/saveConfig\" method=\"POST\">\
<fieldset>\
<legend>\
<h1>WiFi Router credential:</h1>\
</legend>\
<h3>SSID:</h3>\
<div><input type=\"text\" name=\"WiFissid\" value=\"%s\" maxlength=\"10\"></div>\
<h3>PASSWORD:</h3>\
<div><input type=\"text\" name=\"WiFipassword\" value=\"%s\" maxlength=\"10\"></div>\
</fieldset>\
<fieldset>\
<legend>\
<h1>Set max values</h1>\
</legend>\
<h3>Temp:</h3>\
<div><label>Max </label><input type=\"number\" min=\"0\" max=\"100\" name=\"temp\" value=\"%d\" size=\"5\">&#xb0;C</div>\
<h3>Humi:</h3>\
<div><label>Max </label><input type=\"number\" min=\"0\" max=\"100\" name=\"humi\" value=\"%d\" size=\"5\">&percnt;</div>\
<h3>PM1:</h3>\
<div><label>Max </label><input type=\"number\" min=\"0\" max=\"999\" name=\"pm1\" value=\"%d\" size=\"5\"><span>&#181;g/m<sup>3</sup></span></div>\
<h3>PM2.5:</h3>\
<div><label>Max </label><input type=\"number\" min=\"0\" max=\"999\" name=\"pm2\" value=\"%d\" size=\"5\"><span>&#181;g/m<sup>3</sup></span></div>\
<h3>PM10:</h3>\
<div><label>Max </label><input type=\"number\" min=\"0\" max=\"999\" name=\"pm10\" value=\"%d\" size=\"5\"><span>&#181;g/m<sup>3</sup></span></div>\
</fieldset>\
<fieldset>\
<legend>\
<h1>System setting:</h1>\
</legend>\
<h3>Web title:(No space)</h3>\
<div><input type=\"text\" name=\"web-title\" value=\"%s\" maxlength=\"10\"></div>\
<h3>Device name:</h3>\
<div><input type=\"text\" name=\"device-name\" value=\"%s\" maxlength=\"10\"></div>\
</fieldset>\
<fieldset>\
<legend>\
<h1>WiFi AP mode credential:</h1>\
</legend>\
<h3>SSID:</h3>\
<div><input type=\"text\" name=\"APssid\" value=\"%s\" maxlength=\"10\"></div>\
<h3>PASSWORD:</h3>\
<div><input type=\"text\" name=\"APpassword\" value=\"%s\" maxlength=\"10\"></div>\
</fieldset>\
<div><input type=\"submit\" value=\"SAVE\"></div>\
</form>\
<div style=\"text-align: center;\">\
<hr> Ready to use Dustation Devkit.</div>\
</div>\
</body>\
</html>",

           storageGetString("WiFissid").c_str(), storageGetString("WiFipassword").c_str(), maxTemp, maxHumi, maxPM1, maxPM2, maxPM10, storageGetString("webTitle").c_str(), storageGetString("deviceName").c_str(),storageGetString("APssid").c_str(), storageGetString("APpassword").c_str());
  server.send(200, "text/html", html);

  //storageGetString("WiFissid"), storageGetString("WiFipassword"), maxTemp, maxHumi, maxPM1, maxPM2, maxPM10, storageGetString("webTitle"), storageGetString("deviceName"),storageGetString("APssid"), storageGetString("APpassword"));
  server.send(200, "text/html", html);
}

void saveConfig() {

  char temp, humi, pm1, pm2, pm10;

  temp = humi = pm1 = pm2 = pm10 = 0;

  String message = "<h3>Data saved.<br></h3><hr>";
  String WIFIssid, WIFIpassword, APssid, APpassword, _webTitle, _deviceName;
  // message += (server.method() == HTTP_GET) ? "GET" : "POST";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " <b>" + server.argName(i) + ":</b> ______" + server.arg(i) + "______<br>\n";
    if (server.argName(i) == "APssid") {
      APssid = server.arg(i);
    } else if (server.argName(i) == "APpassword") {
      APpassword = server.arg(i);
    } else if (server.argName(i) == "WiFissid") {
      WIFIssid = server.arg(i);
    } else if (server.argName(i) == "WiFipassword") {
      WIFIpassword = server.arg(i);
    } else if (server.argName(i) == "temp") {
      temp = server.arg(i).toInt();
    } else if (server.argName(i) == "humi") {
      humi = server.arg(i).toInt();
    } else if (server.argName(i) == "pm1") {
      pm1 = server.arg(i).toInt();
    } else if (server.argName(i) == "pm2") {
      pm2 = server.arg(i).toInt();
    } else if (server.argName(i) == "pm10") {
      pm10 = server.arg(i).toInt();
    } else if (server.argName(i) == "web-title") {
      _webTitle = server.arg(i);
    } else if (server.argName(i) == "device-name") {
      _deviceName = server.arg(i);
    }
  }
  message += "<html><head><meta charset=\"UTF-8\">\
  <style>\
   html{width:100%%;height:100%%}body{margin:30px;background:#005157;font-family:Arial,Helvetica,sans-serif;color:#eee}\
</style></head><body><script>alert('Saving data please wait until device restarted.');</script><br><br></body></html>";
  server.send(200, "text/html", message);

  storagePutString("APssid", APssid);
  storagePutString("APpassword", APpassword);
  storagePutString("WiFissid", WIFIssid);
  storagePutString("WiFipassword", WIFIpassword);
  storagePutString("webTitle", _webTitle);
  storagePutString("deviceName", _deviceName);
  storagePutInt("maxTemp", temp);
  storagePutInt("maxHumi", humi);
  storagePutInt("maxPM1", pm1);
  storagePutInt("maxPM2", pm2);
  storagePutInt("maxPM10", pm10);
  ESP.restart();
}