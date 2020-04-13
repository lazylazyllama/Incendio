#include "ConfigManager.h"

#define dataBackupFileName "/dataBackup.json"
#define dataBackupFileSize 512

ConfigData ConfigManager::load() {
  SPIFFS.begin(true);

  File file = SPIFFS.open(dataBackupFileName);

  StaticJsonDocument<dataBackupFileSize> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read file, using default configuration"));
  }

  ConfigData configData;

  configData.lastOn = doc["lastOn"] | false;
  configData.lastColor = doc["lastColor"] | "#ffffff";
  configData.lastBrightness = doc["lastBrightness"] | 100;
  configData.lastColorMode = doc["lastColorMode"] | "temperature";
  configData.lastColorTemperature = doc["lastColorTemperature"] | 2700;

  file.close();

  SPIFFS.end();

  return configData;
}

void ConfigManager::save(ConfigData configData) {
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  SPIFFS.remove(dataBackupFileName);

  File file = SPIFFS.open(dataBackupFileName, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  StaticJsonDocument<dataBackupFileSize> doc;

  doc["lastOn"] = configData.lastOn;
  doc["lastColor"] = configData.lastColor;
  doc["lastBrightness"] = configData.lastBrightness;
  doc["lastColorMode"] = configData.lastColorMode;
  doc["lastColorTemperature"] = configData.lastColorTemperature;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  SPIFFS.end();
}