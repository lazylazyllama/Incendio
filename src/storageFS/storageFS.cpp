#include "storageFS.hpp"

Incendio::StorageFSImpl StorageFS;

bool Incendio::StorageFSImpl::begin() {
  #ifdef ESP32
    return SPIFFS.begin(true);
  #elif defined(ESP8266)
    return LittleFS.begin();
  #endif
}

bool Incendio::StorageFSImpl::format() {
  #ifdef ESP32
    return SPIFFS.format();
  #elif defined(ESP8266)
    return LittleFS.format();
  #endif
}

File Incendio::StorageFSImpl::open(const char* path, const char* mode) {
  #ifdef ESP32
    return SPIFFS.open(path, mode);
  #elif defined(ESP8266)
    return LittleFS.open(path, mode);
  #endif
}
File Incendio::StorageFSImpl::open(const String& path, const char* mode) {
  #ifdef ESP32
    return SPIFFS.open(path, mode);
  #elif defined(ESP8266)
    return LittleFS.open(path, mode);
  #endif
}

bool Incendio::StorageFSImpl::exists(const char* path) {
  #ifdef ESP32
    return SPIFFS.exists(path);
  #elif defined(ESP8266)
    return LittleFS.exists(path);
  #endif
}
bool Incendio::StorageFSImpl::exists(const String& path) {
  #ifdef ESP32
    return SPIFFS.exists(path);
  #elif defined(ESP8266)
    return LittleFS.exists(path);
  #endif
}

bool Incendio::StorageFSImpl::remove(const char* path) {
  #ifdef ESP32
    return SPIFFS.remove(path);
  #elif defined(ESP8266)
    return LittleFS.remove(path);
  #endif
}
bool Incendio::StorageFSImpl::remove(const String& path) {
  #ifdef ESP32
    return SPIFFS.remove(path);
  #elif defined(ESP8266)
    return LittleFS.remove(path);
  #endif
}

bool Incendio::StorageFSImpl::rmdir(const char* path) {
  #ifdef ESP32
    return SPIFFS.rmdir(path);
  #elif defined(ESP8266)
    return LittleFS.rmdir(path);
  #endif
}
bool Incendio::StorageFSImpl::rmdir(const String& path) {
  #ifdef ESP32
    return SPIFFS.rmdir(path);
  #elif defined(ESP8266)
    return LittleFS.rmdir(path);
  #endif
}

bool Incendio::StorageFSImpl::mkdir(const char* path) {
  #ifdef ESP32
    return SPIFFS.mkdir(path);
  #elif defined(ESP8266)
    return LittleFS.mkdir(path);
  #endif
}
bool Incendio::StorageFSImpl::mkdir(const String& path) {
  #ifdef ESP32
    return SPIFFS.mkdir(path);
  #elif defined(ESP8266)
    return LittleFS.mkdir(path);
  #endif
}

bool Incendio::StorageFSImpl::rename(const char* pathFrom, const char* pathTo) {
  #ifdef ESP32
    return SPIFFS.rename(pathFrom, pathTo);
  #elif defined(ESP8266)
    return LittleFS.rename(pathFrom, pathTo);
  #endif
}
bool Incendio::StorageFSImpl::rename(const String& pathFrom, const String& pathTo) {
  #ifdef ESP32
    return SPIFFS.rename(pathFrom, pathTo);
  #elif defined(ESP8266)
    return LittleFS.rename(pathFrom, pathTo);
  #endif
}

void Incendio::StorageFSImpl::end() {
  #ifdef ESP32
    SPIFFS.end();
  #elif defined(ESP8266)
    LittleFS.end();
  #endif
}
