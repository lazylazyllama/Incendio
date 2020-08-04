#ifndef INCENDIO_DEVICE_H
#define INCENDIO_DEVICE_H

#include <Thing.h>
#include <WebThingAdapter.h>

namespace Incendio {
  enum DeviceType {
    UNDEFINED, LIGHTSTRIP, ROLLER_SHUTTER
  };

  class Device {
    public:
      static ThingDevice device;

    public:
      static ThingProperty deviceOn;
      static ThingProperty deviceBrightness;
      static ThingProperty deviceColor;
      static ThingProperty deviceColorMode;
      static ThingProperty deviceColorTemperature;

    public:
      static void load(void);
  };
}

#endif /* INCENDIO_DEVICE_H */
