//=====================================================================================================
// IMU.c
// S.O.H. Madgwick
// 25th September 2010
//
// IMU.cpp
// Modified as class for arduino by Lestofante
// 27th December 2010
//=====================================================================================================
// Description:
//
// Quaternion implementation of the 'DCM filter' [Mayhony et al].
//
// User must define 'halfT' as the (sample period / 2), and the filter gains 'Kp' and 'Ki'.
//
// Global variables 'q0', 'q1', 'q2', 'q3' are the quaternion elements representing the estimated
// orientation.  See my report for an overview of the use of quaternions in this application.
//
// User must call 'IMUupdate()' every sample period and parse calibrated giroYroscope ('giroX', 'giroY', 'giroZ')
// and accelerometer ('accX', 'accY', 'accZ') data.  giroYroscope units are radians/second, accelerometer 
// units are irrelevant as the vector is normalised.
//
//=====================================================================================================

//----------------------------------------------------------------------------------------------------
// Header files

#include "DCM.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------
// Definitions

#define Kp 2.0f			// proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.005f		// integral gain governs rate of convergence of giroYroscope biases
//#define halfT 0.5f		// half the sample period

//---------------------------------------------------------------------------------------------------
// Variable definitions

//float q0 = 1, q1 = 0, q2 = 0, q3 = 0;	// quaternion elements representing the estimated orientation
//float exInt = 0, eyInt = 0, ezInt = 0;	// scaled integral error

//====================================================================================================
// Function
//====================================================================================================

DCM::DCM(){
  exInt=0;
  eyInt=0;
  ezInt=0;
  q0=1;
  q1=0;
  q2=0;
  q3=0;
}

void DCM::DCMupdate(struct data sensor, float halfT) {
  float norm;
  float hx, hy, hz, bx, bz;
  float vx, vy, vz, wx, wy, wz;
  float ex, ey, ez;
  
  float gx=sensor.giroX, gy=sensor.giroY, gz=sensor.giroZ,
        ax=sensor.accX,   ay=sensor.accY,   az=sensor.accZ,
        mx=sensor.magneX, my=sensor.magneY, mz=sensor.magneZ;

  //test if there is a new gyro reading
  if (gx == -1 && gy == -1 && gz == -1)
    return;

  //test if threre is a new (valid) accelerometer or magnetometer reading
  if (az == 0 && ay == 0 && az == 0 && mx == 0 && my == 0 && mz == 0)
    return;
    
  // auxiliary variables to reduce number of repeated operations
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;   
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;          

  // normalise the measurements
  norm = ax*ax + ay*ay + az*az;
  if (norm > 0){
    norm = sqrt(norm);       
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;
    // estimated direction of gravity and flux (v and w)
    vx = 2*(q1q3 - q0q2);
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
  }else{
    vx=0;
    vy=0;
    vz=0;
  }

  norm = mx*mx + my*my + mz*mz;
  if (norm > 0){
    norm = sqrt(norm);          
    mx = mx / norm;
    my = my / norm;
    mz = mz / norm;
    // compute reference direction of flux
    hx = 2*mx*(0.5 - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
    hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5 - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
    hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5 - q1q1 - q2q2);         
    bx = sqrt((hx*hx) + (hy*hy));
    bz = hz;
    // estimated direction of flux 
    wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
    wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
    wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);
  }else{
    wx=0;
    wy=0;
    wz=0;
  }

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) + (my*wz - mz*wy);
  ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

  // integral error scaled integral gain
  exInt = exInt + ex*Ki;
  eyInt = eyInt + ey*Ki;
  ezInt = ezInt + ez*Ki;

  // adjusted gyroscope measurements
  gx = gx + Kp*ex + exInt;
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;

  // integrate quaternion rate and normalise
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

  // normalise quaternion
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;

  //set this data as used
  sensor.giroX = -1;
  sensor.giroY = -1;
  sensor.giroZ = -1;
  
  sensor.accX = 0;
  sensor.accY = 0;
  sensor.accZ = 0;
  
  sensor.magneX = 0;
  sensor.magneY = 0;
  sensor.magneZ = 0;
}

void DCM::fromQuaternionToEuler(float *euler){
  //        q1.normalize  ();
  float test = q0 * q1 + q2 * q3;
  if (test > 0.499) { // singularity at north pole
    euler[1] = (float) (2 * atan2(q0, q3));
    euler[2] = (float) (PI / 2);
    euler[0] = 0;
    return;
  }
  if (test < -0.499) { // singularity at south pole
    euler[1] = (float) (2 * atan2(q0, q3));
    euler[2] = (float) -(PI / 2.0);
    euler[0] = 0;
    return;
  }
  float sqx = q0 * q0;
  float sqy = q1 * q1;
  float sqz = q2 * q2;
  euler[1] = (float) atan2(2 * q1 * q3 - 2 * q0 * q2, 1 - 2 * sqy - 2 * sqz);
  euler[2] = (float) asin(2 * test);
  euler[0] = (float) atan2(2 * q0 * q3 - 2 * q1 * q2, 1 - 2 * sqx - 2 * sqz);
  return;
}


//====================================================================================================
// END OF CODE
//====================================================================================================





