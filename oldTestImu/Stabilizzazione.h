//=====================================================================================================
// IMU.h
// S.O.H. Madgwick
// 25th September 2010
//
// Modified as class for arduino by Lestofante
// 27th December 2010
//
//=====================================================================================================
//
// See IMU.c file for description.
// 
//=====================================================================================================
#ifndef Stabilizzazione_h
#define Stabilizzazione_h

#include <WProgram.h>
#include "IMULast.h"
//----------------------------------------------------------------------------------------------------
// Variable declaration
class Stabilizzazione{
  
  public:
    float q0, q1, q2, q3;	// quaternion elements representing the estimated orientation
    float x, y, z, w;
    //---------------------------------------------------------------------------------------------------
    // Function declaration
    Stabilizzazione(); //constructor, set exInt, eyInt and ezInt to 0
    //void IMUupdateVerbose(float gx, float gy, float gz, float ax, float ay, float az);
    void update(unsigned long);
    void start();
    void setTime(float);
    void setKi(float);
    void setKp(float);
    float getKi();
    float getKp();
    void debug();
    void basicDebug();

    void setAcc(bool a){
      acc=a;
    }
    void setGyro(bool a){
      gyro=a;
    }
    
    void reset(){
      exInt=0;
      eyInt=0;
      ezInt=0;
      
      q0=x=1;
      q1=y=0;
      q2=z=0;
      q3=w=0;
    }
  private:
    boolean acc, gyro;
    float invSqrt(float);
    float exInt, eyInt, ezInt;	// scaled integral error
    IMULast imu;
    unsigned long time1;
    unsigned long sumTimePart0;
    unsigned long sumTimePart1;
    unsigned long sumTimePart2;
    int updateNumber;
};
#endif
//=====================================================================================================
// End of file
//=====================================================================================================
