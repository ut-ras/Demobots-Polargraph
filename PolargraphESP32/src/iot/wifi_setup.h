/* IoT Polargraph
 * wifi.h
 *
 * WIFI_MODE variable
 * AP Mode: Broadcasts a WiFi access point
 * STA Mode: Connects to an existing WiFi access point
 *
 * Set WiFi credentials with "ssid" and "pass" strings
 *
 *
 * Resources
 *
 * Soft Access Point
 * https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/soft-access-point-class.html
 *
 */

 #ifndef WIFI_H
 #define WIFI_H

#include <WiFi.h>
#include <ESPmDNS.h>

#include "iot_config.h"


/* Setup Functions */

/* setupWiFi
 * STA = connect to a WiFi network with name ssid
 * AP = create a WiFi access point with  name ssid
 */
void setupWiFi(String mode, const char * _ssid, const char * _pass) {
  if (mode.equals("AP")) {
    //Turn on Access Point
    WiFi.softAP(_ssid, _pass);
    ip = WiFi.softAPIP();
  }
  else {
    //Connect to a WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      yield();
      //Serial.print(".");
    }
    ip = WiFi.localIP();
  }
}

#endif
