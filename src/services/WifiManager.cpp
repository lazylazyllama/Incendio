#include "WifiManager.h"

static AsyncWebServer webServer(80);
static DNSServer dnsServer;
static AsyncWiFiManager wifiManager(&webServer,&dnsServer);

AsyncWiFiManagerParameter custom_device_name("device name", "Custom device name e.g. Kitchen Light", "", 40);

void WifiManager::begin(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  wifiManager.addParameter(&custom_device_name);
  wifiManager.setCustomHeadElement(HTTP_CUSTOM_STYLE);

  wifiManager.autoConnect("Webthing Setup", "password");
  while (WiFi.status() != WL_CONNECTED);

  WiFi.setHostname("Webthings device");
  #ifdef ESP32
    esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
    WiFi.setSleep(true); //this would use WIFI_PS_MIN_MODEM
  #elif defined(ESP8266)
    WiFi.setSleepMode(WIFI_MODEM_SLEEP);
    WiFi.forceSleepBegin();
  #endif

  Serial.println("VOOOOOOOOOOOOOOOOODKA");
  Serial.println(custom_device_name.getValue());
  
  digitalWrite(LED_BUILTIN, LOW);
}

String WifiManager::getCustomDeviceName(void) {
  return custom_device_name.getValue();
}