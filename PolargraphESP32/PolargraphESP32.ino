/*
 * UT R.A.S. Demobots IoT Polargraph
 *
 *
 * TODO
 * Use FreeRTOS Scheduler
 * Use a fixed size circular array for the coordinate buffer
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

long next_update = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  setupWiFiAuto(ssid, pass);
  //setupWiFi(WIFI_MODE, ssid, pass);   //Access Point or Station
  setupWebServer();                   //Set up the Web Server
  next_update = millis();

  //Serial.println("WiFi mode=" + String(WIFI_MODE) + ", ssid = " + String(ssid) + ", pass = " + String(pass));
  //Serial.println("Web server at " + webServerPath);

  //setupPolargraph();

  xTaskCreatePinnedToCore(&vPolargraphTaskCode,"polargraphTask",2000,(void*)1,4,NULL, 1);
  xTaskCreatePinnedToCore(&vWebserverTaskCode,"webserverTask",2000,(void*)1,5,NULL, 1);


}

void loop() {
  Serial.println("loop start");
  //draw a line between two points if available/ enabled
  //returns true if at a good stopping point to check webserver
  /*if (loopPolargraph()) {
    long t_curr = millis();
    if (t_curr > next_update) {
      loopWebServer();    //check the web server for updates
      next_update = t_curr + MIN_UPDATE_MS;
    }
  }*/

  delay(2000);
  //vTaskSwitchContext();


  //Serial.println("loop end");
}

void vPolargraphTaskCode(void * pvParameters){
  Serial.println("vPolargraphTaskCode start");
  configASSERT( ( (uint32_t) pvParameters) == 1);
  for(;;){
    esp_task_wdt_reset();

      if (loopPolargraph()) {
        delay(500);
        //vTaskSwitchContext();
      }
  }
}

void vWebserverTaskCode(void * pvParameters){
  Serial.println("vWebserverTaskCode start");
  configASSERT( ( (uint32_t) pvParameters) == 1);
  for(;;){
    esp_task_wdt_reset();

    long t_curr = millis();
    if (t_curr > next_update) {
      loopWebServer();
      next_update = t_curr + MIN_UPDATE_MS;
    }else  {
      delay(500);
    }

  }
}
