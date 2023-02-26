# Polargraph Source Files
##### ***This readme was made using ChatGPT***

## polargraph.cpp & polargraph.h
The header file defines some constants: a `pos` struct to represent a position on the polargraph, and some functions to interact with the polargraph.

The constants defined are:

- `X_MAX` and `Y_MAX`: the maximum X and Y coordinates of the drawing area in millimeters.
- `STEP_MULT`: a factor to multiply the number of steps taken by the polargraph motors. This is used to increase the resolution of the polargraph drawing.
- `PULLEY_RADIUS`: the radius of the pulleys used by the polargraph in millimeters.
- `PULLEY_CIRC`: the circumference of the pulleys used by the polargraph in millimeters (calculated from `PULLEY_RADIUS`)
- `STEPS_PER_ROT`: the number of steps the polargraph motors take to complete one rotation.

The `pos` struct contains two 16-bit integers, `x` and `y`, to represent the X and Y coordinates of a position on the polargraph. It also defines an `operator=` function to assign one `pos` struct to another, an `operator==` function to compare two `pos` structs for equality, a `toString` function to convert a `pos` struct to a string, and an `toArray` function to convert a `pos` struct to an array of two 16-bit integers.

The public functions defined by polargraph.h and used in polargraph.cpp are:

- `setupPolargraph`:  This function initializes the polargraph by setting the max speed of each stepper motor, creating a queue to buffer incoming positions, and calculating the length of the strings based on the current position of the marker. It also prints some debug information to the serial console.
- `loopPolargraph`:This function is called repeatedly in the main loop of the program. It checks if the polargraph is currently drawing, and if so, it checks if it has reached the next position yet. If it has reached the position, it retrieves the next position from the queue and sets the polargraph to move to that position. If the queue is empty, it stops the drawing.
- `addToBuffer`: This function adds a new position to the end of the queue. It takes a pointer to a `pos` struct as an argument.
- `clearBuffer`: clears the buffer (clears the queue of positions).
- `getBufferSize`: gets the size of the polargraph buffer.
- `getCurrentPos`: gets the current position of the polargraph.
- `resetPos`: sets the current position of the polargraph as (0, 0) and enables drawing. No actual movement is done here, there are no limit switches or feedback.
- `getLeftLength`: This function calculates the length of the left string required to reach a given position pos_new.
- `getRightLength`: This function calculates the length of the right string required to reach a given position pos_new.
- `setPos`: This function sets the position of the marker to a given position pos_new. It first checks if the position is valid (i.e., within the boundaries of the drawing board). If it is valid, it calculates the new length of the left and right strings required to reach the new position. It then calculates the number of steps required for each motor to reach the new string lengths, and sets the motors to move to those positions. Finally, it updates the internal variables for the current position, string lengths, and number of steps.
- `getDistance`: calculates the distance between two `pos` structs.
- `getIsDrawing`: gets the current drawing state of the polargraph.
- `setIsDrawing`: sets the drawing state of the polargraph.

