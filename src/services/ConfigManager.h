#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <SPIFFS.h>
#include <ArduinoJson.h>

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
