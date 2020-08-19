#ifndef LUMOS_DEVICE_RGBW_LIGHTSTRIP_H
#define LUMOS_DEVICE_RGBW_LIGHTSTRIP_H

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
