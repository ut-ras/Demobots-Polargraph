#include <QueueList.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "polargraph.h"

QueueList <pos> pos_buffer;

AccelStepper stepperR(1, 12, 13);      //rstep = gpio12, rdir = gpio13
AccelStepper stepperL(1, 4, 5);        //lstep = gpio4, ldir = gpio5
MultiStepper steppers;


/* Polargraph status */
pos pos_current = {0, 0};     //current position of our drawing instrument
double baseSpeed = 30.0 * STEP_MULT;
//double accel = 90.0;
boolean isDrawing = false;

double left_length = 0;
double right_length = 0;
long left_steps = 0;
long right_steps = 0;


double getLeftStringLength(pos pos_new);
double getRightStringLength(pos pos_new);

bool checkIfAtPos();
bool setPos(pos pos_new);
bool isValidPos(pos new_pos);


void setupPolargraph() {
  pos_buffer.setPrinter(Serial);

  stepperL.setMaxSpeed(baseSpeed);
  //stepperL.setAcceleration(accel);

  stepperR.setMaxSpeed(baseSpeed);
  //stepperR.setAcceleration(accel);

  //stepperL.moveTo(0);
  //stepperR.moveTo(0);

  steppers.addStepper(stepperL);
  steppers.addStepper(stepperR);

  Serial.println("Starting at l_steps=" + String(stepperL.currentPosition()) + " r_steps=" + String(stepperR.currentPosition()));
  Serial.println("Arrived at pos x=" + String(pos_current.x) + " y=" + String(pos_current.y) + " l_steps=" + String(stepperL.currentPosition()) + " r_steps=" + String(stepperR.currentPosition()));

  left_length = getLeftStringLength(pos_current);
  right_length = getRightStringLength(pos_current);

  pos_buffer.setPrinter(Serial);
}


//returns true if at a good stopping point to check Cloud, like the end of a drawing or not drawing
bool loopPolargraph() {
  //are we drawing
  if (!isDrawing) { return true; }

  //are we there yet
  if (!steppers.run()) {
    Serial.println("Arrived at pos x=" + String(pos_current.x) + " y=" + String(pos_current.y) + " l_steps=" + String(stepperL.currentPosition()) + " r_steps=" + String(stepperR.currentPosition()));

    //is the buffer done
    if (pos_buffer.isEmpty()) {
      isDrawing = false;
      return true;
    }
    else {
      setPos(pos_buffer.pop());
    }
  }
  //Serial.println("Currently at l_steps=" + String(stepperL.currentPosition()) + " r_steps=" + String(stepperR.currentPosition()));
  return false;
}


bool addToBuffer(pos pos_new) {
  //Serial.println("Adding to buffer: x=" + String(pos_new.x) + ", y=" + String(pos_new.y));    //beware of overloading Serial buffer
  pos_buffer.push(pos_new);
  return true;
}

void clearBuffer() {
  Serial.println("Clearing Buffer");
  while(!pos_buffer.isEmpty()) {
    Serial.print(pos_buffer.pop().toString() + " ");
  }
}

void resetPos() {
  setPos({0,0});
  isDrawing = true;
}



/* Polargraph Positioning Functions */

/* Get the length of the string from the motor to the marker
*/
double getLeftStringLength(pos pos_new) {
  return sqrt(pow((X_MAX + pos_new.x), 2) + pow((Y_MAX - pos_new.y), 2)) ;
}

double getRightStringLength(pos pos_new) {
  return sqrt(pow((X_MAX - pos_new.x), 2) + pow((Y_MAX - pos_new.y), 2)) ;
}

/* Set position of drawing instrument, return false if invalid pos
   Position (0, 0) is center of the board
*/
bool setPos(pos pos_new) {
  if (isValidPos(pos_new)) {
    // get desired L R string lengths
    double left_length_new = getLeftStringLength(pos_new);
    double right_length_new = getRightStringLength(pos_new);

    //calculate the new absolute step for each motor, based on the difference between curret and new string lengths
    //mm_to_steps_pulley is (steps per rotation) / (pulley circumference)
    int left_steps_diff = (left_length_new - left_length) * mm_to_steps_pulley;
    int right_steps_diff = -1 * (right_length_new - right_length) * mm_to_steps_pulley;
    left_steps += left_steps_diff;
    right_steps += right_steps_diff;

    Serial.println("Setting to Pos: x=" + String(pos_new.x) + " y=" + String(pos_new.y));
    //Serial.println("Diff Lengths: L=" + String(left_length_new - left_length) + " R=" + String(right_length_new - right_length));
    //Serial.println("Old Lengths: L=" + String(left_length) + " R=" + String(right_length));
    Serial.println("New Lengths: L=" + String(left_length_new) + " R=" + String(right_length_new));
    Serial.println("New Steps: L=" + String(left_steps) + " R=" + String(right_steps) + "\n");

    left_length = left_length_new;
    right_length = right_length_new;

    //set the length values to the new length values if the motors actually moved
    if ((left_steps_diff != 0) || (right_steps_diff != 0)) {  pos_current = pos_new;   }

    //give the new steps to the MultiStepper, this starts the movement, each motor is incremented a small amount each loop with run()
    long steps[] = {left_steps, right_steps};
    steppers.moveTo(steps);

    return true;
  }
  else {
    return false;
  }
}



/* Coordinate Functions for pos struct */

//check if an x coordinate will fit in our system
bool isValidX(int x) {
  return (x > X_MAX * -1) && (x < X_MAX);
}

//check if a y coordinate will fit in our system
bool isValidY(int y) {
  return (y > Y_MAX * -1) && (y < Y_MAX);
}

bool isValidPos(pos new_pos) {
  return isValidY(new_pos.y) && isValidX(new_pos.x);
}

//distance between coordinates p1 and p2
double getDistance(pos p1, pos p2) {
  return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}




/* Getters and Setters */

int getBufferSize() { return pos_buffer.count(); }

pos getCurrentPos() { return pos_current; }

double getLeftLength() { return left_length; }

double getRightLength() { return right_length; }

boolean getIsDrawing() { return isDrawing; }

void setIsDrawing(boolean drawing) { isDrawing = drawing; }
