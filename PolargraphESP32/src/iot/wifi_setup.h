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


// Set your Static IP address
IPAddress local_IP(192, 168, 43, 200);
// Set your Gateway IP address
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 0, 0);


/* Setup Functions */

/* setupWiFi
 * STA = connect to a WiFi network with name ssid
 * AP = create a WiFi access point with  name ssid
 * AUTO = STA if it finds the network, AP if it does not
 */
void setupWiFi(String mode, const char * _ssid, const char * _pass) {
  if (mode.equals("AP")) {
    //Turn on Access Point
    WiFi.softAP(_ssid, _pass);
    ip = WiFi.softAPIP();
  }
  else if (mode.equals("STA")) {
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
  else {
    setupWiFiAuto(_ssid, _pass);
  }
}


/* setupWiFiAuto
 * STA if it finds the network, AP if it does not
 */
void setupWiFiAuto(const char * _ssid, const char * _pass) {
  String mode = "STA";

  //Connect to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _pass);


  long sta_timeout_end = millis() + STA_TIMEOUT_MS;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    yield();

    if (millis() > sta_timeout_end) {
      mode = "AP";
      break;
    }
    //Serial.print(".");
  }

  if (mode.equals("AP")) {
    //Turn on Access Point
    WiFi.softAP(_ssid, _pass);
    ip = WiFi.softAPIP();
  }

  WiFi.setHostname("demobot_polargraph");
  ip = WiFi.localIP();

  Serial.println("WiFi mode=" + mode + ", ip=" + String(WiFi.localIP()) + ", ssid = " + String(ssid) + ", pass = " + String(pass));
}



/* setupWiFiAutoScan
 * STA if it finds the network, AP if it does not
 * The scan is slow, use setupWiFiAuto
 */
void setupWiFiAutoScan(const char * _ssid, const char * _pass) {
  String mode = "AP";


  int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    }
    else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");

            if (WiFi.SSID(i).equals(_ssid)) {
              mode = "STA";
              break;
            }
            delay(10);
        }
    }


  if (mode.equals("AP")) {
    //Turn on Access Point
    WiFi.softAP(_ssid, _pass);
    ip = WiFi.softAPIP();
  }
  else {
    /*if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("STA Failed to configure static IP");
    }*/

    //Connect to a WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      yield();
      //Serial.print(".");
    }

    WiFi.setHostname("demobot");

    ip = WiFi.localIP();
  }

  Serial.println("WiFi mode=" + mode + ", ip=" + String(WiFi.localIP()) + ", ssid = " + String(ssid) + ", pass = " + String(pass));
}

#endif
