 
#ifndef WMPandNun_h
#define WMPandNun_h

#include <WProgram.h>
#include <Wire.h>

class WMPandNun{
  
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
};
#endif
