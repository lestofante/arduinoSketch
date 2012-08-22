#include "IMULast.h"
#include "WMP.h"

WMP wmp;

void IMULast::start(){
  analogReference(EXTERNAL);
  wmp.init();
  
  //initAcc();
  midA = 517;
  midB = 510;
  midC = 513;
}

unsigned long time12, sumTimePart0=0, sumTimePart1=0;
int updateNumber=0;
void IMULast::update(unsigned long time){
  time12 = micros();
  updateNumber++;
  
  wmp.update();
  
  sumTimePart0+= micros()-time12;
  time12 = micros();
  
  yaw=analogRead(0)-midA;
  pitch=analogRead(1)-midB;
  roll=analogRead(2)-midC;
  
  sumTimePart1+= micros()-time12;
}

void IMULast::leggi(float* ris){
  
  float *tRis = wmp.leggi();
  for (int i=0;i<3;i++){
    ris[i]=tRis[i];
  }
  
  ris[3]= yaw;
  ris[4]= pitch;
  ris[5]= roll;
  //return ris;
}

void IMULast::debug(){
  
  Serial.print("IMU Part0:");
  Serial.println(sumTimePart0/updateNumber);
  
  Serial.print("IMU Part1:");
  Serial.println(sumTimePart1/updateNumber);
  
  sumTimePart0 = sumTimePart1 = updateNumber=0;
  wmp.debug();
}
/*
void IMULast::initAcc(){
  unsigned long time1 = millis()+1000;
  unsigned long a =0, b=0, c=0;
  int count =0;
  while( time1>millis() ){
    count++;
    a+=analogRead(0);
    b+=analogRead(1);
    c+=analogRead(2);
    //            delayMicroseconds(222);
  }

  Serial.print("Media accelerometro ");
  
  a/=count;
  midA=a-100;
  Serial.print( a );
  Serial.print(" ");

  b/=count;
  midB=b;
  Serial.print( b );
  Serial.print(" ");

  c/=count;  
  midC=c;
  Serial.print( c );
  Serial.print(" ");

  Serial.println();
  Serial.print( count );
  Serial.println();
}
*/
