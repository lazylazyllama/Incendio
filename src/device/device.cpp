#include "device.h"

void Incendio::Device::load() {
  const char *deviceTypes[] = {"OnOffSwitch", "Light", "ColorControl", nullptr};
  Device::device = ThingDevice("lightstrip", "Lightstrip", deviceTypes);

  Device::deviceOn = ThingProperty("on", "Whether the led is turned on", BOOLEAN, "OnOffProperty");
  Device::deviceBrightness = ThingProperty("brithgness", "The brithgness of light from 0-100", NUMBER, "BrightnessProperty");
  Device::deviceColor = ThingProperty("color", "The color of light in RGB", STRING, "ColorProperty");
  Device::deviceColorMode = ThingProperty("colorMode", "The color mode of light", STRING, "ColorModeProperty");
  Device::deviceColorTemperature = ThingProperty("colorTemperature", "The color temperature of light", INTEGER, "ColorTemperatureProperty");
}