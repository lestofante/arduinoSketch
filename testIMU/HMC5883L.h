#ifndef HCM5883L_h
#define HCM5883L_h

#include "Sensor.h"
#include <Wire.h>

class HMC5883L:public Sensor{
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
    int g, address;
    float scaleX, scaleY, scaleZ;
};

#endif
