#ifndef IMU_h
#define IMU_h

class IMU{
  public:
  IMU();
  void start();
  void update(unsigned long deltaTmicros);
  float* leggi();
  void debug();  
  private:
  unsigned long time1;
  unsigned long sumNunTime;
  unsigned long sumGyroTime;
  int updateNumber;
};
#endif
 
