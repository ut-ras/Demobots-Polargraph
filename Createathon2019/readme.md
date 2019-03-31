# Createathon Polargraph 2019
Large drawing machine for Createathon 2019. Includes a wireless device to lift and place the marker. Software based on the [UT Robotics and Automation Society Polargraph](https://github.com/ut-ras/Demobots-Polargraph-IOT/tree/master/PolargraphESP32). </br>


## Hardware
  * Polargraph
      * [ESP32 Pico Kit Microcontroller](https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-PICO-KIT?qs=MLItCLRbWsyoLrlknFRqcQ%3D%3D)
      * 2x [NEMA 17 2A Stepper Motors](https://www.amazon.com/Stepper-Bipolar-4-lead-Connector-Printer/dp/B00PNEQKC0/ref=sr_1_4?ie=UTF8&qid=1517537888&sr=8-4&keywords=nema+17+stepper+motor&refinements=p_72%3A2661618011)
      * 2x [A4988 Pololu Stepper Drivers](https://www.pololu.com/product/1182)
      * Pololu 5V 5A Voltage Regulator
      * 12V DC input
  * Wireless Marker
      * ESP8266-01 Microcontroller
      * Servo Motor
      * Pololu 3.3V 2A Voltage Regulator
      * 5V rechargeable USB battery

### ESP32 Pins
Stepper | Step | Direction
--- | --- | ---
**R** | GPIO 12 |GPIO 13
**L** | GPIO 4 | GPIO 5


## Source Files

### PolargraphESP32_Async
  * `PolargraphESP32_Async.ino` main file, starts the FreeRTOS threads
  * `src/iot/` contains modules for WiFi and the HTTP Web Server
      * `src/iot/webserver.h` HTTP Web Server
      * `src/iot/webserver.cpp`
      * `src/iot/templates.h` Web Server front end HTML templates
      * `src/iot/iot_config.h` Quick WiFi settings
      * `src/iot/wifi_setup.h` WiFi connection functions
      * `src/iot/json_parse.h` JSON parse functions
  * `src/polargraph/` contains the polargraph device class, motor control, and the coordinate buffer
      * `src/polargraph/polargraph.h`
      * `src/polargraph/polargraph.cpp`
  * `static/` HTML and JS source files used to generate the string literals in the web server templates

### PolargraphMarker
  * `PolargraphMarker.ino` main file
  * `http.h` HTTP Client for Server-Sent Events
  * `http.cpp`
  * `marker.h` Servo code
  * `marker.cpp`

## Arduino Libraries
ESP32-Arduino, FreeRTOS (add the ESP32 to the Arduino board manager, select ESP32 Pico Kit) </br>
ESPAsyncWebServer (add as a zip)</br>
AsyncTCP (add as a zip)</br>
AccelStepper (Arduino library manager)</br>
