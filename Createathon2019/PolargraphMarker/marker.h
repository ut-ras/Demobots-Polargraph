/*
 * marker.h
 */

#ifndef MARKER_H
#define MARKER_H

#define LIFT_POS 100
#define SET_POS 10

void setupMarker(int _servoPin);
void liftMarker();
void setMarker();
bool isMarkerDown();

void setServo(int pos);

#endif
