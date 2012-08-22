#ifndef Sensor_h
#define Sensor_h

#include "SensorDataStruct.h"
#include <Arduino.h>

class Sensor {
  public:
    virtual void setup(struct data*)=0;
    virtual void calibrate()=0;
    virtual void update()=0;
    virtual boolean isUpdated()=0; 
    virtual void reset()=0; 
    virtual void debug()=0; 
};

#endif
