#include "rgbw_lightstrip.hpp"

String color = "#ffffff";
const char * colorModeEnum[3] = { "color", "temperature", nullptr };
String colorMode = colorModeEnum[0];

const unsigned char redPin = 12;
const unsigned char greenPin = 14;
const unsigned char bluePin = 13;
const unsigned char whitePin = 15;

const char *deviceTypesRgbwLightstrip[] = { "OnOffSwitch", "Light", "ColorControl", nullptr };

Incendio::RgbwLightstrip::RgbwLightstrip(void)
  : Device("RGBW Lightstrip", deviceTypesRgbwLightstrip),
    onProperty("on", "Whether the led is turned on", BOOLEAN, "OnOffProperty"),
    brightnessProperty("brithgness", "The brithgness of light from 0-100", NUMBER, "BrightnessProperty"),
    colorProperty("color", "The color of light in RGB", STRING, "ColorProperty"),
    colorModeProperty("colorMode", "The color mode of light", STRING, "ColorModeProperty"),
    colorTemperatureProperty("colorTemperature", "The color temperature of light", INTEGER, "ColorTemperatureProperty") {

  device.addProperty(&onProperty);
  ThingPropertyValue onValue;
  onValue.boolean = State::on;
  onProperty.setValue(onValue);

  device.addProperty(&brightnessProperty);
  ThingPropertyValue brightnessValue;
  brightnessValue.number = State::brightness;
  brightnessProperty.setValue(brightnessValue);

  device.addProperty(&colorProperty);
  ThingPropertyValue colorValue;
  colorValue.string = &color;
  colorProperty.setValue(colorValue);

  colorModeProperty.propertyEnum = colorModeEnum;
  device.addProperty(&colorModeProperty);
  ThingPropertyValue colorModeValue;
  colorModeValue.string = &colorMode;
  colorModeProperty.setValue(colorModeValue);

  colorTemperatureProperty.minimum = 1700;
  colorTemperatureProperty.maximum = 6500;
  device.addProperty(&colorTemperatureProperty);
  ThingPropertyValue colorTemperatureValue;
  colorTemperatureValue.integer = State::colorTemperature;
  colorTemperatureProperty.setValue(colorTemperatureValue);

  pinMode(redPin, OUTPUT);
  digitalWrite(redPin, HIGH);
  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, HIGH);
  pinMode(bluePin, OUTPUT);
  digitalWrite(bluePin, HIGH);
  pinMode(whitePin, OUTPUT);
  digitalWrite(whitePin, HIGH);
};

void Incendio::RgbwLightstrip::handle(void) {
  bool on = onProperty.getValue().boolean;
  float brightness = brightnessProperty.getValue().number;
  int colorTemperature = colorTemperatureProperty.getValue().integer;
  
  if (colorMode.compareTo("color")) {
    updateColor(&color, on ? brightness : 0);
  } else if (colorMode.compareTo("temperature")) {
    updateWhite(on ? brightness : 0);
  }

  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  
  // On or off
  if (on != State::on) {
    Serial.print(device.id);
    Serial.print(": on: ");
    Serial.println(on);

    State::on = on;
    State::save();
  }

  // Brightness
  if (brightness != State::brightness) {
    Serial.print(device.id);
    Serial.print(", brightness: ");
    Serial.println(brightness);

    State::brightness = brightness;
    State::save();
  }

  // Color
  if (!color.equals(State::color)) {
    Serial.print(device.id);
    Serial.print(", color: ");
    Serial.println(color);

    colorMode = colorModeEnum[0]; // colorMode = "color"
    State::color = color;
    State::save();
  }

  // Color Mode
  if (!colorMode.equals(State::colorMode)) {
    Serial.print(device.id);
    Serial.print(", colorMode: ");
    Serial.println(colorMode);

    State::colorMode = colorMode;
    State::save();
  }

  // Color Temperature
  if (colorTemperature != State::colorTemperature) {
    Serial.print(device.id);
    Serial.print(", colorTemperature: ");
    Serial.println(colorTemperature);

    colorMode = colorModeEnum[1]; // colorMode = "temperature"
    State::colorTemperature = colorTemperature;
    State::save();
  }
}

void Incendio::RgbwLightstrip::updateColor(String *color, float const brightness) {
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

void Incendio::RgbwLightstrip::updateWhite(float const brightness) {
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