#ifndef IOT_CONFIG_H
#define IOT_CONFIG_H

#include <Arduino.h>

//WiFi Settings
//STA = connect to a WiFi network with name ssid
//AP = create a WiFi access point with name ssid
#define WIFI_MODE "AP"
const char * ssid = "Polargraph";
const char * pass = "abc";

IPAddress ip;

#define MIN_UPDATE_MS 50
//#define CONFIG_TIMEOUT_MS (1000 * 8)

#endif
