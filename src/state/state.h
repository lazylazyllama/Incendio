#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

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

namespace LuxIo {
  struct State {
    bool lastOn;
    float lastBrightness;
    String lastColor;
    String lastColorMode;
    int lastColorTemperature;
  };

  class StateManager {
    public:
      static State load(void);
      static void save(State state);
  };
}

#endif /* CONFIG_MANAGER_H */
