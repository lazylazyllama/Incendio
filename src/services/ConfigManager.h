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

struct ConfigData {
  bool lastOn;
  String lastColor;
  int lastBrightness;
  String lastColorMode;
  int lastColorTemperature;
};

class ConfigManager {
  public:
    static ConfigData load(void);
    static void save(ConfigData configData);
};

#endif /* CONFIG_MANAGER_H */
