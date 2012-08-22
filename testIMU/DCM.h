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
#ifndef DCM_h
#define DCM_h

#include <Arduino.h>
#include "SensorDataStruct.h"
//----------------------------------------------------------------------------------------------------
// Variable declaration
class DCM{
  
  public:
    float q0, q1, q2, q3;	// quaternion elements representing the estimated orientation

    //---------------------------------------------------------------------------------------------------
    // Function declaration
    DCM(); //constructor, set exInt, eyInt and ezInt to 0
    void DCMupdate(struct data, float);
    void fromQuaternionToEuler(float *);
  private:
    float exInt, eyInt, ezInt;	// scaled integral error0
};
#endif
//=====================================================================================================
// End of file
//=====================================================================================================

