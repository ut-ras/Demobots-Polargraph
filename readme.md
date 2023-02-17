# Demobots Polargraph - Spring 2023
UT Austin's R.A.S. Demobots built an IoT drawing machine.

This branch is for updates/upgrades made to Polargraph in Spring of 2023, to eventually be merged into master branch, with older the master (last updated in 2019) archived. </br>
</br>

![Polargraph Image](img/polargraph.gif)

## PolargraphESP32
Runs on an ESP32 in FreeRTOS. Hosts a web server and control interface on the device, and can either connect to an access point or deploy one.

## Hardware
 * [ESP32 Pico Kit](https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-PICO-KIT?qs=MLItCLRbWsyoLrlknFRqcQ%3D%3D)
 * 2x [NEMA 17 2A Stepper Motors](https://www.amazon.com/Stepper-Bipolar-4-lead-Connector-Printer/dp/B00PNEQKC0/ref=sr_1_4?ie=UTF8&qid=1517537888&sr=8-4&keywords=nema+17+stepper+motor&refinements=p_72%3A2661618011)
 * 2x [A4988 Pololu Stepper Drivers](https://www.pololu.com/product/1182)
