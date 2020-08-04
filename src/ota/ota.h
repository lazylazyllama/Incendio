#ifndef INCENDIO_OTA_H
#define INCENDIO_OTA_H

#include <ArduinoOTA.h>

namespace Incendio {
  class OTA {
    public:
      static void begin(void);
      static void handle(void);
  };
}

#endif /* INCENDIO_OTA_H */
