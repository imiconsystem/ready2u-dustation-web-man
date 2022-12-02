#define R1 23

#define BLYNK_TEMPLATE_ID "TMPLcuvcgBK4"
#define BLYNK_DEVICE_NAME "Dustation"
#define BLYNK_AUTH_TOKEN "55WZ-IngvdD3Jhfhyf-fYo8l1ybDKiqd"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;


BLYNK_WRITE(V3) // Executes when the value of virtual pin 0 changes
{
  if(param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(23,HIGH);  // Set digital pin 2 HIGH
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(23,LOW);  // Set digital pin 2 LOW    
  }
}


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
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, PM2);
  Blynk.virtualWrite(V1, PM1);
  Blynk.virtualWrite(V2, PM10);
  Blynk.virtualWrite(V4, TEMP);
  Blynk.virtualWrite(V5, HUMI);
  
 
}

void setup() {

  Serial.begin(115200);  // For debug
  Serial.println("ESP start.");

  Blynk.begin(auth, storageGetString("WiFissid").c_str(), storageGetString("WiFipassword").c_str());
  timer.setInterval(1000L, myTimerEvent);

  serialSetup();
  pmsSetup();
  oledSetup();
  oledLogSetup();
  relaySetup();
  resetbuttonSetup();
  storageSetup();
  dhtSetup();
 // blynk_setup();

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
  if (TEMP > maxTemp) {
    relayHi(R1);
    Blynk.virtualWrite(V3, 1);
  } else if (HUMI > maxHumi) {
    relayHi(R1);
    Blynk.virtualWrite(V3, 1);
  } else if (PM1 > maxPM1) {
    relayHi(R1);
    Blynk.virtualWrite(V3, 1);
  } else if (PM2 > maxPM2) {
    relayHi(R1);
  } else if (PM10 > maxPM10) {
    relayHi(R1);
    Blynk.virtualWrite(V3, 1);
  } else {
    relayLo(R1);
    Blynk.virtualWrite(V3, 0);
  }
}



void loop() {
  Blynk.run();
  timer.run();

  if (SETMODE == 0) {

    //webserverLoop();
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
    //blynk_loop();

  } else {
    clientLoop();
  }
}