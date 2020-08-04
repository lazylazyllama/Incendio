#ifndef INCENDIO_DEVICE_RGBW_LIGHTSTRIP_H
#define INCENDIO_DEVICE_RGBW_LIGHTSTRIP_H

#define ARDUINOJSON_USE_LONG_LONG 1
#include <Thing.h>
#include <WebThingAdapter.h>

#include "../device.hpp"

namespace Incendio {
  class RgbwLightstrip : public Incendio::Device {
    private:
      ThingProperty onProperty;
      ThingProperty brightnessProperty;
      ThingProperty colorProperty;
      ThingProperty colorModeProperty;
      ThingProperty colorTemperatureProperty;

    public:
      RgbwLightstrip(void);
      void handle(void) override;

    private:
      void updateColor(String *color, float const brightness);
      void updateWhite(float const brightness);
  };
}

#endif /* INCENDIO_DEVICE_RGBW_LIGHTSTRIP_H */
