#include <Preferences.h>
#include <nvs_flash.h>

Preferences preferences;
const char *prekey = "APcredentials";

void storageSetup() {
  String WIssid, WIpass, Assid, Apass, www_realm, www_username, www_password, _device_name, _webTitle;
  preferences.begin(prekey, false);

  maxTemp = preferences.getInt("maxTemp", 0);
  maxHumi = preferences.getInt("maxHumi", 0);
  maxPM1 = preferences.getInt("maxPM1", 0);
  maxPM2 = preferences.getInt("maxPM2", 0);
  maxPM10 = preferences.getInt("maxPM10", 0);

  WIssid = preferences.getString("WiFissid", "");
  WIpass = preferences.getString("WiFipassword", "");

  _device_name = preferences.getString("deviceName", "");
  _webTitle = preferences.getString("webTitle", "");

  Assid = preferences.getString("APssid", "");
  Apass = preferences.getString("APpassword", "");

  www_realm = preferences.getString("www_realm", "").c_str();
  www_username = preferences.getString("www_username", "").c_str();
  www_password = preferences.getString("www_password", "").c_str();

  if (Assid == "" || Apass == "") {
    preferences.putString("APssid", APssid);
    preferences.putString("APpassword", APpassword);
    Serial.println("No values saved for APssid or APpassword");
    Serial.println("AP Credentials Saved using default value.");
  }

  if (WIssid == "" || WIpass == "") {
    preferences.putString("WiFissid", WiFissid);
    preferences.putString("WiFipassword", WiFipassword);
    Serial.println("No values saved for WIFssid or WiFipassword");
    Serial.println("WIFI Credentials Saved using default value.");
  }

  if (_device_name == "") {
    preferences.putString("deviceName", device_name);
    Serial.println("No values saved for deviceName");
    Serial.println("deviceName Saved using default value.");
  }

  if (_webTitle == "") {
    preferences.putString("webTitle", webTitle);
    Serial.println("No values saved for webTitle");
    Serial.println("webTitleSaved using default value.");
  }

  preferences.end();
}

void storagePutString(const char *key, String value) {
  preferences.begin(prekey, false);
  preferences.putString(key, value);
  preferences.end();
}

void storagePutInt(const char *key, int32_t value) {
  preferences.begin(prekey, false);
  preferences.putInt(key, value);
  preferences.end();
}

void storagePutChar(const char *key, int8_t val) {
  preferences.begin(prekey, false);
  preferences.putChar(key, val);
  preferences.end();
}

String storageGetString(const char *key) {
  String result;
  preferences.begin(prekey, false);
  result = preferences.getString(key, "").c_str();
  preferences.end();
  return result;
}

String storageGetInt(const char *key) {
  String result;
  preferences.begin(prekey, false);
  result = preferences.getInt(key, 0);
  preferences.end();
  return result;
}

String storageGetChar(const char *key) {
  String result;
  preferences.begin(prekey, false);
  result = preferences.getChar(key);
  preferences.end();
  return result;
}

void storageClear() {
  nvs_flash_erase();  // erase the NVS partition and...
  nvs_flash_init();   // initialize the NVS partition.
  while (true)
    ;
}