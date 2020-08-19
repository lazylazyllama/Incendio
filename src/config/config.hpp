#ifndef LUMOS_CONFIG_H
#define LUMOS_CONFIG_H

#define ARDUINOJSON_USE_LONG_LONG 1
#define ARDUINOJSON_DECODE_UNICODE 1

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../storageFS/storageFS.hpp"
#include "../devices/device.hpp"

namespace Lumos {
  class Config {
    public:
      static String deviceName;
      static Lumos::DeviceType deviceType;

    public:
      static void load(void);
      static void save(void);
  };
}

#endif /* LUMOS_CONFIG_H */