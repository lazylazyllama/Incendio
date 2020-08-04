#include "roller_shutter.hpp"

const unsigned char upOutputPin = 4;
const unsigned char downOutputPin = 15;
const unsigned char upButtonPin = 13;
const unsigned char downButtonPin = 5;

const char *deviceTypesRollerShutter[] = { nullptr };

Incendio::RollerShutter::RollerShutter(void)
  : Device(deviceTypesRollerShutter),
    stopProperty("stop", "Whether the shutter drives up", BOOLEAN, "OnOffProperty"),
    upProperty("up", "Whether the shutter drives up", BOOLEAN, "OnOffProperty"),
    downProperty("down", "Whether the shutter drives up", BOOLEAN, "OnOffProperty") {


  device.addProperty(&stopProperty);
  ThingPropertyValue stopValue;
  stopValue.boolean = State::stop;
  stopProperty.setValue(stopValue);
  stopProperty.title = "Stop";

  device.addProperty(&upProperty);
  ThingPropertyValue upValue;
  upValue.boolean = State::up;
  upProperty.setValue(upValue);
  upProperty.title = "Up";

  device.addProperty(&downProperty);
  ThingPropertyValue downValue;
  downValue.boolean = State::down;
  downProperty.setValue(downValue);
  downProperty.title = "Down";

  pinMode(upOutputPin, OUTPUT);
  digitalWrite(upOutputPin, LOW);
  pinMode(downOutputPin, OUTPUT);
  digitalWrite(downOutputPin, LOW);
  pinMode(upButtonPin, INPUT);
  pinMode(downButtonPin, INPUT);
};

void Incendio::RollerShutter::handle(void) {
  bool up = upProperty.getValue().boolean;
  bool down = downProperty.getValue().boolean;
  bool stop = stopProperty.getValue().boolean;

  /* if (digitalRead(upButtonPin) == HIGH) {
    Serial.println("Hardware Button Pressed: Up");
    if (down) {
      stop = true;
    } else {
      up = true;
    }
  } else if (digitalRead(downButtonPin) == HIGH) {
    Serial.println("Hardware Button Pressed: Down");
    if (up) {
      stop = true;
    } else {
      down = true;
    }
  } */

  if (stop && stop != State::stop) {
    digitalWrite(upOutputPin, LOW);
    digitalWrite(downOutputPin, LOW);
    up = false;
    down = false;
  } else if (up && up != State::up) {
    digitalWrite(downOutputPin, LOW);
    digitalWrite(upOutputPin, HIGH);
    down = false;
    stop = false;
  } else if (down && down != State::down) {
    digitalWrite(upOutputPin, LOW);
    digitalWrite(downOutputPin, HIGH);
    up = false;
    stop = false;
  }

  digitalWrite(LED_BUILTIN, stop ? HIGH : LOW);

  
  // Stop
  if (stop != State::stop) {
    Serial.print(device.id);
    Serial.print(": stop: ");
    Serial.println(stop);

    ThingPropertyValue stopValue;
    stopValue.boolean = stop;
    stopProperty.setValue(stopValue);

    State::stop = stop;
    State::save();
  }

  // Up
  if (up != State::up) {
    Serial.print(device.id);
    Serial.print(": up: ");
    Serial.println(up);

    ThingPropertyValue upValue;
    upValue.boolean = up;
    upProperty.setValue(upValue);

    State::up = up;
    State::save();
  }

  // Down
  if (down != State::down) {
    Serial.print(device.id);
    Serial.print(": down: ");
    Serial.println(down);

    ThingPropertyValue downValue;
    downValue.boolean = down;
    downProperty.setValue(downValue);

    State::down = down;
    State::save();
  }
}
