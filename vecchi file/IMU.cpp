#include "IMU.h"
#include "sensor/WMPandNun.h"

WMPandNun imu;

IMU::IMU(){
}

void IMU::start(){
  imu.init();
}

void IMU::update(unsigned long){
  imu.update();
}

float* IMU::leggi(){
  return imu.leggi();
}

void IMU::debug(){
  imu.debug();
}