#include "PMS.h"
PMS pms(Serial2);
PMS::DATA data;
int set = 18;

const int STANDARD_AQI[5] = { 0, 25, 50, 100, 200 };
const int STANDARD_AQI_PM2[5] = { 0, 25, 37, 50, 90 };


int calAqi(int average) {

  float stepAdd, aqiDiff, valueDiff, aqiLv;
  stepAdd = 1.0;

  for (int i = 1; i <= 4; i++) {

    if (average <= STANDARD_AQI_PM2[i]) {
      average -= STANDARD_AQI_PM2[i - 1] + (i == 1 ? 0 : stepAdd);
      aqiDiff = STANDARD_AQI_PM2[i] - (STANDARD_AQI_PM2[i - 1] + (i == 1 ? 0 : 1));
      aqiDiff = STANDARD_AQI[i] - (STANDARD_AQI[i - 1] + (i == 1 ? 0 : 1));
      valueDiff = STANDARD_AQI_PM2[i] - (STANDARD_AQI_PM2[i - 1] + (i == 1 ? 0 : stepAdd));

      aqiLv = STANDARD_AQI_PM2[i - 1] + (i == 1 ? 0 : 1);
      aqiLv += (average * aqiDiff) / valueDiff;
      // console.log(""+i+": "+average+"*"+aqiDiff+"/"+valueDiff);

      return round(aqiLv);
    }
  }
  //console.log("average = "+average);
  average -= STANDARD_AQI_PM2[4];

  return average + 200;
}

void SetPm25Level(int aqi) {

  if (aqi > 200) pm25lev = { "Unhealthy", "'" };
  else if (aqi > 100 && aqi <= 200) pm25lev = { "Sensitive", "%" };
  else if (aqi > 50 && aqi <= 100) pm25lev = { "Moderate", "$" };
  else if (aqi > 25 && aqi <= 50) pm25lev = { "Good", "!" };
  else if (aqi >= 0 && aqi <= 25) pm25lev = { "VeryGood", " " };
}

void pmsSetup() {

  pinMode(set, OUTPUT);
  //pmsReadDone = false;
}


int pmsSleepPeriod = 5;
int pmsStablePeriod = 0;
bool pmsIsWakeup = false;
void pmsWakeup() {
  if (!pmsIsWakeup) {
    Serial.println("Waking up...");
    digitalWrite(set, HIGH);  //Setting hardware Waking up
    pmsIsWakeup = true;
    pmsStablePeriod = 10;
  }
}

void pmsSleep() {
  Serial.println("Going to sleep.");
  digitalWrite(set, LOW);  //Setting hardware sleep
  pmsSleepPeriod = 5;
  pmsIsWakeup = false;
}


bool pmsWaitSleep() {
  if (pmsSleepPeriod > 0) {
    Serial.print("PMS is sleep for ");
    Serial.println(pmsSleepPeriod);
    pmsSleepPeriod--;
    return true;
  }
  return false;
}

bool pmsWaitStable() {
  if (pmsStablePeriod > 0) {
    Serial.print("Wait for stable readings ");
    Serial.println(pmsStablePeriod);
    pmsStablePeriod--;
    return true;
  }
  return false;
}


void pmsLoop() {

  if (!pmsWaitSleep()) {
    pmsWakeup();

    if (!pmsWaitStable()) {
      Serial.println("Send read request...");
      pms.requestRead();
      Serial.println("Wait max. 1 second for read...");
      if (pms.readUntil(data)) {

        PM1 = data.PM_AE_UG_1_0;
        PM2 = data.PM_AE_UG_2_5;
        PM10 = data.PM_AE_UG_10_0;

        AQI = calAqi(PM2);
        SetPm25Level(AQI);

        Serial.print("AQI: ");
        Serial.println(pm25lev.word);

        String result = "";
        result.concat("PM1.0: ");
        result.concat(PM1);
        Serial.print("PM 1.0 (ug/m3): ");
        Serial.println(data.PM_AE_UG_1_0);

        result.concat("\nPM2.5: ");
        result.concat(PM2);
        Serial.print("PM 2.5 (ug/m3): ");
        Serial.println(data.PM_AE_UG_2_5);

        result.concat("\nPM10:  ");
        result.concat(PM10);
        Serial.print("PM 10.0 (ug/m3): ");
        Serial.println(data.PM_AE_UG_10_0);
        pmsSleep();

      } else {
        Serial.println("No data.");
      }
    }
  }
}