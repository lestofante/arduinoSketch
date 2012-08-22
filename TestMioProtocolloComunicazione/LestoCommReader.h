#ifndef LestoCommReader_h
#define LestoCommReader_h

#include <WProgram.h>

#define bufferSize 20

class LestoCommReader{
  public:
    void init(); //use pin 12(input) and 13 (clock)
    
    uint8_t readByte();
    int available();
    
  private:
   boolean clockStatus;
};
#endif
