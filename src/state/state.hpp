#ifndef LUMOS_STATE_H
#define LUMOS_STATE_H

#define ARDUINOJSON_USE_LONG_LONG 1
#define ARDUINOJSON_DECODE_UNICODE 1

#include <ArduinoJson.h>
#include "../storageFS/storageFS.hpp"

namespace Lumos {
  class State {
    public:
      static bool on;
      static float brightness;
      static String color;
      static String colorMode;
      static int colorTemperature;

    public:
      static void load(void);
      static void save(void);
  };
}

#endif /* LUMOS_STATE_H */
