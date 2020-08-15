#include "storageFS.hpp"

Lumos::StorageFSImpl StorageFS;

bool Lumos::StorageFSImpl::begin() {
  #ifdef ESP32
    return SPIFFS.begin(true);
  #elif defined(ESP8266)
    return LittleFS.begin();
  #endif
}

bool Lumos::StorageFSImpl::format() {
  #ifdef ESP32
    return SPIFFS.format();
  #elif defined(ESP8266)
    return LittleFS.format();
  #endif
}

File Lumos::StorageFSImpl::open(const char* path, const char* mode) {
  #ifdef ESP32
    return SPIFFS.open(path, mode);
  #elif defined(ESP8266)
    return LittleFS.open(path, mode);
  #endif
}
File Lumos::StorageFSImpl::open(const String& path, const char* mode) {
  #ifdef ESP32
    return SPIFFS.open(path, mode);
  #elif defined(ESP8266)
    return LittleFS.open(path, mode);
  #endif
}

bool Lumos::StorageFSImpl::exists(const char* path) {
  #ifdef ESP32
    return SPIFFS.exists(path);
  #elif defined(ESP8266)
    return LittleFS.exists(path);
  #endif
}
bool Lumos::StorageFSImpl::exists(const String& path) {
  #ifdef ESP32
    return SPIFFS.exists(path);
  #elif defined(ESP8266)
    return LittleFS.exists(path);
  #endif
}

bool Lumos::StorageFSImpl::remove(const char* path) {
  #ifdef ESP32
    return SPIFFS.remove(path);
  #elif defined(ESP8266)
    return LittleFS.remove(path);
  #endif
}
bool Lumos::StorageFSImpl::remove(const String& path) {
  #ifdef ESP32
    return SPIFFS.remove(path);
  #elif defined(ESP8266)
    return LittleFS.remove(path);
  #endif
}

bool Lumos::StorageFSImpl::rmdir(const char* path) {
  #ifdef ESP32
    return SPIFFS.rmdir(path);
  #elif defined(ESP8266)
    return LittleFS.rmdir(path);
  #endif
}
bool Lumos::StorageFSImpl::rmdir(const String& path) {
  #ifdef ESP32
    return SPIFFS.rmdir(path);
  #elif defined(ESP8266)
    return LittleFS.rmdir(path);
  #endif
}

bool Lumos::StorageFSImpl::mkdir(const char* path) {
  #ifdef ESP32
    return SPIFFS.mkdir(path);
  #elif defined(ESP8266)
    return LittleFS.mkdir(path);
  #endif
}
bool Lumos::StorageFSImpl::mkdir(const String& path) {
  #ifdef ESP32
    return SPIFFS.mkdir(path);
  #elif defined(ESP8266)
    return LittleFS.mkdir(path);
  #endif
}

bool Lumos::StorageFSImpl::rename(const char* pathFrom, const char* pathTo) {
  #ifdef ESP32
    return SPIFFS.rename(pathFrom, pathTo);
  #elif defined(ESP8266)
    return LittleFS.rename(pathFrom, pathTo);
  #endif
}
bool Lumos::StorageFSImpl::rename(const String& pathFrom, const String& pathTo) {
  #ifdef ESP32
    return SPIFFS.rename(pathFrom, pathTo);
  #elif defined(ESP8266)
    return LittleFS.rename(pathFrom, pathTo);
  #endif
}

void Lumos::StorageFSImpl::end() {
  #ifdef ESP32
    SPIFFS.end();
  #elif defined(ESP8266)
    LittleFS.end();
  #endif
}
