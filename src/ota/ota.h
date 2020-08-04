#ifndef LUXIO_OTA_H
#define LUXIO_OTA_H

#include <ArduinoOTA.h>

namespace Incendio {
  class OTA {
    public:
      static void begin(void);
      static void handle(void);
  };
}

#endif /* LUXIO_OTA_H */
