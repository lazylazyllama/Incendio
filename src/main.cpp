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
Lumos::Device* device;

const int port = 443;

void setup(void) {
  Serial.begin(115200);
  #ifdef ESP32
    btStop();
  #endif

  Lumos::Wifi::begin();
  Lumos::OTA::begin();
  Lumos::State::load();
  Lumos::Config::load();

  adapter = new WebThingAdapter("lumos-adapter", WiFi.localIP(), port);
  switch (Lumos::Config::deviceType) {
    case Lumos::DeviceType::UNDEFINED:
      // TODO Remove this code and replace it with something better
      device = new Lumos::RollerShutter(Lumos::Config::deviceName.c_str());
      break;

    case Lumos::DeviceType::RGBW_LIGHTSTRIP:
      device = new Lumos::RgbwLightstrip(Lumos::Config::deviceName.c_str());
      break;

    case Lumos::DeviceType::ROLLER_SHUTTER:
      device = new Lumos::RollerShutter(Lumos::Config::deviceName.c_str());
      break;
  }
  

  adapter->addDevice(&device->device);

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
  Lumos::OTA::handle();

  adapter->update();
  device->handle();
}
