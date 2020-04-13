#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

#ifdef ESP32
  #include <WiFi.h>
  #include <esp_wifi.h>
  #include <esp_wifi_types.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif


class WifiManager {
  public:
    static void begin(void);
};

#endif /* WIFI_MANAGER_H */