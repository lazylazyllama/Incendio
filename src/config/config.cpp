#include "config.hpp"

#define CONFIG_FILE_NAME "/config.json"
#define CONFIG_FILE_SIZE 512

String Lumos::Config::deviceName = "Undefiniertes Gerät";
Lumos::DeviceType Lumos::Config::deviceType = DeviceType::UNDEFINED;

void Lumos::Config::load() {
  bool ok = StorageFS.begin();

  if(!ok) {
    StorageFS.format();
  }

  File file = StorageFS.open(CONFIG_FILE_NAME, FILE_READ);

  StaticJsonDocument<CONFIG_FILE_SIZE> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read file, using default state"));
  }

  Lumos::Config::deviceName = doc["deviceName"] | "Lumos Device";
  Lumos::Config::deviceType = doc["deviceType"] | Lumos::DeviceType::UNDEFINED;

  file.close();

  StorageFS.end();
}

void Lumos::Config::save() {
  bool ok = StorageFS.begin();

  if(!ok) {
    StorageFS.format();
  }

  StorageFS.remove(CONFIG_FILE_NAME);

  File file = StorageFS.open(CONFIG_FILE_NAME, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  StaticJsonDocument<CONFIG_FILE_SIZE> doc;

  doc["deviceName"] = Lumos::Config::deviceName;
  doc["deviceType"] = Lumos::Config::deviceType;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  StorageFS.end();
}