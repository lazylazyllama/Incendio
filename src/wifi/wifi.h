#ifndef LUXIO_WIFI_H
#define LUXIO_WIFI_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>

#include <ArduinoJson.h>

#ifdef ESP32
  #include <WiFi.h>
  #include <esp_wifi.h>
  #include <esp_wifi_types.h>
  #include <SPIFFS.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <SPI.h>
  #include <FS.h>
  #define FILE_READ       "r"
  #define FILE_WRITE      "w"
  #define FILE_APPEND     "a"
#endif

namespace Incendio {
    class Wifi {
    public:
        static void begin(void);
        static String getCustomDeviceName(void);

    private:
        static void loadCustomParameters(void);
        static void saveCustomParameters(void);
    };

    const char HTTP_CUSTOM_STYLE[] PROGMEM = R"rawliteral(
    <style>
        .c {
            text-align: center;
        }

        
    </style>
    <style>
        html {
            font-family: "Courier New", Courier, monospace;
            background-color: #000;
            color: #fff;
        }
        
        body {
            margin: 0;
        }

        *::-moz-focus-inner {
            border: 0;
        }

        
        a {
            display: inline-block;
            margin-left: -5px;
            padding: 0.4rem;
            border-radius: 0.3rem;
            background-color: #292929;
            color: #fff;
            text-decoration: none;
        }

        .q {
            float: right;
            width: 64px;
            text-align: right;
            margin-right: -5px;
            padding: 0.4rem;
            padding-right: 0;
        }

        .l {
            background: none;
        }
        .l::before {
            display: inline-block;
            content: "l";
            width: 1rem;
            background-color: #fff;
            -webkit-mask-image: url("data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMjQiIHZpZXdCb3g9IjAgMCAyNCAyNCIgd2lkdGg9IjI0Ij48cGF0aCBkPSJNMCAwaDI0djI0SDB6IiBmaWxsPSJub25lIi8+PHBhdGggZD0iTTE4IDhoLTFWNmMwLTIuNzYtMi4yNC01LTUtNVM3IDMuMjQgNyA2djJINmMtMS4xIDAtMiAuOS0yIDJ2MTBjMCAxLjEuOSAyIDIgMmgxMmMxLjEgMCAyLS45IDItMlYxMGMwLTEuMS0uOS0yLTItMnptLTYgOWMtMS4xIDAtMi0uOS0yLTJzLjktMiAyLTIgMiAuOSAyIDItLjkgMi0yIDJ6bTMuMS05SDguOVY2YzAtMS43MSAxLjM5LTMuMSAzLjEtMy4xIDEuNzEgMCAzLjEgMS4zOSAzLjEgMy4xdjJ6Ii8+PC9zdmc+");
            -webkit-mask-repeat: no-repeat;
            -webkit-mask-position: left center;
            -webkit-mask-size: 1em;
            mask-image: url("data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMjQiIHZpZXdCb3g9IjAgMCAyNCAyNCIgd2lkdGg9IjI0Ij48cGF0aCBkPSJNMCAwaDI0djI0SDB6IiBmaWxsPSJub25lIi8+PHBhdGggZD0iTTE4IDhoLTFWNmMwLTIuNzYtMi4yNC01LTUtNVM3IDMuMjQgNyA2djJINmMtMS4xIDAtMiAuOS0yIDJ2MTBjMCAxLjEuOSAyIDIgMmgxMmMxLjEgMCAyLS45IDItMlYxMGMwLTEuMS0uOS0yLTItMnptLTYgOWMtMS4xIDAtMi0uOS0yLTJzLjktMiAyLTIgMiAuOSAyIDItLjkgMi0yIDJ6bTMuMS05SDguOVY2YzAtMS43MSAxLjM5LTMuMSAzLjEtMy4xIDEuNzEgMCAzLjEgMS4zOSAzLjEgMy4xdjJ6Ii8+PC9zdmc+");
            mask-repeat: no-repeat;
            mask-position: left center;
            mask-size: 1em;
        }

        input {
            box-sizing: border-box;
            margin-top: 0.5rem;
            padding: 0.6em;
            width: 100%;
            font-family: "Courier New", Courier, monospace !important;
            border: 2px solid #fff;
            border-radius:0.3rem;
            color: #000;
            background-color: #fff;
            outline: none;
        }

        button {
            margin-top: 0.5rem;
            width: 100%;
            border: 2px solid #fff;
            border-radius:0.3rem;
            color: #fff;
            background-color: #000;
            padding: 0.6em;
            line-height: inherit;
            cursor: pointer;
            font-family: "Courier New", Courier, monospace !important;
            outline: none;
        }

        button:hover {
            background-color: #fff;
            color: #000;
            outline: none;
        }
    </style>
    )rawliteral";
}

#endif /* LUXIO_WIFI_H */