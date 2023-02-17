# Demobots Polargraph IOT 
UT Austin's R.A.S. Demobots built an IoT drawing machine, with a few different software versions.</br>
</br>

![Polargraph Image](img/polargraph.gif)

## PolargraphArduinoGIOT
Runs on an ESP8266. Uses Google Cloud IOT device manager for communication, and connects to an AppEngine webserver in polargraph_appengine.

## PolargraphESP32
Runs on an ESP32 in FreeRTOS. Hosts a web server and control interface on the device, and can either connect to an access point or deploy one.

## Hardware
 * [ESP32 Pico Kit](https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-PICO-KIT?qs=MLItCLRbWsyoLrlknFRqcQ%3D%3D)
 * 2x [NEMA 17 2A Stepper Motors](https://www.amazon.com/Stepper-Bipolar-4-lead-Connector-Printer/dp/B00PNEQKC0/ref=sr_1_4?ie=UTF8&qid=1517537888&sr=8-4&keywords=nema+17+stepper+motor&refinements=p_72%3A2661618011)
 * 2x [A4988 Pololu Stepper Drivers](https://www.pololu.com/product/1182)
