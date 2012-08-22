#define gyroXpin 0


float GYRO_VOLTAGE = 5;         //Gyro is running at 5V
float GYRO_ZERO_VOLTAGE = 1.35;   //Gyro is zeroed at 1.35V
float GYRO_SENSIVITY = .00067;  //Our example gyro is 0.67mV/deg/sec

float rotationThreshold = 1;   //Minimum deg/sec to keep track of - helps with gyro drifting

float readError = (GYRO_VOLTAGE/1023)*2;
float currentAngle = 0;          //Keep track of our current angle

unsigned long time;
void setup(){
  Serial.begin(19200);
  time = micros();
}

unsigned long time2;
long loopNumber=0;
void loop(){
  time2=micros();
  loopNumber++;
  int gyroValue = analogRead(gyroXpin);
  
  //This line converts the 0-1023 signal to 0-5V
  float gyroActualVoltage = (gyroValue * GYRO_VOLTAGE) / 1023;

  //This line finds the voltage offset from sitting still
  float gyroActualZero = gyroActualVoltage - GYRO_ZERO_VOLTAGE;
  
  if (gyroActualZero<=readError && gyroActualZero>=-readError){
    gyroActualZero=0;
  }

  //This line divides the voltage we found by the gyro's sensitivity
  float gyroRate = gyroActualZero / GYRO_SENSIVITY;
  
   //Ignore the gyro if our angular velocity does not meet our threshold
  float gyroRateTime=0;
  if (gyroRate >= rotationThreshold || gyroRate <= -rotationThreshold) {
    //This line divides the value by 100 since we are running in a 10ms loop (1000ms/10ms)
    gyroRateTime = gyroRate/( 1000000.0/(time2-time) );
    Serial.println(gyroRate, 10);
    currentAngle += gyroRateTime;
//    Serial.print(time2);
//    Serial.print(":");
//    Serial.println(gyroRateTime, 10);
  }

  //Keep our angle between 0-359 degrees
  if (currentAngle < 0)
    currentAngle += 360;
  else if (currentAngle > 359)
    currentAngle -= 360;
  
  if (time2-time>1000000L){
    //Serial.print("X Y Z:");

    Serial.print( "SOME INFO: " );
//    Serial.println(gyroActualVoltage, 10);   

    Serial.println(gyroActualZero, 10);   
        
//    Serial.println(gyroRate, 10);

//    Serial.println(gyroRateTime, 10);
     
    Serial.println(currentAngle, 10);
    /*  
     Serial.print(" ");
     Serial.print( gyro[1].leggi() );
     Serial.print(" ");
     Serial.print( gyro[2].leggi() );
     */
//    Serial.println();
    Serial.print( "loop number: " );
    Serial.print( loopNumber );
    Serial.println();
    loopNumber=0;
    time=micros();
  }
  
  delay(20);

}

