#ifndef Motore_h
#define Motore_h

#include "WProgram.h"

class Motore{
  public:
  Motore(int);
  void update(int actualPower);
  private:
  int PWMpin;
};
#endif

