#ifndef Giroscopio_h
#define Giroscopio_h

#include "WProgram.h"

class Giroscopio{
  public:
  void setPin(int);
  float leggi();
  void update(unsigned long);
  private:
  int gyroPin;
  float gyro;
};
#endif

