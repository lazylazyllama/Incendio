#ifndef INCENDIO_DEVICE_ROLLER_SHUTTER_H
#define INCENDIO_DEVICE_ROLLER_SHUTTER_H

#define ARDUINOJSON_USE_LONG_LONG 1
#include <Thing.h>
#include <WebThingAdapter.h>

#include "../device.hpp"

namespace Incendio {
  class RollerShutter : public Incendio::Device {
    private:      
      ThingAction stopAction;
      ThingAction upAction;
      ThingAction downAction;

    public:
      RollerShutter(void);
      void handle(void) override;
  };
}

#endif /* INCENDIO_DEVICE_ROLLER_SHUTTER_H */
