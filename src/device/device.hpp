#ifndef INCENDIO_DEVICE_H
#define INCENDIO_DEVICE_H

#define ARDUINOJSON_USE_LONG_LONG 1
#include <Thing.h>
#include <WebThingAdapter.h>
#ifdef ESP32
  #include <analogWrite.h>
#endif

#include "../state/state.hpp"

namespace Incendio {
  enum DeviceType {
    UNDEFINED, RGBW_LIGHTSTRIP, ROLLER_SHUTTER
  };
  
  class Device {
    public:
      ThingDevice device;

    public:
      Device(const char *title, const char **deviceTypes) : device("incendio-device", title, deviceTypes) {};
      virtual void handle(void) = 0;
  };
}

#endif /* INCENDIO_DEVICE_H */
