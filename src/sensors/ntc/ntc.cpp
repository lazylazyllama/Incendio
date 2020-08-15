#include "ntc.hpp"

#define ANALOG_V33   3.3

double Lumos::NtcSensor::getTemperature() {
  double read = analogRead(A0);

  double Rt = (read * r21) / (1024.0 * ANALOG_V33 - (double) read);
  double T = beta / (beta/t0 + log(Rt/r0));
  double temperature = T - 273.15;

  return temperature;
}