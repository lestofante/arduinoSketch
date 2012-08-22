#ifndef InputPin_h
#define InputPin_h


#include "WProgram.h"

class InputPin{
  
  public:
    InputPin();
    bool getAndSetFalseHasChanged(); // send the initilization handshake
    int getDuration(int);
  private:
};
#endif
