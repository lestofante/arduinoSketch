#import "Giroscopio.h"

#define GYRO_VOLTAGE 5.0        //Gyro is running at 5V
#define GYRO_ZERO_VOLTAGE 1.35  //Gyro is zeroed at 1.35V
#define GYRO_SENSIVITY 0.00067 //Our example gyro is 0.67mV/deg/sec
#define rotationThreshold 1.0   //Minimum deg/sec to keep track of - helps with gyro drifting
#define readError GYRO_VOLTAGE/1024.0

void Giroscopio::setPin(int pin){
  gyro=0;
  gyroPin=pin;
}

float Giroscopio::leggi(){
  return gyro;
}

void Giroscopio::update(unsigned long deltaTmicros){
  
  gyro = (analogRead(gyroPin) * GYRO_VOLTAGE) / 1024.0;
  Serial.println(gyro);
  gyro -= GYRO_ZERO_VOLTAGE;

  if (gyro<=readError && gyro>=-readError){
    gyro=0;
  }

  gyro /= GYRO_SENSIVITY;
  
  float gyroRateTime=0;
  if (gyro >= rotationThreshold || gyro <= -rotationThreshold) {
    gyroRateTime = gyro/( 1000000.0/deltaTmicros );
  }
  gyro = gyroRateTime;
}
