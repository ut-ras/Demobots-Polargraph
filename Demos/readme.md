
# Instructions for Demos
This guide will hopefully be able to cover everything you'd need to know for demos, and will be updated as the project changes. Still, if you encounter any issues, don't hesitate to reach out to me on discord!
 
#####  - Jake (Demobots Head) 
##
 ## Setup and Inspection
In it's current state, Polargraph is a little heavy and very unwieldy and cumbersome to move around, so be careful and ask for help if you need it. Be sure to grab it's wooden laser-cut stand (better one coming in the future) from the RAS Office as well.

After you've propped it up securely, verify that the winches (black wheels attached the motors) don't have any of their tension string tangled around them. Unravel the power supply cord from Polargraph and set aside. 

Unscrew the backplate of the electrical harness to access the breadboard. (It may be off already)

**Verify that there are no loose connections and that** **everything is seated properly in the breadboard(s).** 

**When doing this, refer to the circuit diagrams, breadboard diagram, pinouts, etc.**

### Breadboard Diagram
Note that the MS1 and MS2 pins of each A4988 Stepper Driver are both 5V high, this sets the stepper motors to eighth step, refer to the [A4988 pinout](../Electrical/A4988_pinout.png) as needed.

The exact order of the stepper driver output wires depends on the stepper motor used, refer to the [A4988 pinout](../Electrical/A4988_pinout.png) as needed.
</br>

![connections](../Electrical/breadboard_diagram.png)


*Extra documentation can be found in [Electrical](../Electrical).*

Once you've double checked the connections, you can either rescrew the backplate or leave it open. Go ahead and plug in the power adapter to an AC wall outlet or extension cord.

You'll notice the ESP32's red light come on, along with a quiet but noticable noise coming from the stepper motors, if you don't, something is wrong. Unhook the tension wire from the paper clips and the top of the board, the motors should be locked, holding the gondola (marker holder) in place.

 *(Notice that when the Polargraph loses power, the steppers go limp and drop the gondola, we avoid that by placing the string of each winch hooked on the paper clip its next to.)*

 ## Changing Out Paper
Get a piece/roll of large paper and cut it to your desired size, and either attach it to the board using the paperclips already on there, or by using tape. be sure to account for clearence with the winches when sizing your paper.

After you've attached the paper, either measure or guestimate the exact center of the page and mark it with a dot using the marker. 

Carefully and slowly, facing the board, manually adjust the winches (careful not to get anything tangled) such that the gondola's marker hole rest directly over the dot you marked. This is where we will tell Polargraph to set as the origin ("zero") of it's coordinate system. 

Now your marker in the gondola, and secure it using the breadtie on there. the portion of marker sticking through the hole should be roughly the same length of the diameter of the castors on the gondola

 ## Drawing
 On your device, be sure to have these instructions open/downloaded, as well as any SVG (Vector files) you'll want to upload to Polargraph to draw are saved on your device as well. 

*The current Polargraph code can be finnicky about what SVG's it likes and which it doesn't like, so I've gone ahead and verified/prepared some good demonstration drawings:*

[Click here for Demo SVG files.](SVGs)

On your device's wifi settings, disable any automatic reconnection to the ***utexas*** wifi (or other public access points), this helps to avoid accidental disconnections.

### Connect to the ESP32's access point using:

|SSID| Password |
|--|--|
|Demobot  | demobot1234  |


**When you're connected to Polargraph's access point, you won't have internet access! Be sure to have everything you need before connecting.**

 ### Once connected, in your web browser, go to [192.168.4.1](http://192.168.4.1/)

You should see a webpage that looks like this:

![html](html.png)


# ---WIP---
