#include "roller_shutter.hpp"

const unsigned char upOutputPin = 4;
const unsigned char downOutputPin = 15;
const unsigned char upInputPin = 13;
const unsigned char downInputPin = 5;

// Init motor timer
unsigned long startedDrivingMillis = 0;

// Define I2C pins for power consumption sensor
#define local_CLK 14
#define local_CS 12

// Define adc temperature values
#define TO_KELVIN(x) ((x) + 273.15)
#define TO_CELSIUS(x) ((x) - 273.15)
#define ANALOG_V33   3.3
#define ANALOG_R21   32000.0
#define ANALOG_R0    10000.0
#define ANALOG_T0    TO_KELVIN(25.0)
#define ANALOG_B     3350.0

// Create power consumption sensor
ADE7953 myADE7953(local_CLK, local_CS);

// Webthings device types
const char *deviceTypesRollerShutter[] = { nullptr };

void ICACHE_RAM_ATTR hardwareButtonChangedISR() {
  if (digitalRead(upInputPin) == HIGH && digitalRead(downInputPin) == HIGH) {
    Serial.println("Both input pins HIGH => STOP");
    digitalWrite(upOutputPin, LOW);
    digitalWrite(downOutputPin, LOW);
  } else if (digitalRead(upInputPin) == HIGH) {
    Serial.println("Only up input pin HIGH => DRIVE UP");
    startedDrivingMillis = millis();
    digitalWrite(upOutputPin, HIGH);
    digitalWrite(downOutputPin, LOW);
  } else if (digitalRead(downInputPin) == HIGH) {
    Serial.println("Only down input pin HIGH => DRIVE DOWN");
    startedDrivingMillis = millis();
    digitalWrite(upOutputPin, LOW);
    digitalWrite(downOutputPin, HIGH);
  } else {
    Serial.println("Both input pins LOW => STOP");
    digitalWrite(upOutputPin, LOW);
    digitalWrite(downOutputPin, LOW);
  }
}

void do_stop(const JsonVariant &input) {
  Serial.println("STOP");

  digitalWrite(upOutputPin, LOW);
  digitalWrite(downOutputPin, LOW);

  digitalWrite(LED_BUILTIN, HIGH);
}

void do_up(const JsonVariant &input) {
  Serial.println("UP");
  
  digitalWrite(upOutputPin, HIGH);
  digitalWrite(downOutputPin, LOW);

  digitalWrite(LED_BUILTIN, LOW);
}

void do_down(const JsonVariant &input) {
  Serial.println("DOWN");
  
  digitalWrite(upOutputPin, LOW);
  digitalWrite(downOutputPin, HIGH);

  digitalWrite(LED_BUILTIN, LOW);
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

Incendio::RollerShutter::RollerShutter(const char *title)
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
  /* delay(200);
  myADE7953.initialize(); */
};

void Incendio::RollerShutter::handle(void) {
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

    /* // Power consumption
    static float old_activePowerA = 0.0;
    float activePowerA = myADE7953.getInstActivePowerA();
    if (true) {
      old_activePowerA = activePowerA;

      ThingPropertyValue powerValue;
      powerValue.number = activePowerA;
      powerProperty.setValue(powerValue);

      Serial.print("Update power consumption: ");
      Serial.print(activePowerA);
      Serial.println("W");
    } */

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
