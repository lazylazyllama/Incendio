#define LARGE_JSON_BUFFERS 1
#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <Thing.h>
#include <WebThingAdapter.h>

#include "ota/ota.hpp"
#include "wifi/wifi.hpp"
#include "state/state.hpp"
#include "device/device.hpp"
#include "device/rgbw_lightstrip/rgbw_lightstrip.hpp"
#include "device/roller_shutter/roller_shutter.hpp"

WebThingAdapter* adapter;
Incendio::Device* device;

const int port = 443;

void setup(void) {
  Serial.begin(115200);
  #ifdef ESP32
    btStop();
  #endif

  Incendio::Wifi::begin();
  Serial.println("Vodka 1");
  Incendio::OTA::begin();
  Serial.println("Vodka 2");
  Incendio::State::load();
  Serial.println("Vodka 3");
  Incendio::Config::load();
  Serial.println("Vodka 4");

  adapter = new WebThingAdapter("incendio-adapter", WiFi.localIP(), port);
  device = new Incendio::RollerShutter();
  Serial.println("Vodka 5");

  adapter->addDevice(&device->device);
  Serial.println("Vodka 6");

  adapter->begin();
  Serial.println("Webthings HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.print(port);
  Serial.print("/things/");
  Serial.println(device->device.id);
}

void loop(void) {
  Incendio::OTA::handle();

  adapter->update();
  device->handle();
}
