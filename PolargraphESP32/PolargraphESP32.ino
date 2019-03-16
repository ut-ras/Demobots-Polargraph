/*
 * UT R.A.S. Demobots IoT Polargraph
 *
 *
 * TODO
 * Improve front end
 *
 * Scheduler https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/freertos-smp.html
 * Create Task https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/system/freertos.html
 * Run Time stats https://www.freertos.org/a00021.html#vTaskGetRunTimeStats
 * Watchdog Timer https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/system/wdts.html
 */

#include <Arduino.h>
#include <esp_task_wdt.h>

#include "src/iot/webserver.h"
#include "src/iot/wifi_setup.h"
#include "src/polargraph/polargraph.h"


// Macro for printing debug information, quickly turn prints on/ off
#define DEBUG_MODE 0
#if DEBUG_MODE
  #define debugPrint(s) Serial.println(s)
#else
  #define debugPrint(s)
#endif


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //setupWiFiAuto(ssid, pass);
  setupWiFi(WIFI_MODE, ssid, pass);   //Access Point or Station
  setupWebServer();                   //Set up the Web Server
  setupPolargraph();                  //Set up the Polargraph

  //debugPrint("WiFi mode=" + String(WIFI_MODE) + ", ssid = " + String(ssid) + ", pass = " + String(pass));
  //debugPrint("Web server at " + webServerPath);

  xTaskCreatePinnedToCore(&vPolargraphTaskCode,"polargraphTask", 2000, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(&vWebserverTaskCode,"webserverTask", 2000, NULL, 2, NULL, 1);

  //remove the main arduino loop thread
  vTaskDelete(NULL);
}

void loop() {
  //this thread has been stopped in setup()

  debugPrint("loop start");
  delay(1000);
  debugPrint("loop end");
  delay(1000);
}



void vPolargraphTaskCode(void * pvParameters){
  debugPrint("vPolargraphTaskCode start");
  while(true) {
    esp_task_wdt_reset();
    debugPrint("pg start");

    if (loopPolargraph()) {
      delay(500);
    }

    debugPrint("pg end");
  }
}

void vWebserverTaskCode(void * pvParameters){
  debugPrint("vWebserverTaskCode start");
  static long next_update = millis();

  while(true) {
    esp_task_wdt_reset();
    debugPrint("ws start");

    long t_curr = millis();
    if (t_curr > next_update) {
      loopWebServer();
      next_update = t_curr + MIN_UPDATE_MS;
    }else  {
      delay(20);
    }

    debugPrint("ws end");
  }
}


// Single thread old version
void singleThreadLoop() {
  //draw a line between two points if available/ enabled
  //returns true if at a good stopping point to check webserver
  static long next_update = millis();
  if (loopPolargraph()) {   //returns true if the polargraph is at a stopping point
    long t_curr = millis();
    if (t_curr > next_update) {
      loopWebServer();    //check the web server for updates
      next_update = t_curr + MIN_UPDATE_MS;
    }
  }
}
