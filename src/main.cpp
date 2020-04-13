#define LARGE_JSON_BUFFERS 1

#include <Arduino.h>
#include <Thing.h>
#include <WebThingAdapter.h>

#include "services/WifiManager.h"
#include "services/OTAManager.h"
#include "services/ConfigManager.h"

#ifdef ESP32
#include <analogWrite.h>
#endif

WebThingAdapter *adapter;

const char *deviceTypes[] = {"OnOffSwitch", "Light", "ColorControl", nullptr};
ThingDevice device("lasse-lightstrip", "Lasse Lightstrip", deviceTypes);

ThingProperty deviceOn("on", "Whether the led is turned on", BOOLEAN, "OnOffProperty");
ThingProperty deviceBrightness("brithgness", "The brithgness of light from 0-100", NUMBER, "BrightnessProperty");
ThingProperty deviceColor("color", "The color of light in RGB", STRING, "ColorProperty");
ThingProperty deviceColorMode("colorMode", "The color mode of light", STRING, "ColorModeProperty");
ThingProperty deviceColorTemperature("colorTemperature", "The color temperature of light", INTEGER, "ColorTemperatureProperty");

String color = "#ffffff";
const char * colorModeEnum[3] = {"color", "temperature", nullptr};
String colorMode = colorModeEnum[0];

ConfigData data;

const unsigned char redPin = 12;
const unsigned char greenPin = 13;
const unsigned char bluePin = 14;

void setupLamp(void) {
  pinMode(redPin, OUTPUT);
  digitalWrite(redPin, HIGH);
  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, HIGH);
  pinMode(bluePin, OUTPUT);
  digitalWrite(bluePin, HIGH);
}

void setup(void) {
  Serial.begin(115200);
  #ifdef ESP32
    btStop();
  #endif
  WifiManager::begin();
  OTAManager::begin();

  setupLamp();
  data = ConfigManager::load();

  adapter = new WebThingAdapter("lasse-lightstrip-adapter", WiFi.localIP());
  
  device.addProperty(&deviceOn);
  ThingPropertyValue onValue;
  onValue.boolean = data.lastOn;
  deviceOn.setValue(onValue);

  device.addProperty(&deviceBrightness);
  ThingPropertyValue brightnessValue;
  brightnessValue.number = data.lastBrightness;
  deviceBrightness.setValue(brightnessValue);

  device.addProperty(&deviceColor);
  ThingPropertyValue colorValue;
  colorValue.string = &color;
  deviceColor.setValue(colorValue);

  deviceColorMode.propertyEnum = colorModeEnum;
  device.addProperty(&deviceColorMode);
  ThingPropertyValue colorModeValue;
  colorModeValue.string = &colorMode;
  deviceColorMode.setValue(colorModeValue);

  deviceColorTemperature.minimum = 1700;
  deviceColorTemperature.maximum = 6500;
  device.addProperty(&deviceColorTemperature);
  ThingPropertyValue colorTemperatureValue;
  colorTemperatureValue.integer = data.lastColorTemperature;
  deviceColorTemperature.setValue(colorTemperatureValue);

  adapter->addDevice(&device);

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(device.id);
}

void update(String *color, int const brightness) {
  if (!color)
    return;
  float dim = brightness / 100.0;
  int red, green, blue;
  if (color && (color->length() == 7) && color->charAt(0) == '#') {
    const char *hex = 1 + (color->c_str()); // skip leading '#'
    sscanf(0 + hex, "%2x", &red);
    sscanf(2 + hex, "%2x", &green);
    sscanf(4 + hex, "%2x", &blue);
  }
  analogWrite(redPin, red * dim, 255U);
  analogWrite(greenPin, green * dim, 255U);
  analogWrite(bluePin, blue * dim, 255U);
}

void loop(void) {
  if (WiFi.status() != WL_CONNECTED) {
    WifiManager::begin();
  }
  OTAManager::handle();

  adapter->update();

  bool on = deviceOn.getValue().boolean;
  int brightness = deviceBrightness.getValue().number;
  int colorTemperature = deviceColorTemperature.getValue().integer;
  update(&color, on ? brightness : 0);

  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  if (on != data.lastOn) {
    Serial.print(device.id);
    Serial.print(": on: ");
    Serial.println(on);

    data.lastOn = on;
    ConfigManager::save(data);
  }
  if (brightness != data.lastBrightness) {
    Serial.print(device.id);
    Serial.print(", brightness: ");
    Serial.println(brightness);

    data.lastBrightness = brightness;
    ConfigManager::save(data);
  }
  if (color.compareTo(data.lastColor)) {
    Serial.print(device.id);
    Serial.print(", color: ");
    Serial.println(color);

    colorMode = colorModeEnum[0]; // colorMode = "color"
    data.lastColor = color;
    ConfigManager::save(data);
  }
  if (colorMode.compareTo(data.lastColorMode)) {
    Serial.print(device.id);
    Serial.print(", colorMode: ");
    Serial.println(colorMode);

    data.lastColorMode = colorMode;
    ConfigManager::save(data);
  }
  if (colorTemperature != data.lastColorTemperature) {
    Serial.print(device.id);
    Serial.print(", colorTemperature: ");
    Serial.println(colorTemperature);

    colorMode = colorModeEnum[1]; // colorMode = "temperature"
    data.lastColorTemperature = colorTemperature;
    ConfigManager::save(data);
  }
}
