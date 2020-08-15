#include "ADE7953_I2C.hpp"

#define ADE7953_PREF            1540.0
#define ADE7953_UREF            26000
#define ADE7953_IREF            10000.0

void Lumos::ADE7953Sensor::init() {
  delay(100);

  Wire.begin(sda, scl);
}

float Lumos::ADE7953Sensor::getPower() {
  return getCurrent() * getVoltage();
}

float Lumos::ADE7953Sensor::getCurrent() {
  return max(read(address, 0x31A), read(address, 0x31B)) / (ADE7953_IREF * 10);
}

int Lumos::ADE7953Sensor::getVoltage() {
  return read(address, 0x31C) / ADE7953_UREF;
}

uint32_t Lumos::ADE7953Sensor::read(int address, uint16_t reg) {
  uint32_t response = 0;
  const int size = regSize(reg);
  if (size) {
    Wire.beginTransmission(address);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    Wire.endTransmission(0);
    Wire.requestFrom(address, size);
    if (size <= Wire.available()) {
      for (int i = 0; i < size; i++) {
        response = response << 8 | Wire.read();   // receive DATA (MSB first)
      }
    }
  }
  return response;
}

int Lumos::ADE7953Sensor::regSize(const uint16_t reg) {
  const uint8_t mask = ((reg >> 8) & 0b1111);
  if (!mask || (mask & 0b1100)) {
    return 1;
  } else if (mask & 0b0011) {
    return mask + 1;
  }
  return 0;
}
