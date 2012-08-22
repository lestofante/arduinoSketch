#ifndef Giroscopio_h
#define Giroscopio_h

#include "WProgram.h"

class Giroscopio{
  public:
  float setPin(int);
  float leggi();
  void update();
  private:
  int gyroPin;
  float gyro, mid;
};
#endif

