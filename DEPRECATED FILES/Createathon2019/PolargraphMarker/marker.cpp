/*
 * marker.cpp
 */

//#include <Arduino.h>
#include <Servo.h>
#include "marker.h"

Servo myservo;
int servoPin;

bool markerDown = true;

void setupMarker(int _servoPin) {
  Serial.println("Starting servo");
  servoPin = _servoPin;
  markerDown = true;
  myservo.attach(_servoPin);
  //setMarker();
}

void setServo(int pos) {
  Serial.print("Setting servo: ");
  Serial.print(pos);
  Serial.println();

  myservo.write(pos);
}

void liftMarker() {
  Serial.println("Marker up");
  markerDown = false;
  setServo(LIFT_POS);
}

void setMarker() {
  Serial.println("Marker down");
  markerDown = true;
  setServo(SET_POS);
}

bool isMarkerDown() {
  return markerDown;
}
