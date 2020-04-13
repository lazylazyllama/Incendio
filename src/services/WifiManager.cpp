#include "WifiManager.h"
#if __has_include("WifiConfig.h")
#include "WifiConfig.h"
#endif

#ifdef WIFI_SSID
const char *ssid = WIFI_SSID;
#else
const char *ssid = "";
#endif

#ifdef WIFI_PASSWORD
const char *password = WIFI_PASSWORD;
#else
const char *password = "";
#endif

void WifiManager::begin(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.print("Connecting to \"");
  Serial.print(ssid);
  Serial.println("\"");
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);

  //esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
  WiFi.setSleep(true); //this would use WIFI_PS_MIN_MODEM

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  bool blink = true;
  int connection_counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, blink ? LOW : HIGH); // active low led
    blink = !blink;

    connection_counter++;
    if (connection_counter == 10) {
      Serial.print("Try again");
      WiFi.reconnect();
    }
  }
  digitalWrite(LED_BUILTIN, HIGH); // active low led

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
