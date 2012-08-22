 
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
    void print();
    uint8_t outbuf[6];
    float data[6];
    float midValue[3];
    float numberToVoltage[3];
    boolean nunRead, wmpRead, isNun;
    byte ret;
};
#endif
