#include <Wire.h>
#include <Servo.h>
#include "NunchukFarlocco.h"
#include "IMU.h"
#include "Motore.h"
#include "Giroscopio.h"
#include "InputPin.h"
#include "PID.h"

#define ledPin 13

#define gyroXpin 1
#define gyroYpin 2
#define gyroZpin 3

#define motore1PWMPin 3
#define motore2PWMPin 9
#define motore3PWMPin 10
#define motore4PWMPin 11

//If max PWM value is 255, a -100<P<100
//#define PofPID 1

#define MIN_RADIO_VALUE 1100
#define MAX_RADIO_VALUE 1950
#define maxReceiverAngle 0.5f
//maxReceiverAngle is in radiant

#define ROLL_STABLE_ANGLE 1.57
#define PITCH_STABLE_ANGLE 0
#define YAW_STABLE_ANGLE 0

Giroscopio gyro[3];
NunchukFarlocco nun;
IMU orizzonte;
PID pid[4]={PID(), PID(), PID(), PID()};
//Motore motori[] = { Motore(motore1PWMPin), Motore(motore2PWMPin), Motore(motore3PWMPin), Motore(motore4PWMPin) };
Servo motori[4]={Servo(), Servo(), Servo(), Servo()};

//channel 0=power, 1=pitch, 2=roll, 3=yaw from 900 to 1900
InputPin inputs;

unsigned long time;
boolean statoLedPin=false;

void setup(){
  time = micros();
  Serial.begin(19200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  int x = analogRead(0);
  x = map(x, 0, 1023, 1, 100);
  Serial.print("p for PID:");
  Serial.println(x);
  int i;
  for (i=0; i<4; i++){
    pid[i].setP(x);
  }
  
  motori[0].attach( motore1PWMPin, 600, 2400 );
  motori[1].attach( motore2PWMPin, 600, 2400 );
  motori[2].attach( motore3PWMPin, 600, 2400 );
  motori[3].attach( motore4PWMPin, 600, 2400 );

  //setting gyro pin, automagigally they calibration
  gyro[0].setPin(gyroXpin);
  gyro[1].setPin(gyroYpin);
  gyro[2].setPin(gyroZpin);
  
  digitalWrite(ledPin, LOW);
  
  Serial.println("begin nunchuck");
  nun.nunchuck_init();
  Serial.println("end nunchuck");
}

int loopCount=0;
unsigned long timeMid=0;
unsigned long time2;
float lastYaw=0, minYaw=1000, maxYaw=-1000;
unsigned long nunchukNotReady=0;
void loop(){
  //Serial.print("loop");
  
  loopCount++;
  time2=micros();
  if (timeMid==0){
    timeMid=time2-time;
  }else{
    timeMid= (timeMid+(time2-time) )/2;
  }
  time=time2;
  
  gyro[0].update();
  gyro[1].update();
  gyro[2].update();
  
  if (loopCount>200){
    Serial.print("X acc, gyro:");
    Serial.print( nun.x );
    Serial.print(" ");
    Serial.println( gyro[0].leggi() );

    Serial.print("Y acc, gyro:");
    Serial.print( nun.y );
    Serial.print(" ");
    Serial.println( gyro[1].leggi() );
    
    Serial.print("Z acc, gyro:");
    Serial.print( nun.z );
    Serial.print(" ");
    Serial.println( gyro[2].leggi() );
  }

  if ( nun.nunchuck_get_data() ){
    orizzonte.IMUupdate(-gyro[1].leggi(), gyro[2].leggi(), -gyro[0].leggi(), nun.x, nun.z, nun.y); //update time: 1 millisecond
    
    //orizzonte.IMUupdate(0, 0, 0, nun.x, -nun.y, nun.z); //FOR DEBUG, TEST
  }else{
    //Serial.println("Nunckuck non pronto!!");
    nunchukNotReady++;
  }

  float roll=atan2(2*(orizzonte.q0*orizzonte.q1+orizzonte.q2*orizzonte.q3), 1-2*(orizzonte.q1*orizzonte.q1+orizzonte.q2*orizzonte.q2));
  float pitch=asin( 2*(orizzonte.q0*orizzonte.q2-orizzonte.q1*orizzonte.q3) );
  float yaw=atan2(2*(orizzonte.q0*orizzonte.q3+orizzonte.q1*orizzonte.q2), 1-2*(orizzonte.q2*orizzonte.q2+orizzonte.q3*orizzonte.q3));
  
  //find out actual estimated yaw angular rotation. We can also use directly gyro value with a proportion :-D
  float yawR = yaw-lastYaw;
  lastYaw=yaw;
  
  if (yawR<minYaw)
    minYaw = yawR;
  
  if (yawR>maxYaw)
    maxYaw = yawR;
  /* because a quadricopter, engine setup:
      0
    2   3
      1
  */
  float engineAngle[4];
  engineAngle[0]=roll+yawR;
  engineAngle[1]=-roll+yawR;
  engineAngle[2]=-pitch-yawR;
  engineAngle[3]=pitch-yawR;
  
  if (loopCount>200){
    Serial.print("Yaw lastYaw yawMin yawMax:");
    Serial.print(yaw);
    Serial.print(" ");
    Serial.print(lastYaw);
    Serial.print(" ");
    Serial.print(minYaw, 7);
    Serial.print(" ");
    Serial.println(maxYaw, 7);   
    Serial.print("Actual angle:");
    Serial.print(roll);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.print(yawR);
    Serial.println();
    Serial.print("Engine correction:");
    Serial.print(engineAngle[0]);
    Serial.print(" ");
    Serial.print(engineAngle[1]);
    Serial.print(" ");
    Serial.print(engineAngle[2]);
    Serial.print(" ");
    Serial.print(engineAngle[3]);
    Serial.println();
  }
  
  int i;
  
  /*
  //USER INPUT
  */
  long enginesPower=0;
  float desiredAngle[4];
  /*
  // FAKE INPUT
  //
  for (i=0; i < 4; i++){
    desiredAngle[0]= ROLL_STABLE_ANGLE+YAW_STABLE_ANGLE;
    desiredAngle[1]= -ROLL_STABLE_ANGLE+YAW_STABLE_ANGLE;
    desiredAngle[2]= -PITCH_STABLE_ANGLE-YAW_STABLE_ANGLE;
    desiredAngle[3]= PITCH_STABLE_ANGLE-YAW_STABLE_ANGLE;
  }
  //
  // END FAKE INPUT
  //
  */
  
  //
  // REAL INPUT
  //
  enginesPower=map( inputs.getDuration(0), MIN_RADIO_VALUE, MAX_RADIO_VALUE, 0, 180 );
  
  //angle in radiant!!
  pitch = mapFloat( inputs.getDuration(1), MIN_RADIO_VALUE, MAX_RADIO_VALUE, -maxReceiverAngle, maxReceiverAngle )+PITCH_STABLE_ANGLE;
  roll = -mapFloat( inputs.getDuration(2), MIN_RADIO_VALUE, MAX_RADIO_VALUE, -maxReceiverAngle, maxReceiverAngle )+ROLL_STABLE_ANGLE;
  yaw = mapFloat( inputs.getDuration(3), MIN_RADIO_VALUE, MAX_RADIO_VALUE, -maxReceiverAngle, maxReceiverAngle )+YAW_STABLE_ANGLE;
  
  if (loopCount>200){
    Serial.print("Input:");
    Serial.print(inputs.getDuration(0));
    Serial.print(" ");
    Serial.print(inputs.getDuration(1));
    Serial.print(" ");
    Serial.print(inputs.getDuration(2));
    Serial.print(" ");
    Serial.print(inputs.getDuration(3));
    Serial.println();
    
    Serial.print("Input angle:");
    Serial.print(roll);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.print(yaw);
    Serial.println();
  }
  
  desiredAngle[0]=roll+yaw;
  desiredAngle[1]=-roll+yaw;
  desiredAngle[2]=-pitch-yaw;
  desiredAngle[3]=pitch-yaw;
  if (loopCount>200){
    Serial.print("Engine input:");
    Serial.print(desiredAngle[0]);
    Serial.print(" ");
    Serial.print(desiredAngle[1]);
    Serial.print(" ");
    Serial.print(desiredAngle[2]);
    Serial.print(" ");
    Serial.print(desiredAngle[3]);
    Serial.println();
  }
  //
  // END REAL INPUT
  //
  
  
  /*
  //END USER INPUT
  */
  
  float enginePower[4];  
  int maxP=0;
  for (i=0; i < 4; i++){
    enginePower[i]=pid[i].update(engineAngle[i], desiredAngle[i])+enginesPower;
    if (enginePower[i]>maxP)
      maxP=enginePower[i];
    if (enginePower[i]<0)
      enginePower[i]=0;
  }

  if (loopCount>200){
    Serial.print("Engine power:");
    Serial.print(enginesPower);
    Serial.print(" Engine power after PID:");
    Serial.print(enginePower[0]-enginesPower);
    Serial.print(" ");
    Serial.print(enginePower[1]-enginesPower);
    Serial.print(" ");
    Serial.print(enginePower[2]-enginesPower);
    Serial.print(" ");
    Serial.print(enginePower[3]-enginesPower);
    Serial.println();
  }
  
  if (maxP>180){//if motor power requested is bigger than maximus (255 for PWM)
    for (i=0; i < 4; i++){
      enginePower[i]=mapFloat(enginePower[i], 0, maxP, 0, 180);//scale engine power
    }
  }

  if (loopCount>200){
    Serial.print("Motori:");
    Serial.print(enginePower[0]);
    Serial.print(" ");
    Serial.print(enginePower[1]);
    Serial.print(" ");
    Serial.print(enginePower[2]);
    Serial.print(" ");
    Serial.println(enginePower[3]);
  }

  for (i=0; i < 4; i++){
    motori[i].write( enginePower[i] );
  }
  
  if (loopCount>200){

    statoLedPin=!statoLedPin;
    if (statoLedPin)
      digitalWrite(ledPin, HIGH);
    else
      digitalWrite(ledPin, LOW);
    
    Serial.print("TIME:");
    Serial.println(timeMid);

    Serial.print("nunchukNotReady:");
    Serial.println(nunchukNotReady);
    
    Serial.print(orizzonte.q0);
    Serial.print(" ");
    Serial.print(orizzonte.q1);
    Serial.print(" ");
    Serial.print(orizzonte.q2);
    Serial.print(" ");
    Serial.print(orizzonte.q3);
    Serial.println();
    Serial.println();
    
    loopCount=0;
  }

}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

