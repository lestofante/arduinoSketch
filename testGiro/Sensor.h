#ifndef Sensor_h
#define Sensor_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Sensor
{
public:
  virtual void init();
  virtual void update();
  virtual boolean isDataUpdated();
  virtual void reset();
  virtual void debug();
};

#endif 

