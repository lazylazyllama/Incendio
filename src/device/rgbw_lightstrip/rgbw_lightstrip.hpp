#ifndef LUMOS_DEVICE_RGBW_LIGHTSTRIP_H
#define LUMOS_DEVICE_RGBW_LIGHTSTRIP_H

#define ARDUINOJSON_USE_LONG_LONG 1
#include <Thing.h>
#include <WebThingAdapter.h>

#include "../device.hpp"

namespace Lumos {
  class RgbwLightstrip : public Lumos::Device {
    private:
      ThingProperty onProperty;
      ThingProperty brightnessProperty;
      ThingProperty colorProperty;
      ThingProperty colorModeProperty;
      ThingProperty colorTemperatureProperty;

    public:
      RgbwLightstrip(const char *title);
      void handle(void) override;

    private:
      void updateColor(String *color, float const brightness);
      void updateWhite(float const brightness);
  };
}

#endif /* LUMOS_DEVICE_RGBW_LIGHTSTRIP_H */
