#ifndef __POLARGRAPH_H__
#define __POLARGRAPH_H__

//#define RAS_POLARGRAPH
#define CREATEATHON_POLARGRAPH

/* Measurements (mm)*/

#ifdef RAS_POLARGRAPH
#define X_MAX 515
#define Y_MAX 355
#define STEP_MULT 8
#define PULLEY_RADIUS 43
#define PULLEY_CIRC (2 * PULLEY_RADIUS * PI)
#define STEPS_PER_ROT 200 * STEP_MULT
#define MARKER_WAIT_MS 0
#define MOVE_SPEED 40.0
#endif

#ifdef CREATEATHON_POLARGRAPH
#define X_MAX 380
#define Y_MAX 500
#define STEP_MULT 2
#define PULLEY_RADIUS 7.5
#define PULLEY_CIRC (2 * PULLEY_RADIUS * PI)
#define STEPS_PER_ROT 200 * STEP_MULT
#define MARKER_WAIT_MS 3000
#define MOVE_SPEED 90.0
#endif




//steps = distance * mm_to_steps_pulley
#define mm_to_steps_pulley double(STEPS_PER_ROT) / double(PULLEY_CIRC);


enum marker_enum {STAY, SET, LIFT};
typedef enum marker_enum marker_enum_t;

/* Structs */
struct pos {
  int16_t x;
  int16_t y;
  marker_enum_t marker;           //Marker events that are not STAY (so SET or LIFT) will ignore (x,y) and just move marker
  pos& operator=(const pos& a) {
    x = a.x;
    y = a.y;
    marker = a.marker;
    return *this;
  }
  bool operator==(const pos& a) const {
    return (x == a.x && y == a.y);
  }
  String toString() {
    return String("(" + String(x) + "," + String(y) + ")");
  }
  int16_t * toArray() {
    int16_t arr[2] = {x, y};
    return arr;
  }
};



/* Public Polargraph Functions */
void setupPolargraph();
bool loopPolargraph();      //returns true if at a good stopping point to check IoT, like the end of a drawing or not drawing

bool addToBuffer(pos * pos_new);
void clearBuffer();
int getBufferSize();

pos getCurrentPos();
void resetPos();

double getLeftLength();
double getRightLength();
double getDistance(pos p1, pos p2);

boolean getIsDrawing();
void setIsDrawing(boolean drawing);

#endif //__POLARGRAPH_H__
