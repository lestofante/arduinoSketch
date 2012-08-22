#ifndef Giroscopio_h
#define Giroscopio_h

#include "WProgram.h"

#define midPrecision 20

class Giroscopio{
  public:
  float setPin(int);
  float leggi();
  void update();
  private:
  int gyroPin;
  float gyro;
  float sommaMedia;
};
#endif

