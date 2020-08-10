#include "state.hpp"

#define STATE_FILE_NAME "/state.json"
#define STATE_FILE_SIZE 512

bool Incendio::State::on = false;
float Incendio::State::brightness = 100.0f;
String Incendio::State::color = "#ffffff";
String Incendio::State::colorMode = "temperature";
int Incendio::State::colorTemperature = 2700;
bool Incendio::State::up = false;
bool Incendio::State::down = false;
bool Incendio::State::stop = true;

void Incendio::State::load() {
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

  Incendio::State::on = doc["on"] | false;
  Incendio::State::brightness = doc["brightness"] | 100.0f;
  Incendio::State::color = doc["color"] | "#ffffff";
  Incendio::State::colorMode = doc["colorMode"] | "temperature";
  Incendio::State::colorTemperature = doc["colorTemperature"] | 2700;
  Incendio::State::up = doc["up"] | false;
  Incendio::State::down = doc["down"] | false;
  Incendio::State::stop = doc["stop"] | true;

  file.close();

  StorageFS.end();
}

void Incendio::State::save() {
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

  doc["lastOn"] = Incendio::State::on;
  doc["lastBrightness"] = Incendio::State::brightness;
  doc["lastColor"] = Incendio::State::color;
  doc["lastColorMode"] = Incendio::State::colorMode;
  doc["lastColorTemperature"] = Incendio::State::colorTemperature;
  doc["up"] = Incendio::State::up;
  doc["down"] = Incendio::State::down;
  doc["stop"] = Incendio::State::stop;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  StorageFS.end();
}