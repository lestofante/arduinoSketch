#include "InputPin.h"

#define MIN_RADIO_VALUE 900
#define MAX_RADIO_VALUE 1900


/*
pin3 = motor0
pin9 = motor1
pin10 = motor2
pin11 = motor3

^up
    motor0
motor1 + motor2
    motor3
    

*/

//void setMid();

unsigned long time;
unsigned long midChannel[4];
long motorPower[4];

InputPin inputs;

unsigned long lastInputChannelValue[4];

void setup(){
  Serial.begin(57600);
  time = micros();
  
  for (int i=0; i < 4; i++){
      lastInputChannelValue[i]=0;
  }
  setMid();
}

void setMid(){
  //calculate the mid of the RX
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  for (int a=0; a < 100;a++){
    while ( !inputs.getAndSetFalseHasChanged() ) {
      //wait until new data is readed
    }
    if (a==0){
      midChannel[0]=inputs.getDuration(0);
      midChannel[1]=inputs.getDuration(1);
      midChannel[2]=inputs.getDuration(2);
      midChannel[3]=inputs.getDuration(3);
    }else{
      midChannel[0]=( midChannel[0]+inputs.getDuration(0) )/2;
      midChannel[1]=( midChannel[1]+inputs.getDuration(1) )/2;
      midChannel[2]=( midChannel[2]+inputs.getDuration(2) )/2;
      midChannel[3]=( midChannel[3]+inputs.getDuration(3) )/2;
    }
  }
  
  digitalWrite(13, LOW);
}


void loop(){
  
  computeInputs();
  
  calculateLoopDuraton();

}

unsigned long desiredMotorPower=MIN_RADIO_VALUE;

int volte=0;
unsigned long midTime=0;
boolean led = false;
void calculateLoopDuraton(){
  
  if (midTime!=0)
    midTime = (midTime+micros() - time)/2;
  else
    midTime = (micros() - time);
    
  time = micros();
  volte++;
  if (volte>=15000){
    digitalWrite(13, led);
    led = !led;
    Serial.print("time 1 loop in micros:");
    Serial.println(midTime);
//    Serial.println(F_CPU);
    volte = 0;
  
    Serial.println("input:");
    for (int i =0; i < 4; i++){
      Serial.print(i);
      Serial.print(":");
      Serial.println( inputs.getDuration(i) );
    }
    Serial.print("Mid0:");
    Serial.println(midChannel[0] );
    Serial.print("Mid1:");
    Serial.println( midChannel[1] );
    Serial.print("Mid2:");
    Serial.println( midChannel[2] );
    Serial.print("Mid3:");
    Serial.println( midChannel[3] );
    
    Serial.print("DesiredPower:");
    Serial.println( desiredMotorPower );
    
    Serial.print("Mot0:");
    Serial.println( motorPower[0] );
    
    Serial.print("Mot1:");
    Serial.println( motorPower[1] );
    
    Serial.print("Mot2:");
    Serial.println( motorPower[2] );
    
    Serial.print("Mot3:");
    Serial.println( motorPower[3] );
      
  }
  
}


int ROLL_PITCH_YAW_WEIGHT = 4;
int volteErrSignalNotInRange=0;
int volteErrSignalLost=0;
int MAXDIFF=20;
int diff;
byte cont;

void computeInputs(){
  /*
  desiredMotorPower = inputs.getDuration(0);
  diff=false;
  for (int cont=0; cont < 4; cont++){
    motorPower[cont] = map(desiredMotorPower, 0, MAX_RADIO_VALUE, 0, 220);
  }
  analogWrite(  3, motorPower[0] );
  analogWrite(  9, motorPower[1] );
  analogWrite(  10, motorPower[2] );
  analogWrite(  11, motorPower[3] );
  */
  desiredMotorPower = inputs.getDuration(0);
  diff=0;
  
  if ( inputs.getAndSetFalseHasChanged() ){
    for (cont = 0; cont < 4; cont++){
      diff+=inputs.getDuration(cont)-lastInputChannelValue[cont];
    }
    if (abs(diff)>MAXDIFF){  //if there is enought difference
      volteErrSignalLost=0;
      if ( (desiredMotorPower >= MIN_RADIO_VALUE) && (desiredMotorPower <= MAX_RADIO_VALUE) ){ //if engine input is valid
        volteErrSignalNotInRange=0;
        long a = ( (long)inputs.getDuration(1) -(long)midChannel[1] )/ROLL_PITCH_YAW_WEIGHT; //PITCH
//Serial.print("a:");
//Serial.println(a);

        long b = ( (long)inputs.getDuration(2) -(long)midChannel[2] )/ROLL_PITCH_YAW_WEIGHT; //ROLL
//Serial.print("b:");
//Serial.println(b);

        long c = ( (long)inputs.getDuration(3) -(long)midChannel[3] )/ROLL_PITCH_YAW_WEIGHT; //YAW
//Serial.print("c:");
//Serial.println(c);

        motorPower[0] = constrain( map(desiredMotorPower+a+c, 0, MAX_RADIO_VALUE, 0, 220), 0, 250);
        analogWrite(  3, motorPower[0] );

        motorPower[1] = constrain( map(desiredMotorPower-b-c, 0, MAX_RADIO_VALUE, 0, 220), 0, 250);
        analogWrite(  9, motorPower[1] );

        motorPower[2] = constrain( map(desiredMotorPower+b-c, 0, MAX_RADIO_VALUE, 0, 220), 0, 250);
        analogWrite(  10, motorPower[2] );

        motorPower[3] = constrain( map(desiredMotorPower-a+c, 0, MAX_RADIO_VALUE, 0, 220), 0, 250);
        analogWrite(  11, motorPower[3] );

        for (cont = 0; cont < 4; cont++){
          lastInputChannelValue[cont]=inputs.getDuration(cont);
        }
      }else{
        if (volteErrSignalNotInRange>1){
          digitalWrite(13, HIGH);
          Serial.println("Error, radio signal not in range!");
          //slowly turn down engine
          if (motorPower[0]>0)
            motorPower[0]--;
          if (motorPower[1]>0)
            motorPower[1]--;
          if (motorPower[2]>0)
            motorPower[2]--;
          if (motorPower[3]>0)
            motorPower[3]--;
          analogWrite(  3, motorPower[0] );
          analogWrite(  9, motorPower[1] );
          analogWrite(  10, motorPower[2] );
          analogWrite(  11, motorPower[3] );
        }else{
          volteErrSignalNotInRange++;
        }
      }
    }
  }else{
    if (volteErrSignalLost>36000){
      digitalWrite(13, HIGH);
      Serial.println("Error, radio signal lost!");
      //slowly turn down engine
      if (motorPower[0]>0)
        motorPower[0]--;
      if (motorPower[1]>0)
        motorPower[1]--;
      if (motorPower[2]>0)
        motorPower[2]--;
      if (motorPower[3]>0)
        motorPower[3]--;
      analogWrite(  3, motorPower[0] );
      analogWrite(  9, motorPower[1] );
      analogWrite(  10, motorPower[2] );
      analogWrite(  11, motorPower[3] );
    }else{
        volteErrSignalLost++;
    }
  }
 
}
