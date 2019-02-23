/*
 * UT R.A.S. Demobots IoT Polargraph
 *
 *
 * TODO
 * Use FreeRTOS Scheduler
 * Use a fixed size circular array for the coordinate buffer
 * Improve front end
 */

#include <Arduino.h>

#include "src/iot/webserver.h"
#include "src/iot/wifi_setup.h"

#include "src/polargraph/polargraph.h"


long next_update = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  setupWiFi(WIFI_MODE, ssid, pass);   //Access Point or Station
  setupWebServer();                   //Set up the Web Server
  next_update = millis();

  //Serial.println("WiFi mode=" + String(WIFI_MODE) + ", ssid = " + String(ssid) + ", pass = " + String(pass));
  //Serial.println("Web server at " + webServerPath);

  setupPolargraph();

}

void loop() {
  //draw a line between two points if available/ enabled
  //returns true if at a good stopping point to check webserver
  if (loopPolargraph()) {
    long t_curr = millis();
    if (t_curr > next_update) {
      loopWebServer();    //check the web server for updates
      next_update = t_curr + MIN_UPDATE_MS;
    }
  }
}
