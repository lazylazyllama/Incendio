#include "state.h"

#define dataBackupFileName "/dataBackup.json"
#define dataBackupFileSize 512

void Incendio::State::load() {
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

  Incendio::State::on = doc["on"] | false;
  Incendio::State::brightness = doc["brightness"] | 100.0f;
  Incendio::State::color = doc["color"] | "#ffffff";
  Incendio::State::colorMode = doc["colorMode"] | "temperature";
  Incendio::State::colorTemperature = doc["colorTemperature"] | 2700;

  file.close();

  SPIFFS.end();
}

void Incendio::State::save() {
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

  doc["lastOn"] = Incendio::State::on;
  doc["lastBrightness"] = Incendio::State::brightness;
  doc["lastColor"] = Incendio::State::color;
  doc["lastColorMode"] = Incendio::State::colorMode;
  doc["lastColorTemperature"] = Incendio::State::colorTemperature;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  SPIFFS.end();
}