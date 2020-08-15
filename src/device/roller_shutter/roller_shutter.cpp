#include "roller_shutter.hpp"

const unsigned char upOutputPin = 4;
const unsigned char downOutputPin = 15;
const unsigned char upInputPin = 13;
const unsigned char downInputPin = 5;

// Init motor timer
unsigned long startedDrivingMillis = 0;

// Define adc temperature values
#define TO_KELVIN(x) ((x) + 273.15)
#define TO_CELSIUS(x) ((x) - 273.15)
#define ANALOG_V33   3.3
#define ANALOG_R21   32000.0
#define ANALOG_R0    10000.0
#define ANALOG_T0    TO_KELVIN(25.0)
#define ANALOG_B     3350.0

// Create power consumption sensor
Lumos::ADE7953 ade7953(12, 14);

// Webthings device types
const char *deviceTypesRollerShutter[] = { nullptr };

enum DrivingMode {
  STOP, UP, DOWN
};

void setDrivingMode(DrivingMode mode) {
  switch (mode) {
    case DrivingMode::STOP:
      Serial.println("Driving Mode => STOP");
      digitalWrite(upOutputPin, LOW);
      digitalWrite(downOutputPin, LOW);
      break;
    case DrivingMode::UP:
      Serial.println("Driving Mode => UP");
      startedDrivingMillis = millis();
      digitalWrite(downOutputPin, LOW);
      digitalWrite(upOutputPin, HIGH);
      break;
    case DrivingMode::DOWN:
      Serial.println("Driving Mode => DOWN");
      startedDrivingMillis = millis();
      digitalWrite(upOutputPin, LOW);
      digitalWrite(downOutputPin, HIGH);
      break;
  }
}

void ICACHE_RAM_ATTR hardwareButtonChangedISR() {
  if (digitalRead(upInputPin) == HIGH && digitalRead(downInputPin) == HIGH) {
    setDrivingMode(DrivingMode::STOP);
  } else if (digitalRead(upInputPin) == HIGH) {
    setDrivingMode(DrivingMode::UP);
  } else if (digitalRead(downInputPin) == HIGH) {
    setDrivingMode(DrivingMode::DOWN);
  } else {
    setDrivingMode(DrivingMode::STOP);
  }
}

void do_stop(const JsonVariant &input) {
  setDrivingMode(DrivingMode::STOP);
}

void do_up(const JsonVariant &input) {
  setDrivingMode(DrivingMode::UP);
}

void do_down(const JsonVariant &input) {
  setDrivingMode(DrivingMode::DOWN);
}

ThingActionObject *stop_action_generator(DynamicJsonDocument *input) {
  return new ThingActionObject("stop", input, do_stop, nullptr);
}

ThingActionObject *up_action_generator(DynamicJsonDocument *input) {
  return new ThingActionObject("up", input, do_up, nullptr);
}

ThingActionObject *down_action_generator(DynamicJsonDocument *input) {
  return new ThingActionObject("down", input, do_down, nullptr);
}

Lumos::RollerShutter::RollerShutter(const char *title)
  : Device(title, deviceTypesRollerShutter),
    stopAction("stop", "Stop", "Whether the shutter stops driving", "ToggleAction", nullptr, stop_action_generator),
    upAction("up", "Up", "Whether the shutter drives up", "ToggleAction", nullptr, up_action_generator),
    downAction("down", "Down", "Whether the shutter drives down", "ToggleAction", nullptr, down_action_generator),
    powerProperty("power", "The current power consumption in watt", NUMBER, "InstantaneousPowerProperty"),
    currentProperty("current", "The current current in ampere", NUMBER, "CurrentProperty"),
    voltageProperty("voltage", "The current voltage in volt", NUMBER, "VoltageProperty"),
    temperatureProperty("temperature", "The current temperature of the device in °C", NUMBER, "TemperatureProperty") {

  // Add actions
  device.addAction(&stopAction);
  device.addAction(&upAction);
  device.addAction(&downAction);

  // Add properties
  device.addProperty(&powerProperty);
  ThingPropertyValue powerValue;
  powerValue.number = 0.0;
  powerProperty.setValue(powerValue);

  device.addProperty(&currentProperty);
  ThingPropertyValue currentValue;
  currentValue.number = 0.0;
  currentProperty.setValue(currentValue);

  device.addProperty(&voltageProperty);
  ThingPropertyValue voltageValue;
  voltageValue.number = 0.0;
  voltageProperty.setValue(voltageValue);

  device.addProperty(&temperatureProperty);
  ThingPropertyValue temperatureValue;
  temperatureValue.number = 0.0;
  temperatureProperty.setValue(temperatureValue);

  // Init pins
  pinMode(upOutputPin, OUTPUT);
  digitalWrite(upOutputPin, LOW);

  pinMode(downOutputPin, OUTPUT);
  digitalWrite(downOutputPin, LOW);

  pinMode(upInputPin, INPUT);
  pinMode(downInputPin, INPUT);

  // Init interrupts
  attachInterrupt(digitalPinToInterrupt(upInputPin), hardwareButtonChangedISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(downInputPin), hardwareButtonChangedISR, CHANGE);

  // Init ADE7953 power sensor
  ade7953.init();
};

void Lumos::RollerShutter::handle(void) {
  unsigned long currentMillis = millis();

  // TODO handle millis() overflow
  // Stop motor after 1 minute
  if ((currentMillis - startedDrivingMillis) >= 60000 && startedDrivingMillis != 0) {
    Serial.println("Driving for 1 minute already => STOP");
    startedDrivingMillis = 0;
    digitalWrite(upOutputPin, LOW);
    digitalWrite(downOutputPin, LOW);
  }

  // Only update sensors all 10 seconds
  static unsigned long last10sMillis = millis();
  if ((currentMillis - last10sMillis) >= 10000) {
    last10sMillis = currentMillis;

    // Power consumption
    static int old_power = 0.0;
    float power = ade7953.getPower();
    float current = ade7953.getCurrent();
    int voltage = ade7953.getVoltage();
    if (true) {
      old_power = power;

      ThingPropertyValue powerValue;
      powerValue.number = power;
      powerProperty.setValue(powerValue);

      ThingPropertyValue currentValue;
      currentValue.number = current;
      currentProperty.setValue(currentValue);

      ThingPropertyValue voltageValue;
      voltageValue.number = voltage;
      voltageProperty.setValue(voltageValue);

      Serial.print("Update power consumption: ");
      Serial.print(power);
      Serial.println("W");

      Serial.print("Update current consumption: ");
      Serial.print(current);
      Serial.println("I");

      Serial.print("Update voltage consumption: ");
      Serial.print(voltage);
      Serial.println("V");
    }

    // Temperature
    static int old_adc = 0;
    int adc = analogRead(A0);
    if ((adc - old_adc) >= 20 || (adc - old_adc) <= 20) {
      old_adc = adc;
      double Rt = (adc * ANALOG_R21) / (1024.0 * ANALOG_V33 - (double)adc);
      double T = ANALOG_B / (ANALOG_B/ANALOG_T0 + log(Rt/ANALOG_R0));
      double temp = TO_CELSIUS(T);

      ThingPropertyValue temperatureValue;
      temperatureValue.number = temp;
      temperatureProperty.setValue(temperatureValue);

      Serial.print("Update temperature: ");
      Serial.print(temp);
      Serial.println("°C");
    }
  }
}
