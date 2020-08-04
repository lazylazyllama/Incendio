#include "wifi.h"

#define wifiBackupFileName "/wifiBackup.json"
#define wifiBackupFileSize 512

static AsyncWebServer webServer(80);
static DNSServer dnsServer;
static AsyncWiFiManager wifiManager(&webServer,&dnsServer);

String customDeviceName = "";
AsyncWiFiManagerParameter customDeviceNameParameter("");

void Incendio::Wifi::begin(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  loadCustomParameters();
  customDeviceNameParameter = AsyncWiFiManagerParameter("device name", "Custom device name e.g. Kitchen Light", customDeviceName.c_str(), 40);
  
  wifiManager.addParameter(&customDeviceNameParameter);
  wifiManager.setCustomHeadElement(HTTP_CUSTOM_STYLE);
  wifiManager.setSaveConfigCallback(saveCustomParameters);

  wifiManager.autoConnect("Webthings Setup", "password");
  while (WiFi.status() != WL_CONNECTED);
  
  String hostName = "Webthings Device";
  if (!customDeviceName.equals("")) {
    hostName.concat(": ");
    hostName.concat(customDeviceName);
  }
  WiFi.setHostname(hostName.c_str());
  #ifdef ESP32
    esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
    WiFi.setSleep(true); //this would use WIFI_PS_MIN_MODEM
  #elif defined(ESP8266)
    WiFi.setSleepMode(WIFI_MODEM_SLEEP);
    WiFi.forceSleepBegin();
  #endif
  
  digitalWrite(LED_BUILTIN, LOW);
}

String Incendio::Wifi::getCustomDeviceName(void) {
  return customDeviceName;
}

void Incendio::Wifi::loadCustomParameters() {
  #ifdef ESP32
    bool ok = SPIFFS.begin(true);
  #elif defined(ESP8266)
    bool ok = SPIFFS.begin();
  #endif

  if(!ok) {
    SPIFFS.format();
  }

  File file = SPIFFS.open(wifiBackupFileName, FILE_READ);

  StaticJsonDocument<wifiBackupFileSize> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read file, using default configuration"));
  }

  customDeviceName = doc["customDeviceName"] | "";

  file.close();

  SPIFFS.end();
}

void Incendio::Wifi::saveCustomParameters(void) {
  #ifdef ESP32
    bool ok = SPIFFS.begin(true);
  #elif defined(ESP8266)
    bool ok = SPIFFS.begin();
  #endif

  if(!ok) {
    SPIFFS.format();
  }

  SPIFFS.remove(wifiBackupFileName);

  File file = SPIFFS.open(wifiBackupFileName, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  StaticJsonDocument<wifiBackupFileSize> doc;

  customDeviceName = customDeviceNameParameter.getValue();
  doc["customDeviceName"] = customDeviceNameParameter.getValue();

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  SPIFFS.end();
}