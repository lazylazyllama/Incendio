#ifndef LUMOS_DEVICE_ROLLER_SHUTTER_H
#define LUMOS_DEVICE_ROLLER_SHUTTER_H

#define ARDUINOJSON_USE_LONG_LONG 1
#include <Thing.h>
#include <WebThingAdapter.h>

#include "../../sensors/ADE7953/ADE7953_I2C.hpp"
#include "../device.hpp"

namespace Lumos {
  class RollerShutter : public Lumos::Device {
    private:      
      ThingAction stopAction;
      ThingAction upAction;
      ThingAction downAction;

      ThingProperty powerProperty;
      ThingProperty currentProperty;
      ThingProperty voltageProperty;
      ThingProperty temperatureProperty;

    public:
      RollerShutter(const char *title);
      void handle(void) override;
  };
}

#endif /* LUMOS_DEVICE_ROLLER_SHUTTER_H */
