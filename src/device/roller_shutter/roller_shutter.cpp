#include "roller_shutter.hpp"

const unsigned char upOutputPin = 4;
const unsigned char downOutputPin = 15;
const unsigned char upInputPin = 13;
const unsigned char downInputPin = 5;

// Init motor timer
unsigned long startedDrivingMillis = 0;

// Create power consumption sensor
Lumos::ADE7953Sensor ade7953Sensor(12, 14);
Lumos::NtcSensor ntcSensor(3350, 32000, 8000, (273.15 + 25.0));

// Webthings device types
const char *deviceTypesRollerShutter[] = { nullptr };

enum DrivingMode {
  STOP, UP, DOWN
};

void setDrivingMode(DrivingMode mode) {
  switch (mode) {
    case DrivingMode::STOP:
      Serial.println("Driving Mode => STOP");
      startedDrivingMillis = 0;
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
  ade7953Sensor.init();
};

void Lumos::RollerShutter::handle(void) {
  unsigned long currentMillis = millis();

  // TODO handle millis() overflow
  // Stop motor after 1 minute
  if ((currentMillis - startedDrivingMillis) >= (120000) && startedDrivingMillis != 0) {
    Serial.println("Driving for 2 minutes already => STOP");
    setDrivingMode(DrivingMode::STOP);
  }

  // Only update sensors all 10 seconds
  static unsigned long last10sMillis = millis();
  if ((currentMillis - last10sMillis) >= 10000) {
    last10sMillis = currentMillis;

    // Power consumption
    static float old_power = 0.0;
    float power = ade7953Sensor.getPower();
    if (abs(power - old_power) >= 2) {
      old_power = power;

      ThingPropertyValue powerValue;
      powerValue.number = power;
      powerProperty.setValue(powerValue);

      Serial.print("Update power consumption: ");
      Serial.print(power);
      Serial.println("W");
    }

    // Auto relay stop by power consumption
    if (power < 10) {
      setDrivingMode(DrivingMode::STOP);
    }

    // Temperature
    static double old_temperature = 0.0;
    float temperature = ntcSensor.getTemperature();
    if (abs(temperature - old_temperature) >= 20) {
      old_temperature = temperature;

      ThingPropertyValue temperatureValue;
      temperatureValue.number = temperature;
      temperatureProperty.setValue(temperatureValue);

      Serial.print("Update temperature: ");
      Serial.print(temperature);
      Serial.println("°C");
    }
  }
}
