#include "WMP.h"

//modified version from Krulkip's code at: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1248889032/45
unsigned long lastRead=millis();

#define WMPW 0xA6
#define WMPR 0xA4

void WMP::init(){
  Serial.print("Init1:");
  //send initialization handshake
  isNun=false;
  
  Fastwire::setup(300, false);
    Serial.print("Init2:");
  Fastwire::write(WMPW, 0xFE, 0x05);
    Serial.print("Init3:");
//  Wire.beginTransmission(0x53); //WM+ starts out deactivated at address 0x53
//  Wire.send(0xfe); //send 0x04 to address 0xFE to activate WM+
//  Wire.send(0x04);
//  Wire.endTransmission(); //WM+ jumps to address 0x52 and is now active

  wmpRead = nunRead = false;

  delay(500);

  extrapolateMidWMP();
  Serial.print("Init4:");
  //send_zero (); // send the request for next bytes
  //Wire.requestFrom (0x52,6);
  lastRead=millis();
}

void WMP::extrapolateMidWMP(){
  int sampleNumber = 1000; //about 5 seconds of sample
  
  Serial.print("Mid:");
  
  midValue[0]=midValue[1]=midValue[2]=0;
  //byte ret;
  for (int i=0; i < sampleNumber; i++){
    delay(1);
    /*
    if (i%100==0){
      Serial.println(".");
    }
    */
    ret=Fastwire::readBuf(WMPR,0x00,outbuf,6);
      //Serial.print("Mid2:");
      //Serial.print(ret, DEC);
//          Serial.print("Mid:");
    //send_zero (); // send the request for next bytes
    //delay (3);
    //Wire.requestFrom (0x52,6);
    /*if (Wire.available() < 6){
      Serial.println("SLOW!");
      return;
    }
    outbuf[0] = Wire.receive();
    outbuf[1] = Wire.receive();
    outbuf[2] = Wire.receive();
    outbuf[3] = Wire.receive();
    outbuf[4] = Wire.receive();
    outbuf[5] = Wire.receive();
    */
    
    if  (ret==0&&(outbuf[5]&0x03)==0x02) {
      //READING WII MOTION PLUS
      midValue[0] += ( ( (outbuf[3]>>2) <<8) + outbuf[0]);
      midValue[1] += ( ( (outbuf[4]>>2) <<8) + outbuf[1]);
      midValue[2] += ( ( (outbuf[5]>>2) <<8) + outbuf[2]);
      Serial.print(".");
    }
    else{
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

  numberToVoltage[0] = 1.35 / midValue[0];
  Serial.print("numberToVoltage[0]:");
  Serial.println(numberToVoltage[0], 10);

  numberToVoltage[1] = 1.35 / midValue[1];
  Serial.print("numberToVoltage[1]:");
  Serial.println(numberToVoltage[1], 10);

  numberToVoltage[2] = 1.35 / midValue[2];
  Serial.print("numberToVoltage[2]:");
  Serial.println(numberToVoltage[2], 10);

}

int giro=0, err=0;

unsigned long t1=0, t2=0,t3=0, n1=0, n2=0, n3=0;
unsigned long t;
boolean WMP::update(){
  delay(1);
/*
  if ( Wire.available()>=6){
    t=micros();
    outbuf[0] = Wire.receive();
    outbuf[1] = Wire.receive();
    outbuf[2] = Wire.receive();
    outbuf[3] = Wire.receive();
    outbuf[4] = Wire.receive();
    outbuf[5] = Wire.receive();
    */
    t=micros();
    ret=Fastwire::readBuf(WMPR,0x00,outbuf,6);
    n1++;
    t1+=micros()-t;
    
    if  (ret==0&&(outbuf[5]&0x03)==0x02){
      t=micros();
      giro++;
      data[0] = ( (((outbuf[3]>>2)<<8) + outbuf[0]) - midValue[0] );//*numberToVoltage[0]; //YAW
      data[1] = ( (((outbuf[4]>>2)<<8) + outbuf[1]) - midValue[1] );//*numberToVoltage[1]; //ROLL
      data[2] = ( (((outbuf[5]>>2)<<8) + outbuf[2]) - midValue[2] );//*numberToVoltage[2]; //PITCH
/*      
      //At high speed (slow bit = 0) sensibility is 2.27mV/°/s, at low speed is 0.5mV/°/s. Also tolerance is +-0.05V
      if (abs(data[0]) > 0.05){
        if ( outbuf[3] & B00000010 ){
          //slow speed        
          data[0]*=500;
        }
        else{
          data[0]*=2270;
        }
        data[0]*=0.0174532925;
      }
      else{
        data[0]=0;
      }

      if (abs(data[1]) > 0.05){
        if ( outbuf[4] & B00000010 ){
          //slow speed        
          data[1]*=500;
        }
        else{
          data[1]*=2270;
        }
        data[1]*=0.0174532925;
      }
      else{
        data[1]=0;
      }

      if (abs(data[2]) > 0.05){
        if ( outbuf[3] & B00000001 ){
          //slow speed
          data[2]*=500;
        }
        else{
          data[2]*=2270;
        }
        data[2]*=0.0174532925;
      }
      else{
        data[2]=0;
      }
      n2++;
      t2+=micros()-t;
      t=micros();
*/      
      //send_zero (); // send the request for next bytes
      //Wire.requestFrom (0x52,6);
      n3++;
      t3+=micros()-t;
      return true;
      //print ();
    }
    else{
      err++;
    }
    t=micros();
    
    //send_zero (); // send the request for next bytes
    //Wire.requestFrom (0x52,6);
    n3++;
    t3+=micros()-t;
    //lastRead=micros()+500;
  //}
  return false;
}

float* WMP::leggi(){
  return data;
}
/*
void WMP::send_zero (){
  Wire.beginTransmission(0x52);
  Wire.send(0x00);
  Wire.endTransmission();
}
*/
void WMP::debug(){
  Serial.print("Dati: ");
  for (int i=0;i<6;i++){
    Serial.print(" ");
    Serial.print(data[i]);
  }
  Serial.println();

  Serial.print("giro:");
  Serial.print(giro);
  Serial.print(" err:");
  Serial.println(err);
  err=0;

  Serial.print("There is nun:");
  Serial.println(isNun?"true":"false");  
  wmpRead = nunRead = false;

//  Serial.print("ava:");
//  Serial.println(Wire.available(), DEC);
  
  Serial.print("time1:");
  Serial.println( t1/n1 );

  Serial.print("time2:");
  Serial.println( t2/n2 );

  Serial.print("time3:");
  Serial.println( t3/n3 );
  t1=t2=t3=n1=n2=n3=0;
}

void WMP::print () {
  // check if nunchuk is really connected

  if ((outbuf[4]&0x01)==0x01) {
    Serial.print("Ext con: ");
  }
  else{
    Serial.print("NO Ext con: ");
  }
  if ((outbuf[5]&0x03)==0x00) {
    int joy_x_axis = outbuf[0];
    int joy_y_axis = outbuf[1];
    int accel_x_axis = (outbuf[2] << 2) + ((outbuf[5] >> 3) & 2);
    int accel_y_axis = (outbuf[3] << 2) + ((outbuf[5] >> 4) & 2);
    int accel_z_axis = (outbuf[4] << 2) + ((outbuf[5] >> 5) & 6);
    int z_button = (outbuf[5]>>2) & 1;
    int c_button = (outbuf[5]>>3) & 1;
    //Serial.print ("joyx= ");
    //Serial.print (joy_x_axis);
    //Serial.print ("  joyy=");
    //Serial.print (joy_y_axis);
    Serial.print ("  accx= ");
    Serial.print (accel_x_axis);
    Serial.print ("  accy= ");
    Serial.print (accel_y_axis);
    Serial.print (" accz= ");
    Serial.print (accel_z_axis);
    Serial.print ("  ");
    if (z_button == 0) { 
      Serial.print ("z_button "); 
    }
    if (c_button == 0) { 
      Serial.print ("c_button "); 
    }
    Serial.print ("\r\n");
  }
  else
    if  ((outbuf[5]&0x03)==0x02) {
      int yaw = (((outbuf[5]&0xFC)<<6) + outbuf[0]);
      int pitch = (((outbuf[4]&0xFC)<<6) + outbuf[1]);
      int roll = (((outbuf[3]&0xFC)<<6) + outbuf[2]);
      Serial.print ("yaw= ");
      Serial.print (yaw);
      Serial.print ("  pitch= ");
      Serial.print (pitch);
      Serial.print ("  roll= ");
      Serial.print (roll);
      Serial.print ("\r\n");
    }

}


