#ifndef InputPin_h
#define InputPin_h


#include "WProgram.h"

class InputPin{
  
  public:
    InputPin();
    bool getAndSetFalseHasChanged(); // send the initilization handshake
    int getDuration(int); //return the raw duration of high signal, input: channel
    float getAngle(int); //return the duration mapped from -PI to PI, input: channel
    int getDuration(int, int, int); //return the duration mapped in the range, input: channel, min value, max value
  private:
};
#endif
