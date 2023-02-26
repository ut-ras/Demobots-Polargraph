# Demobots Polargraph
![Polargraph GIF](Demos/02-2023polargraph.gif)

Automated art robot originally built by the 2018-2019 Demobots committee, Revived and Documented by Jake (Demobots) in Spring of 2023

Polargraph translates digital art to a larger, more tangible medium. 

# Table of contents
  - [Demonstration Instructions](#demonstration-instructions)
  - [Updates](#update---feb-26th-2023)
  - [Installation](#installation-via-arduino-ide)
  - [Software](#software)
  - [Electrical](#electrical)
  - [Current Contributors](#current-contributors)



## Demonstration Instructions
### [For current demonstration instructions, click here!](Demos/readme.md)




## Update - Feb 26th, 2023 

- 2023 revival release can be found [here!](https://github.com/ut-ras/Demobots-Polargraph/releases)
- We now have a whiteboard attached to polargraph for easier demos.
- There are now very comprehensive demo instructions, see above.
- There should now be *way* more documentation throughout as well. 





## Installation Via Arduino IDE

1. Download and install the latest version of the Arduino IDE from the official website. Install the [AccelStepper](https://www.airspayce.com/mikem/arduino/AccelStepper/index.html) library via the Arduino IDE Library Manager.

2. Grab the latest release of Polargraph's code and documentation from [here](https://github.com/ut-ras/Demobots-Polargraph/releases) or clone the repo.

3. Open the Arduino IDE, click on File > Preferences, and in the "Additional Boards Manager URLs" field, add this URL: 

```https://dl.espressif.com/dl/package_esp32_index.json```

4. Go to Tools > Board > Boards Manager, search for "ESP32", and install the "ESP32 by Espressif Systems" board.

5. Connect the ESP32 board to your computer via USB.

6. Go to Tools > Board and select "ESP32 Dev Module" from the dropdown menu.

7. Select the appropriate port by going to Tools > Port and choosing the port that your ESP32 board is connected to.

8. Locate the `PolargraphESP32` folder from the release, and open `PolargraphESP32.ino` in Arduino IDE.

9. Click > Upload, and wait for the code to compile.

10. After it compiles, quickly move to ESP32 Board and hold down the `BOOT` button. Hold it down until you see on the IDE that uploading has begun.

11. Wait for the upload to complete, and then install the ESP32-PICO-KIT into Polargraph, and draw away!





## Software

(Both require the [AccelStepper](https://www.airspayce.com/mikem/arduino/AccelStepper/index.html) library)

### [PolargraphESP32](PolargraphESP32) (In use, not maintained)
Runs on an ESP32 in FreeRTOS. Hosts a web server and control interface on the device, and can either connect to an access point or deploy one. Allows users to upload SVG drawing for robot to draw.

### {DEPRECATED} PolargraphArduinoGIOT 
The deprecated PolargraphArduinoGIOT version is included [here](DEPRECATED_FILES/PolargraphArduinoGIOT), but hasn't been touched since 2019, feel free to look into it. 
Runs on an ESP8266. Uses Google Cloud IOT device manager for communication, and connects to an AppEngine web server in polargraph_appengine.





## Electrical
 * [ESP32-PICO-KIT V4 / V4.1](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-pico-kit.html#get-started-pico-kit-v4-board-front)
 * 2x [NEMA 17 2A Stepper Motors](https://www.amazon.com/Stepper-Bipolar-4-lead-Connector-Printer/dp/B00PNEQKC0/ref=sr_1_4?ie=UTF8&qid=1517537888&sr=8-4&keywords=nema+17+stepper+motor&refinements=p_72%3A2661618011)
 * 2x [A4988 Pololu Stepper Drivers](https://www.pololu.com/product/1182)
 * [5V, 5A Step-Down Voltage Regulator D24V50F5](https://www.pololu.com/product/2851)
 * 12V, 24-36W DC Power Supply
 * 100uF, 50V Capacitor
 * Breadboard with Jumpers

### Circuit Diagram 

Still need to put one here...
*WIP*

### Breadboard Diagram
Note that the MS1 and MS2 pins of each A4988 Stepper Driver are both 5V high, this sets the stepper motors to eighth step, refer to the [A4988 pinout](Electrical/A4988_pinout.png) as needed.

The exact order of the stepper driver output wires depends on the stepper motor used, refer to the [A4988 pinout](Electrical/A4988_pinout.png) as needed.
</br>

![connections](Electrical/breadboard_diagram.png)

*An ESP32-PICO-KIT Fritzing part doesn't exist as of writing, the other ESP32 board was used just to illustrate the pin connections.*





## Current Contributors
 - Jacob Tomczeszyn (Demobots Head) (4jakers18 on the RAS Discord server) 
 


