#ifndef INCENDIO_CONFIG_H
#define INCENDIO_CONFIG_H

#define ARDUINOJSON_USE_LONG_LONG 1
#include <Arduino.h>
#include <ArduinoJson.h>

#include "../storageFS/storageFS.hpp"
#include "../device/device.hpp"

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