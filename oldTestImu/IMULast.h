#ifndef IMULast_h
#define IMULast_h

class IMULast{
  public:
    void start();
    void update(unsigned long);
    void leggi(float*);
    void debug();  
  private:
    //void initAcc();
    int midA, midB, midC, yaw, pitch, roll;
};
#endif
 
