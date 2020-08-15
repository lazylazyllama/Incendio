#ifndef LUMOS_SENSORS_ADE7953_H
#define LUMOS_SENSORS_ADE7953_H

#include <Arduino.h>
#include <Wire.h>
#include <math.h>

namespace Lumos {
  class ADE7953Sensor {
    private:
      const uint8_t address = 0x38;
      const int sda;
      const int scl;

    public:
      ADE7953Sensor(int sda, int scl) : sda(sda), scl(scl) {}
      
    public:
      void init();
      float getPower();
      float getCurrent();
      int getVoltage();
      

    private:
      void write(unsigned char address, uint16_t reg, uint32_t val);
      uint32_t read(int address, uint16_t reg);
      int regSize(const uint16_t reg);
  };
}

#endif /* LUMOS_SENSORS_ADE7953_H */
