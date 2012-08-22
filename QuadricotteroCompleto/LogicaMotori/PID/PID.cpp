#include "PID.h"
#define MAX_I_ERROR 200

PID::PID(){
  PID(0, 0, 0);
}

PID::PID(float p, float i, float d){
  P=p;
  I=i;
  D=d;
  sumI=0;
  oldError=0;
}

float PID::update(float angle, float setPoint){
  
  float error = (setPoint-angle);
  
  /*INTEGRAL*/
  sumI+=error;
  //limit integral
  if (sumI > MAX_I_ERROR){
    sumI = MAX_I_ERROR;
  }
  if (sumI < -MAX_I_ERROR){
    sumI = -MAX_I_ERROR;
  }
  
  /*DERIVATIVE*/
  float dTemp= (error-oldError);
  oldError = error;
  
  return error*P+ sumI*I+ dTemp*D;
}

void PID::setP(float p){
  P=p;
}

float PID::getP(){
  return P;
}

void PID::setI(float i){
  I=i;
  sumI=0;
}

float PID::getI(){
  return I;
}

void PID::setD(float d){
  D=d;
}

float PID::getD(){
  return D;
}
