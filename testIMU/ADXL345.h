#ifndef ADXL345_h
#define ADXL345_h

#include "Sensor.h"
#include <Wire.h>

class ADXL345:public Sensor{
  public:
    void setup(struct data*);
    void update();
    void calibrate();
    boolean isUpdated(); 
    void reset(); 
    void debug();
  private:
    struct data *store;
    boolean isUp;
    int x, y, z;
    long midX, midY, midZ;
    int g;
    int address;
};

#endif
