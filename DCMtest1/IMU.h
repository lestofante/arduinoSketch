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
#ifndef IMU_h
#define IMU_h

#include <WProgram.h>
//----------------------------------------------------------------------------------------------------
// Variable declaration
class IMU{
  
  public:
    float q0, q1, q2, q3;	// quaternion elements representing the estimated orientation

    //---------------------------------------------------------------------------------------------------
    // Function declaration
    IMU(); //constructor, set exInt, eyInt and ezInt to 0
    void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
  
  private:
    float exInt, eyInt, ezInt;	// scaled integral error
  
};
#endif
//=====================================================================================================
// End of file
//=====================================================================================================
