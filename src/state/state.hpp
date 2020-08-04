#ifndef INCENDIO_STATE_H
#define INCENDIO_STATE_H

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

namespace Incendio {
  class State {
    public:
      static bool on;
      static float brightness;
      static String color;
      static String colorMode;
      static int colorTemperature;
      static bool up;
      static bool down;
      static bool stop;

    public:
      static void load(void);
      static void save(void);
  };
}

#endif /* INCENDIO_STATE_H */
