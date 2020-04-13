#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>

class WifiManager {
  public:
    static void begin(void);
};

#endif /* WIFI_MANAGER_H */