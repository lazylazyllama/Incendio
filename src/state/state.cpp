#include "state.h"

#define dataBackupFileName "/dataBackup.json"
#define dataBackupFileSize 512

LuxIo::State LuxIo::StateManager::load() {
  #ifdef ESP32
    bool ok = SPIFFS.begin(true);
  #elif defined(ESP8266)
    bool ok = SPIFFS.begin();
  #endif

  if(!ok) {
    SPIFFS.format();
  }

  File file = SPIFFS.open(dataBackupFileName, FILE_READ);

  StaticJsonDocument<dataBackupFileSize> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read file, using default state"));
  }

  LuxIo::State state;

  state.lastOn = doc["lastOn"] | false;
  state.lastBrightness = doc["lastBrightness"] | 100.0f;
  state.lastColor = doc["lastColor"] | "#ffffff";
  state.lastColorMode = doc["lastColorMode"] | "temperature";
  state.lastColorTemperature = doc["lastColorTemperature"] | 2700;

  file.close();

  SPIFFS.end();

  return state;
}

void LuxIo::StateManager::save(LuxIo::State state) {
  #ifdef ESP32
    bool ok = SPIFFS.begin(true);
  #elif defined(ESP8266)
    bool ok = SPIFFS.begin();
  #endif

  if(!ok) {
    SPIFFS.format();
  }

  SPIFFS.remove(dataBackupFileName);

  File file = SPIFFS.open(dataBackupFileName, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  StaticJsonDocument<dataBackupFileSize> doc;

  doc["lastOn"] = state.lastOn;
  doc["lastBrightness"] = state.lastBrightness;
  doc["lastColor"] = state.lastColor;
  doc["lastColorMode"] = state.lastColorMode;
  doc["lastColorTemperature"] = state.lastColorTemperature;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  SPIFFS.end();
}