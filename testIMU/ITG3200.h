#ifndef ITG3200_h
#define ITG3200_h

#include "Sensor.h"
#include <Wire.h>

class ITG3200:public Sensor{
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
    float toRadians;
    int address;
};

#endif
