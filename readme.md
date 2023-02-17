# Demobots Polargraph IoT

Automated Drawing Robot originally built by the 2018-2019 Demobots comittee.

Demobots is currently working on improving and adding to features to Polargraph (as of 2023).

### Updates:

Currently working on the PolargraphESP32 code, getting it to a more easy to use state. 

More detailed user instructions for demonstrations will follow soon.</br>
 

</br>

## Demo
![Polargraph GIF](img/02-2023polargraph.gif)





## PolargraphESP32
Runs on an ESP32 in FreeRTOS. Hosts a web server and control interface on the device, and can either connect to an access point or deploy one. Allows user to upload SVG drawing for robot to draw. 

## {Deprecated} PolargraphArduinoGIOT 
The deprecated PolargraphArduinoGIOT version is included here, but hasn't been touched since 2019, feel free to look into it. 

Runs on an ESP8266. Uses Google Cloud IOT device manager for communication, and connects to an AppEngine webserver in polargraph_appengine.

## Hardware
 * [ESP32 Pico Kit](https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-PICO-KIT?qs=MLItCLRbWsyoLrlknFRqcQ%3D%3D)
 * 2x [NEMA 17 2A Stepper Motors](https://www.amazon.com/Stepper-Bipolar-4-lead-Connector-Printer/dp/B00PNEQKC0/ref=sr_1_4?ie=UTF8&qid=1517537888&sr=8-4&keywords=nema+17+stepper+motor&refinements=p_72%3A2661618011)
 * 2x [A4988 Pololu Stepper Drivers](https://www.pololu.com/product/1182)

## Upgrades In the Works
 * Whiteboard paneling with dry-erase, to save paper and to allow erasing.
 * Marker/pen actuation, allowing for marker to be moved up/down, to avoid ugly travel lines.


 



## Current Contributors

 - Jake Tomczeszyn - Demobots Head (4jakers18 on the RAS Discord server) 


 ##
  Thanks to Cole Thompson, the RAS Alum who created/maintained the code for this bot back in 2018

