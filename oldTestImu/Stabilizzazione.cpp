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
// User must call 'IMUupdate()' every sample period and parse calibrated gyroscope ('gx', 'gy', 'gz')
// and accelerometer ('ax', 'ay', 'ay') data.  Gyroscope units are radians/second, accelerometer 
// units are irrelevant as the vector is normalised.
//
//=====================================================================================================

//----------------------------------------------------------------------------------------------------
// Header files

#include "Stabilizzazione.h"
//#include <math.h>

//----------------------------------------------------------------------------------------------------
// Definitions

float Kp = 7.0f;		// proportional gain governs rate of convergence to accelerometer/magnetometer
float Ki = 0.005f;		// integral gain governs rate of convergence of gyroscope biases
//#define Ki 0
float halfT = 0.0025f;		// half the sample period

//---------------------------------------------------------------------------------------------------
// Variable definitions

//float q0 = 1, q1 = 0, q2 = 0, q3 = 0;	// quaternion elements representing the estimated orientation
//float exInt = 0, eyInt = 0, ezInt = 0;	// scaled integral error

//====================================================================================================
// Function
//====================================================================================================

Stabilizzazione::Stabilizzazione(){
  reset();

  sumTimePart0 = sumTimePart1 = sumTimePart2 = updateNumber = 0;
}

void Stabilizzazione::start(){
  Serial.println("Starting IMU");
  imu.start();
  acc=gyro=true;
}

void Stabilizzazione::update(unsigned long deltaTmicros) {


  time1 = micros();
  updateNumber++;

  float invNorm;
  float vx, vy, vz;
  float ex, ey, ez;

  imu.update(deltaTmicros);

  sumTimePart0+= micros()-time1;
  time1 = micros();

  float data[6];
  imu.leggi(data);

  sumTimePart1+= micros()-time1;

  time1 = micros();

  float gx=0, gy=0, gz=0, ax=0, ay=0, az=1;

  if (acc){
    ax=-data[4];
    ay=-data[3];
    az=data[5];
  }
  if (gyro){
    gx=data[2];
    gy=-data[1];
    gz=data[0];
  }
  
  if (!acc && !gyro){
    Serial.println("No gyro or acc set");
  }
  //
  //
  if (ax==0 && ay==0 && az==0){
    Serial.println("Accelerometer bad data");
    return;
  }

  // normalise the measurements

  invNorm = invSqrt(ax*ax + ay*ay + az*az);
  ax = ax * invNorm;
  ay = ay * invNorm;
  az = az * invNorm;

  // estimated direction of gravity
  vx = 2*(q1*q3 - q0*q2);
  vy = 2*(q0*q1 + q2*q3);
  vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

  // error is sum of cross product between reference direction of field and direction measured by sensor
  ex = (ay*vz - az*vy);
  ey = (az*vx - ax*vz);
  ez = (ax*vy - ay*vx);


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

  invNorm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = x = q0 * invNorm;
  q1 = y = q1 * invNorm;
  q2 = z = q2 * invNorm;
  q3 = w = q3 * invNorm;
  sumTimePart2+= micros()-time1;

}

void Stabilizzazione::setTime(float t){
  halfT = (t/1000000.0) /2.0;
}


float Stabilizzazione::invSqrt(float number) {
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y  = number;
  i  = * ( long * ) &y;                       // evil floating point bit level hacking
  i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return y;
}


void Stabilizzazione::setKi(float i){
  Ki=i;
}

void Stabilizzazione::setKp(float p){
  Kp=p;
}

float Stabilizzazione::getKi(){
  return Ki;
}

float Stabilizzazione::getKp(){
  return Kp;
}

float gMin=-1, gMax=0;
void Stabilizzazione::debug(){
  Serial.print("orizon PI:");
  Serial.print( getKp() );
  Serial.print(" ");
  Serial.println( getKi() );

  float data[6];
  imu.leggi(data);
  Serial.print("dati IMU:");
  for (int i=0;i<6;i++){
    Serial.print(" ");
    Serial.print(data[i]);
  }

  Serial.println();
  float g=0;
  for (int i=3;i<6;i++){
    g+=data[i]*data[i];
  }
  g=1/invSqrt(g);
  if (gMin==-1){
    gMin=g;
  }

  if (g<gMin){
    gMin=g;
  }
  if (g>gMax){
    gMax=g;
  }
  Serial.print("Gravity:");
  Serial.print(g);
  Serial.print(" MIN:");
  Serial.print(gMin);
  Serial.print(" MAX:");
  Serial.println(gMax);

  Serial.print("STAB Part0:");
  Serial.println(sumTimePart0/updateNumber);

  Serial.print("STAB Part1:");
  Serial.println(sumTimePart1/updateNumber);

  Serial.print("STAB Part2:");
  Serial.println(sumTimePart2/updateNumber);
  sumTimePart0 = sumTimePart1 = sumTimePart2 = updateNumber = 0;
  imu.debug();
}

void Stabilizzazione::basicDebug(){
  float data[6];
  imu.leggi(data);
  Serial.print("dati IMU:");
  for (int i=0;i<6;i++){
    Serial.print(" ");
    Serial.print(data[i]);
  }
  Serial.println();

  float g=0;
  for (int i=3;i<6;i++){
    g+=data[i]*data[i];
  }
  g=1/invSqrt(g);
  if (gMin==-1){
    gMin=g;
  }

  if (g<gMin){
    gMin=g;
  }
  if (g>gMax){
    gMax=g;
  }
  Serial.print("Gravity:");
  Serial.print(g);
  Serial.print(" MIN:");
  Serial.print(gMin);
  Serial.print(" MAX:");
  Serial.println(gMax);
  
  Serial.print("halfT:");
  Serial.println(halfT, 5);
}
//====================================================================================================
// END OF CODE
//====================================================================================================


