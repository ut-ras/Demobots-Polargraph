#ifndef IOT_CONFIG_H
#define IOT_CONFIG_H

#include <Arduino.h>

//WiFi Settings
//STA = connect to a WiFi network with name ssid
//AP = create a WiFi access point with name ssid
//AUTO = STA if it finds the network, AP if it does not
#define WIFI_MODE "AP"
const char * ssid = "CreateathonPolargraph";
const char * pass = "supersecure";

IPAddress ip;

#define STA_TIMEOUT_MS 3000       //for AUTO mode. when initializing WiFi, how long to look for existing AP before deploying one

#endif
