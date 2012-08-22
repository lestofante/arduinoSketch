#ifndef SensorInput_h
#define SensorInput_h

class SensorInput{
  public:
  virtual void start() = 0;
  virtual void update() = 0;
  virtual float* leggi() = 0;
};

#endif
