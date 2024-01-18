#ifndef _H_GYROSCOPE
#define _H_GYROSCOPE

#include "MPU6050.h"
#include "Wire.h"

class Gyroscope {
public:
  bool init(void);
  bool calibrate(void);
  bool getAngle(float *yaw);

//  int16_t gz;
  unsigned long now, lastTime = 0;
  float dt;      // derivative time
  float agz = 0; // z-axis angle
  long gzo = 0;  //gyroscope offset
};

#endif