#ifndef PID_h
#define PID_h

class PID{
  public:
  PID();
  PID(float, float, float);
  float update(float, float);
  void setP(float);
  float getP();
  void setI(float);
  float getI();
  void setD(float);
  float getD();
  private:
  float P, I, sumI, D, oldError;
};
#endif

