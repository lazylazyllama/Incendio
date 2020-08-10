#include "roller_shutter.hpp"

const unsigned char upOutputPin = 4;
const unsigned char downOutputPin = 15;
const unsigned char upInputPin = 13;
const unsigned char downInputPin = 5;

const char *deviceTypesRollerShutter[] = { nullptr };

void hardwareButtonChangedISR() {
  if (digitalRead(upInputPin) == HIGH && digitalRead(downInputPin) == HIGH) {
    Serial.println("Both input pins HIGH => STOP");
    digitalWrite(upOutputPin, LOW);
    digitalWrite(downOutputPin, LOW);
  } else if (digitalRead(upInputPin) == HIGH) {
    Serial.println("Only up input pin HIGH => DRIVE UP");
    digitalWrite(upOutputPin, HIGH);
    digitalWrite(downOutputPin, LOW);
  } else if (digitalRead(downInputPin) == HIGH) {
    Serial.println("Only down input pin HIGH => DRIVE DOWN");
    digitalWrite(upOutputPin, LOW);
    digitalWrite(downOutputPin, HIGH);
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

Incendio::RollerShutter::RollerShutter(void)
  : Device("Roller Shutter", deviceTypesRollerShutter),
    stopAction("stop", "Stop", "Whether the shutter stops driving", "ToggleAction", nullptr, stop_action_generator),
    upAction("up", "Up", "Whether the shutter drives up", "ToggleAction", nullptr, up_action_generator),
    downAction("down", "Down", "Whether the shutter drives down", "ToggleAction", nullptr, down_action_generator) {

  // Add actions
  device.addAction(&stopAction);
  device.addAction(&upAction);
  device.addAction(&downAction);

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
};

void Incendio::RollerShutter::handle(void) {
  // Nothing
}
