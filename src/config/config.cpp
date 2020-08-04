#include "config.hpp"

#define CONFIG_FILE_NAME "/config.json"
#define CONFIG_FILE_SIZE 512

String Incendio::Config::deviceName = "sdf";
Incendio::DeviceType Incendio::Config::deviceType = DeviceType::UNDEFINED;

void Incendio::Config::load() {
  #ifdef ESP32
    bool ok = SPIFFS.begin(true);
  #elif defined(ESP8266)
    bool ok = SPIFFS.begin();
  #endif

  if(!ok) {
    SPIFFS.format();
  }

  File file = SPIFFS.open(CONFIG_FILE_NAME, FILE_READ);

  StaticJsonDocument<CONFIG_FILE_SIZE> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read file, using default state"));
  }

  Incendio::Config::deviceName = doc["deviceName"] | "Incendio Device";
  Incendio::Config::deviceType = doc["deviceType"] | Incendio::DeviceType::UNDEFINED;

  file.close();

  SPIFFS.end();
}

void Incendio::Config::save() {
  #ifdef ESP32
    bool ok = SPIFFS.begin(true);
  #elif defined(ESP8266)
    bool ok = SPIFFS.begin();
  #endif

  if(!ok) {
    SPIFFS.format();
  }

  SPIFFS.remove(CONFIG_FILE_NAME);

  File file = SPIFFS.open(CONFIG_FILE_NAME, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  StaticJsonDocument<CONFIG_FILE_SIZE> doc;

  doc["deviceName"] = Incendio::Config::deviceName;
  doc["deviceType"] = Incendio::Config::deviceType;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  SPIFFS.end();
}