#ifndef IMU_h
#define IMU_h

#include "SensorInput.h"

class IMU:public SensorInput{
  public:
  IMU();
  void start();
  void update();
  float* leggi();
};
#endif
