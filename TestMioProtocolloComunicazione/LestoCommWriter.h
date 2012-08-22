#ifndef LestoCommWriter_h
#define LestoCommWriter_h

#include <WProgram.h>

#define bufferSize 20

class LestoCommWriter{
  public:
    void init(); //use pin 12(output) and 13 (clock)
    
    boolean writeByte(uint8_t);
};
#endif
