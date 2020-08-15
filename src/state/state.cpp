#include "state.hpp"

#define STATE_FILE_NAME "/state.json"
#define STATE_FILE_SIZE 512

bool Lumos::State::on = false;
float Lumos::State::brightness = 100.0f;
String Lumos::State::color = "#ffffff";
String Lumos::State::colorMode = "temperature";
int Lumos::State::colorTemperature = 2700;

void Lumos::State::load() {
  bool ok = StorageFS.begin();

  if(!ok) {
    StorageFS.format();
  }

  File file = StorageFS.open(STATE_FILE_NAME, FILE_READ);

  StaticJsonDocument<STATE_FILE_SIZE> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read file, using default state"));
  }

  Lumos::State::on = doc["on"] | false;
  Lumos::State::brightness = doc["brightness"] | 100.0f;
  Lumos::State::color = doc["color"] | "#ffffff";
  Lumos::State::colorMode = doc["colorMode"] | "temperature";
  Lumos::State::colorTemperature = doc["colorTemperature"] | 2700;

  file.close();

  StorageFS.end();
}

void Lumos::State::save() {
  bool ok = StorageFS.begin();

  if(!ok) {
    StorageFS.format();
  }

  StorageFS.remove(STATE_FILE_NAME);

  File file = StorageFS.open(STATE_FILE_NAME, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  StaticJsonDocument<STATE_FILE_SIZE> doc;

  doc["lastOn"] = Lumos::State::on;
  doc["lastBrightness"] = Lumos::State::brightness;
  doc["lastColor"] = Lumos::State::color;
  doc["lastColorMode"] = Lumos::State::colorMode;
  doc["lastColorTemperature"] = Lumos::State::colorTemperature;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  StorageFS.end();
}