#define LARGE_JSON_BUFFERS 1

#include <Arduino.h>
#include <Thing.h>
#include <WebThingAdapter.h>

#include "ota/ota.h"
#include "wifi/wifi.h"
#include "state/state.h"

#ifdef ESP32
  #include <analogWrite.h>
#endif

WebThingAdapter *adapter;

String color = "#ffffff";
const char * colorModeEnum[3] = {"color", "temperature", nullptr};
String colorMode = colorModeEnum[0];

const unsigned char redPin = 12;
const unsigned char greenPin = 14;
const unsigned char bluePin = 13;
const unsigned char whitePin = 15;

void setupLamp(void) {
  pinMode(redPin, OUTPUT);
  digitalWrite(redPin, HIGH);
  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, HIGH);
  pinMode(bluePin, OUTPUT);
  digitalWrite(bluePin, HIGH);
  pinMode(whitePin, OUTPUT);
  digitalWrite(whitePin, HIGH);
}

void setup(void) {
  Serial.begin(115200);
  #ifdef ESP32
    btStop();
  #endif

  Incendio::Wifi::begin();
  Incendio::OTA::begin();
  Incendio::State::load();
  Incendio::Config::load();
  Incendio::Device::load();

  setupLamp();

  adapter = new WebThingAdapter("lasse-lightstrip-adapter", WiFi.localIP(), 443);
  
  Incendio::Device::device.addProperty(&Incendio::Device::deviceOn);
  ThingPropertyValue onValue;
  onValue.boolean = Incendio::State::on;
  Incendio::Device::deviceOn.setValue(onValue);

  Incendio::Device::device.addProperty(&Incendio::Device::deviceBrightness);
  ThingPropertyValue brightnessValue;
  brightnessValue.number = Incendio::State::brightness;
  Incendio::Device::deviceBrightness.setValue(brightnessValue);

  Incendio::Device::device.addProperty(&Incendio::Device::deviceColor);
  ThingPropertyValue colorValue;
  colorValue.string = &color;
  Incendio::Device::deviceColor.setValue(colorValue);

  Incendio::Device::deviceColorMode.propertyEnum = colorModeEnum;
  Incendio::Device::device.addProperty(&Incendio::Device::deviceColorMode);
  ThingPropertyValue colorModeValue;
  colorModeValue.string = &colorMode;
  Incendio::Device::deviceColorMode.setValue(colorModeValue);

  Incendio::Device::deviceColorTemperature.minimum = 1700;
  Incendio::Device::deviceColorTemperature.maximum = 6500;
  Incendio::Device::device.addProperty(&Incendio::Device::deviceColorTemperature);
  ThingPropertyValue colorTemperatureValue;
  colorTemperatureValue.integer = Incendio::State::colorTemperature;
  Incendio::Device::deviceColorTemperature.setValue(colorTemperatureValue);

  adapter->addDevice(&Incendio::Device::device);

  adapter->begin();
  Serial.println("Webthings HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(Incendio::Device::device.id);
}

void updateColor(String *color, float const brightness) {
  if (!color) {
    return;
  }
  float dim = brightness / 100.0;
  int red, green, blue;
  if (color && (color->length() == 7) && color->charAt(0) == '#') {
    const char *hex = 1 + (color->c_str()); // skip leading '#'
    sscanf(0 + hex, "%2x", &red);
    sscanf(2 + hex, "%2x", &green);
    sscanf(4 + hex, "%2x", &blue);
  }
  #ifdef ESP32
    analogWrite(redPin, red * dim, 255U);
    analogWrite(greenPin, green * dim, 255U);
    analogWrite(bluePin, blue * dim, 255U);
    analogWrite(whitePin, 0, 255U);
  #elif defined(ESP8266)
    analogWrite(redPin, red * dim);
    analogWrite(greenPin, green * dim);
    analogWrite(bluePin, blue * dim);
    analogWrite(whitePin, 0);
  #endif
}

void updateWhite(float const brightness) {
  int dim = map(brightness, 0, 100, 255, 0);
  #ifdef ESP32
    analogWrite(redPin, 0, 255U);
    analogWrite(greenPin, 0, 255U);
    analogWrite(bluePin, 0, 255U);
    analogWrite(whitePin, dim, 255U);
  #elif defined(ESP8266)
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    analogWrite(whitePin, dim);
  #endif
}

void loop(void) {
  Incendio::OTA::handle();

  adapter->update();

  bool on = Incendio::Device::deviceOn.getValue().boolean;
  float brightness = Incendio::Device::deviceBrightness.getValue().number;
  int colorTemperature = Incendio::Device::deviceColorTemperature.getValue().integer;
  if (colorMode.compareTo("color")) {
    updateColor(&color, on ? brightness : 0);
  } else if (colorMode.compareTo("temperature")) {
    updateWhite(on ? brightness : 0);
  }

  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  
  // On or off
  if (on != Incendio::State::on) {
    Serial.print(Incendio::Device::device.id);
    Serial.print(": on: ");
    Serial.println(on);

    Incendio::State::on = on;
    Incendio::State::save();
  }

  // Brightness
  if (brightness != Incendio::State::brightness) {
    Serial.print(Incendio::Device::device.id);
    Serial.print(", brightness: ");
    Serial.println(brightness);

    Incendio::State::brightness = brightness;
    Incendio::State::save();
  }

  // Color
  if (!color.equals(Incendio::State::color)) {
    Serial.print(Incendio::Device::device.id);
    Serial.print(", color: ");
    Serial.println(color);

    colorMode = colorModeEnum[0]; // colorMode = "color"
    Incendio::State::color = color;
    Incendio::State::save();
  }

  // Color Mode
  if (!colorMode.equals(Incendio::State::colorMode)) {
    Serial.print(Incendio::Device::device.id);
    Serial.print(", colorMode: ");
    Serial.println(colorMode);

    Incendio::State::colorMode = colorMode;
    Incendio::State::save();
  }

  // Color Temperature
  if (colorTemperature != Incendio::State::colorTemperature) {
    Serial.print(Incendio::Device::device.id);
    Serial.print(", colorTemperature: ");
    Serial.println(colorTemperature);

    colorMode = colorModeEnum[1]; // colorMode = "temperature"
    Incendio::State::colorTemperature = colorTemperature;
    Incendio::State::save();
  }
}
