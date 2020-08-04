#ifndef INCENDIO_CONFIG_H
#define INCENDIO_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef ESP32
  #include <SPIFFS.h>
#elif defined(ESP8266)
  #include <SPI.h>
  #include <FS.h>
  #define FILE_READ       "r"
  #define FILE_WRITE      "w"
  #define FILE_APPEND     "a"
#endif

#include "../device/device.h"

namespace Incendio {
  class Config {
    public:
      static String deviceName;
      static Incendio::DeviceType deviceType;
    public:
      static void load(void);
      static void save(void);
  };
}

#endif /* INCENDIO_CONFIG_H */