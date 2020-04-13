#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include <ArduinoOTA.h>

class OTAManager {
  public:
    static void begin(void);
    static void handle(void);
};

#endif /* OTA_MANAGER_H */
