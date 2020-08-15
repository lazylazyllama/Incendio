#include "wifi.hpp"

static AsyncWebServer webServer(80);
static DNSServer dnsServer;
static AsyncWiFiManager wifiManager(&webServer,&dnsServer);

AsyncWiFiManagerParameter customDeviceNameParameter("");

void Lumos::Wifi::begin(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Lumos::Config::load();

  customDeviceNameParameter = AsyncWiFiManagerParameter("device name", "Custom device name e.g. Kitchen Light", Lumos::Config::deviceName.c_str(), 40);
  
  wifiManager.addParameter(&customDeviceNameParameter);
  wifiManager.setCustomHeadElement(HTTP_CUSTOM_STYLE);
  wifiManager.setSaveConfigCallback(saveCustomParameters);

  wifiManager.autoConnect("Webthings Device Setup", WIFI_AP_PASSWORD);
  while (WiFi.status() != WL_CONNECTED);
  
  String hostName = "Webthings Device";
  if (!Lumos::Config::deviceName.equals("")) {
    hostName.concat(": ");
    hostName.concat(Lumos::Config::deviceName);
  }
  
  #ifdef ESP32
    WiFi.setHostname(hostName.c_str());
    esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
    WiFi.setSleep(true); //this would use WIFI_PS_MIN_MODEM
  #elif defined(ESP8266)
    /* WiFi.hostname(hostName.c_str());
    WiFi.setSleepMode(WIFI_MODEM_SLEEP);
    WiFi.forceSleepBegin(); */
  #endif
  
  digitalWrite(LED_BUILTIN, LOW);
}

void Lumos::Wifi::saveCustomParameters(void) {
  Lumos::Config::deviceName = customDeviceNameParameter.getValue();
  Lumos::Config::save();
}