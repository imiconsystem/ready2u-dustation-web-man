#define R1 23



String APssid = "R2Devkit";
String APpassword = "dddddddd";

int SETMODE = 0;

String WiFissid = "";
String WiFipassword = "";

String WiFiRSSI;
String localIP;
String webTitle = "DustStion1";
String device_name = "dst";

int32_t maxTemp, maxHumi, maxPM1, maxPM2, maxPM10;

int32_t PM1, PM2, PM10, AQI;
float_t TEMP, HUMI;

struct PM25AQI {
  String word;
  String icon;
};
struct PM25AQI pm25lev = {};

String logString;

void setup() {

  Serial.begin(115200);  // For debug
  Serial.println("ESP start.");
  serialSetup();
  pmsSetup();
  oledSetup();
  oledLogSetup();
  relaySetup();
  resetbuttonSetup();
  storageSetup();
  dhtSetup();

  TEMP = 0;
  HUMI = 0;
  PM1 = 0;
  PM2 = 0;
  PM10 = 0;
  AQI = 0.0;

  if (SETMODE == 1)  // SET
  {
    SETMODE = true;
    logString = "Set mode..";
    wifiapSetup();
    oledLogLoop();

  } else if (SETMODE == 2) {  // RESET FACTORY
    logString = "Wait 10 seconds then push EN(reset) button.";
    oledLogLoop();
    storageClear();
    ESP.restart();
  } else {
    // RUN
    webserverSetup();
    logString = "System starting..";
    oledLogLoop();
  }
}

void controlRelay() {
  if (maxTemp > 0 && TEMP > maxTemp) {
    relayHi(R1);
  } else if (maxHumi > 0 && HUMI > maxHumi) {
    relayHi(R1);
  } else if (maxPM1 > 0 && PM1 > maxPM1) {
    relayHi(R1);
  } else if (maxPM2 > 0 && PM2 > maxPM2) {
    relayHi(R1);
  } else if (maxPM10 > 0 && PM10 > maxPM10) {
    relayHi(R1);
  } else {
    relayLo(R1);
  }
}



void loop() {


  if (SETMODE == 0) {

    webserverLoop();
    clientLoop();

    //if (millis() > pmsNextRead) {
    //  while (!pmsReadDone) {
        pmsLoop();
    //  }
    //  pmsReadDone = false;
    //  pmsNextRead = millis() + pmsWaitPeriod;
    //}

    dhtLoop();
    controlRelay();
    oledLoop();

  } else {
    clientLoop();
  }
}