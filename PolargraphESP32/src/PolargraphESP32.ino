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
 *
 * delay() wraps around the FreeRTOS vTaskDelay(), which implements blocking
 *    If you delay() a thread, the Scheduler will not give it a timeslice until
 *    its delay is up, so other threads are free to run.
 */

#include <Arduino.h>
#include <esp_task_wdt.h>

#include "../lib/iot/webserver.h"
#include "../lib/iot/wifi_setup.h"
#include "../lib/polargraph/polargraph.h"


// Macro for printing debug information, quickly turn prints on/ off
#define DEBUG_MODE 0
#if DEBUG_MODE
  #define debugPrint(s) Serial.println(s)
#else
  #define debugPrint(s)
#endif


#define STACK_SIZE 2000     //words
#define PRI_POLARGRAPH 2
#define PRI_WEBSERVER 2


void setup() {
  Serial.begin(115200);

  setupWiFi("AP", ssid, pass);   //Access Point (AP), Station (STA), or Auto (AUTO)
  setupWebServer();                   //Set up the Web Server
  setupPolargraph();                  //Set up the Polargraph

  //debugPrint("WiFi mode=" + String(WIFI_MODE) + ", ssid = " + String(ssid) + ", pass = " + String(pass));
  //debugPrint("Web server at " + webServerPath);

  //start new threads for the polargraph and the webserver
  //The priority of the arduino main thread (setup/ loop) is 1
  xTaskCreatePinnedToCore(&vPolargraphTaskCode,"polargraphTask", STACK_SIZE, NULL, PRI_POLARGRAPH, NULL, 1);
  xTaskCreatePinnedToCore(&vWebserverTaskCode,"webserverTask", STACK_SIZE, NULL, PRI_WEBSERVER, NULL, 1);

  //remove the main arduino thread (setup/ loop)
  vTaskDelete(NULL);
}

void loop() {
  //this thread has been stopped in setup()
  /*
  debugPrint("loop start");
  delay(1000);
  debugPrint("loop end");
  delay(1000);
  */
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
