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
#ifndef MahonyAHRS_h
#define MahonyAHRS_h

#include <Arduino.h>
#include "SensorDataStruct.h"
//----------------------------------------------------------------------------------------------------
// Variable declaration
class MahonyAHRS{
  
  public:
    //---------------------------------------------------------------------------------------------------
    // Function declaration
    void begin(float, float);
    void update(struct data sensor);
    void printQuad();
    void reset();
  private:
    void Update(float gx, float gy, float gz, float ax, float ay, float az, float samplePeriod);
    void Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, float samplePeriod);
    float Quaternion[4], eInt[3];
    unsigned long lastUp;
    unsigned long now;
    struct data *sensorP;
    struct data sensor;
    float Kp, Ki;
    byte floatEqZero(float a);
};
#endif
//=====================================================================================================
// End of file
//=====================================================================================================

