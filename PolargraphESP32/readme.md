# Demobots Polargraph ESP32
Uses arduino-esp32 FreeRTOS core. Hosts a web server which allows nearby users to control the device when in the same local network. Supports SVG file inputs and lists of coordinates. </br>

![Polargraph Image](../img/polargraph.gif)

## Arduino Libraries
ESP32-Arduino, FreeRTOS</br>
AccelStepper</br>

## Source Files
  * `src/iot` contains modules for WiFi and the HTTP Web Server
    * `src/iot/webserver.h` HTTP Web Server
    * `src/iot/webserver.cpp`
    * `src/iot/templates.h` Web Server front end HTML templates
    * `src/iot/iot_config.h` Quick WiFi settings
    * `src/iot/wifi_setup.h` WiFi connection functions
    * `src/iot/json_parse.h` JSON parse functions
  * `src/polargraph` contains the polargraph device class, motor control, and the coordinate buffer
    * `src/polargraph/polargraph.h`
    * `src/polargraph/polargraph.cpp`
  * `static` HTML and JS source files used to generate the string literals in the web server templates

## WiFi Settings
 * WiFi ssid, pass, and mode set in `src/iot/iot_config.h`
 * `WIFI_MODE` Modes:
   * `AP` Access Point: Device Broadcasts an access point, webserver IP default `192.168.4.1`
   * `STA` Station : Device connects to existing access point, webserver IP based on access point settings
   * `AUTO` Auto: Device searches for existing access point, and broadcasts if it can't find one

## Hardware
 * [ESP32 Pico Kit](https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-PICO-KIT?qs=MLItCLRbWsyoLrlknFRqcQ%3D%3D)
 * 2x [NEMA 17 2A Stepper Motors](https://www.amazon.com/Stepper-Bipolar-4-lead-Connector-Printer/dp/B00PNEQKC0/ref=sr_1_4?ie=UTF8&qid=1517537888&sr=8-4&keywords=nema+17+stepper+motor&refinements=p_72%3A2661618011)
 * 2x [A4988 Pololu Stepper Drivers](https://www.pololu.com/product/1182)

### ESP32 Pins
Stepper | Step | Direction
--- | --- | ---
**R** | GPIO 12 |GPIO 13
**L** | GPIO 4 | GPIO 5
