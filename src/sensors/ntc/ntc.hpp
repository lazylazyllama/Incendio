#ifndef LUMOS_SENSORS_NTC_H
#define LUMOS_SENSORS_NTC_H

#include <Arduino.h>

namespace Lumos {
  class NtcSensor {
    private:
      unsigned long beta;
      unsigned long r21;
      unsigned long r0;
      double t0;

    public:
      NtcSensor(unsigned long beta, unsigned long r21, unsigned long r0, double t0)
        : beta(beta), r21(r21), r0(r0), t0(t0) {}
      
    public:
      double getTemperature();
  };
}

#endif /* LUMOS_SENSORS_NTC_H */
