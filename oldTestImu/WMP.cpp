#include "WMP.h"

//modified version from Krulkip's code at: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1248889032/45
unsigned long lastRead=millis();

#define WMPW 0xA6
#define WMPR 0xA4

#define RAW_TO_RADIANS_LOW_SPEED 0.000872665
#define RAW_TO_RADIANS_HIGH_SPEED 0.00396666

void WMP::init(){
  Serial.print("Init1:");
  //send initialization handshake
  
  Fastwire::setup(300, false);
    Serial.print("Init2:");
  Fastwire::write(WMPW, 0xFE, 0x05);
    Serial.print("Init3:");

  delay(500);

  extrapolateMidWMP();

  lastRead=millis();
}

void WMP::extrapolateMidWMP(){
  int sampleNumber = 1000; //about 5 seconds of sample
  
  Serial.print("Calculating mid of ");
  Serial.print(sampleNumber);
  Serial.println(" read");
  
  midValue[0]=midValue[1]=midValue[2]=0;

  byte ret;
  for (int i=0; i < sampleNumber; i++){
    delay (1);
    if (i%100==0){
      Serial.print("Mid working out, actually at ");
      Serial.println(i);
    }
    ret=Fastwire::readBuf(WMPR,0x00,outbuf,6);
    
    if  (ret==0&&(outbuf[5]&0x03)==0x02) {
      //READING WII MOTION PLUS
      midValue[0] += ( ( (outbuf[3]>>2) <<8) + outbuf[0]);
      midValue[1] += ( ( (outbuf[4]>>2) <<8) + outbuf[1]);
      midValue[2] += ( ( (outbuf[5]>>2) <<8) + outbuf[2]);
    }
    else{
      if (ret!=16){ //16 means no data available
        Serial.print("Invalid read, ret value:");
        Serial.println(ret, BIN);
      }else{
        Serial.print("Unknow error, ret value:");
        Serial.println(ret, BIN);
      }
      //invalid read!
      i--;
    }
  }

  midValue[0]/=sampleNumber;
  Serial.print("midValue[0]:");
  Serial.println(midValue[0]);

  midValue[1]/=sampleNumber;
  Serial.print("midValue[1]:");
  Serial.println(midValue[1]);

  midValue[2]/=sampleNumber;
  Serial.print("midValue[2]:");
  Serial.println(midValue[2]);

}

int giro=0, err=0;

unsigned long t1=0, t2=0,t3=0, n1=0, n2=0, n3=0;
unsigned long t;
boolean WMP::update(){
    byte ret;
    t=micros();
    ret=Fastwire::readBuf(WMPR,0x00,outbuf,6);
    n1++;
    t1+=micros()-t;
    
    if  (ret==0&&(outbuf[5]&0x03)==0x02){
      t=micros();
      giro++;
      data[0] = ( (((outbuf[3]>>2)<<8) + outbuf[0]) - midValue[0] ); //YAW
      data[1] = ( (((outbuf[4]>>2)<<8) + outbuf[1]) - midValue[1] ); //ROLL
      data[2] = ( (((outbuf[5]>>2)<<8) + outbuf[2]) - midValue[2] ); //PITCH
      
      //At high speed (slow bit = 0) sensibility is 2.27mV/°/s, at low speed is 0.5mV/°/s. Also tolerance is +-0.05V
      if (abs(data[0]) > 0.05){
        if ( outbuf[3] & B00000010 ){
          //slow speed        
          data[0]*=RAW_TO_RADIANS_LOW_SPEED;
        }
        else{
          data[0]*=RAW_TO_RADIANS_HIGH_SPEED;
        }
      }
      else{
        data[0]=0;
      }

      if (abs(data[1]) > 0.05){
        if ( outbuf[4] & B00000010 ){
          //slow speed        
          data[1]*=RAW_TO_RADIANS_LOW_SPEED;
        }
        else{
          data[1]*=RAW_TO_RADIANS_HIGH_SPEED;
        }
      }
      else{
        data[1]=0;
      }

      if (abs(data[2]) > 0.05){
        if ( outbuf[3] & B00000001 ){
          //slow speed
          data[2]*=RAW_TO_RADIANS_LOW_SPEED;
        }
        else{
          data[2]*=RAW_TO_RADIANS_HIGH_SPEED;
        }
      }
      else{
        data[2]=0;
      }
      n2++;
      t2+=micros()-t;
      t=micros();
      
      n3++;
      t3+=micros()-t;
      return true;
      //print ();
    }
    else{
      err++;
    }
    t=micros();
    
    n3++;
    t3+=micros()-t;
    //lastRead=micros()+500;
  //}
  return false;
}

float* WMP::leggi(){
  return data;
}

void WMP::debug(){
  Serial.print("Dati: ");
  for (int i=0;i<3;i++){
    Serial.print(" ");
    Serial.print(data[i]);
  }
  Serial.println();

  Serial.print("giro:");
  Serial.print(giro);
  Serial.print(" err:");
  Serial.println(err);
  err=0;

//  Serial.print("ava:");
//  Serial.println(Wire.available(), DEC);
  
  Serial.print("WMP time1:");
  Serial.println( t1/n1 );

  Serial.print("WMP time2:");
  Serial.println( t2/n2 );

  Serial.print("WMP time3:");
  Serial.println( t3/n3 );
  t1=t2=t3=n1=n2=n3=0;
}
