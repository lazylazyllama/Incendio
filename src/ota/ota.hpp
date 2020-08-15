#ifndef LUMOS_OTA_H
#define LUMOS_OTA_H

#include <ArduinoOTA.h>

namespace Lumos {
  class OTA {
    public:
      static void begin(void);
      static void handle(void);
  };
}

#endif /* LUMOS_OTA_H */
