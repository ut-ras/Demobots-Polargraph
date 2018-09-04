#ifndef __POLARGRAPH_H__
#define __POLARGRAPH_H__

/* Measurements (mm)*/
#define X_MAX 515
#define Y_MAX 355
#define PULLEY_RADIUS 43  //update this
#define PULLEY_CIRC (2 * PULLEY_RADIUS * PI)
#define STEPS_PER_ROT 200
//steps = distance * mm_to_steps_pulley
#define mm_to_steps_pulley double(STEPS_PER_ROT) / double(PULLEY_CIRC);



/* Structs */
struct pos {
  int16_t x;
  int16_t y;
  pos& operator=(const pos& a) {
    x = a.x;
    y = a.y;
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
bool loopPolargraph();

bool addToBuffer(pos pos_new);
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
