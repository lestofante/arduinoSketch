 
#ifndef WMP_h
#define WMP_h

#include <WProgram.h>
#include "Fastwire.h"

class WMP{
  
  public:
    void init();
    boolean update();
    float* leggi();
    void debug();
  private:
    void send_zero();
    void extrapolateMidWMP();
    uint8_t outbuf[6];
    float data[3];
    float midValue[3];
};
#endif
