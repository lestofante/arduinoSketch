#ifndef PID_h
#define PID_h

class PID{
  public:
  PID();
  PID(int);
  float update(float, float);
  void setP(int);
  private:
  int P;
};
#endif

