#ifndef INCENDIO_STATE_H
#define INCENDIO_STATE_H

#include <ArduinoJson.h>
#include "../storageFS/storageFS.hpp"

namespace Incendio {
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

#endif /* INCENDIO_STATE_H */
