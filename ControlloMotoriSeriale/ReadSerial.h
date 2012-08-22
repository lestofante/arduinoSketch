#ifndef Giroscopio_h
#define Giroscopio_h

#include "WProgram.h"

#define midPrecision 500

class ReadSerial{
  public:
  ReadSerial();
  bool readSerial();
  void get(char *, float *);
  private:
  char input1;
  char input2[10];
  int indexInput2;
  boolean inputValue;
  boolean validInput;  
  char finalC;
  float finalF;
};
#endif
