/*
 * Createathon 2019 Polargraph
 * Software Based on UT R.A.S. Demobots IoT Polargraph
 *
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


#define STACK_SIZE_POLARGRAPH 2000  //words
#define PRI_POLARGRAPH 2
#define BLOCK_MS_POLARGRAPH 500     //how long to block if polargraph not busy

// Test program for the marker, up and down
#define STACK_SIZE_MARKER 2000      //words
#define PRI_MARKER 2
#define BLOCK_MS_MARKER 3000        //how long to block between marker updates


void setup() {
  Serial.begin(115200);

  setupWiFi(WIFI_MODE, ssid, pass);   //Access Point (AP), Station (STA), or Auto (AUTO)
  setupWebServer();                   //Set up the Web Server
  setupPolargraph();                  //Set up the Polargraph

  //debugPrint("WiFi mode=" + String(WIFI_MODE) + ", ssid = " + String(ssid) + ", pass = " + String(pass));
  //debugPrint("Web server at " + webServerPath);

  //start new threads for the polargraph and the webserver
  //The priority of the arduino main thread (setup/ loop) is 1
  xTaskCreatePinnedToCore(&vPolargraphTaskCode,"polargraphTask", STACK_SIZE_POLARGRAPH, NULL, PRI_POLARGRAPH, NULL, 1);

  // Test program for the marker, up and down
  //xTaskCreatePinnedToCore(&vMarkerTaskCode,"markerTask", STACK_SIZE_MARKER, NULL, PRI_MARKER, NULL, 1);

  //remove the main arduino thread (setup/ loop)
  vTaskDelete(NULL);
}

void loop() {
  //this thread has been stopped in setup()
}



void vPolargraphTaskCode(void * pvParameters){
  debugPrint("vPolargraphTaskCode start");
  while(true) {
    esp_task_wdt_reset();
    debugPrint("pg start");

    if (loopPolargraph()) {
      delay(BLOCK_MS_POLARGRAPH);
    }

    debugPrint("pg end");
  }
}


// Test program for the marker, up and down
void vMarkerTaskCode(void * pvParameters){
  debugPrint("vMarkerTaskCode start");

  bool markerOn = false;

  while(true) {
    esp_task_wdt_reset();
    debugPrint("marker start");

    setMarker(markerOn);
    markerOn = !markerOn;

    delay(BLOCK_MS_MARKER);

    debugPrint("marker end");
  }
}
