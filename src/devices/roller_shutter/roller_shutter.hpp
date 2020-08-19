#ifndef LUMOS_DEVICE_ROLLER_SHUTTER_H
#define LUMOS_DEVICE_ROLLER_SHUTTER_H

#include "../../sensors/ADE7953/ADE7953_I2C.hpp"
#include "../../sensors/ntc/ntc.hpp"
#include "../device.hpp"

namespace Lumos {
  class RollerShutter : public Lumos::Device {
    private:
      ThingAction stopAction;
      ThingAction upAction;
      ThingAction downAction;

      ThingProperty powerProperty;
      ThingProperty temperatureProperty;

    public:
      RollerShutter(const char *title);
      void handle(void) override;
  };
}

#endif /* LUMOS_DEVICE_ROLLER_SHUTTER_H */
