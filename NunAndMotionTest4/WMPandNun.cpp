#include "WMPandNun.h"

//modified version from Krulkip's code at: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1248889032/45
unsigned long lastRead=millis();

void WMPandNun::init(){
  //send initialization handshake
  isNun=false;
  Wire.begin();
  Serial.print ("Finished setup\n");
  Serial.print ("Now detecting WM+\n");
  delay(100);
  // now make Wii Motion plus the active extension
  // nunchuk mode = 05, wm+ only = 04, classic controller = 07
  Serial.print ("Making Wii Motion plus the active extension in nunchuk mode = 05 ........");
  Wire.beginTransmission(0x53);
  Wire.send(0xFE);
  Wire.send(0x05);// nunchuk
  Wire.endTransmission();
  Serial.print (" OK done");
  Serial.print ("\r\n");
  delay (100);
  // now innitiate Wii Motion plus
  Serial.print ("Innitialising Wii Motion plus ........");
  Wire.beginTransmission(0x53);
  Wire.send(0xF0);
  Wire.send(0x55);
  Wire.endTransmission();
  Serial.print (" OK done");
  Serial.print ("\r\n");
  delay (100);
  Serial.print ("Set reading address at 0xFA .......");
  Wire.beginTransmission(0x52);
  Wire.send(0xFA);
  Wire.endTransmission();
  Serial.print(" OK done");
  Serial.print ("\r\n");
  delay (100);
  Wire.requestFrom (0x52,6);
  outbuf[0] = Wire.receive();
  Serial.print(outbuf[0],HEX);
  Serial.print(" ");
  outbuf[1] = Wire.receive();
  Serial.print(outbuf[1],HEX);
  Serial.print(" ");
  outbuf[2] = Wire.receive();
  Serial.print(outbuf[2],HEX);
  Serial.print(" ");
  outbuf[3] = Wire.receive();
  Serial.print(outbuf[3],HEX);
  Serial.print(" ");
  outbuf[4] = Wire.receive();
  Serial.print(outbuf[4],HEX);
  Serial.print(" ");
  outbuf[5] = Wire.receive();
  Serial.print(outbuf[5],HEX);
  Serial.print(" ");
  Serial.print ("\r\n");
  int xID= outbuf[0] + outbuf[1] + outbuf[2] + outbuf[3] + outbuf[4] + outbuf[5];
  Serial.print("Extension controller xID = 0x");
  Serial.print(xID,HEX);
  if (xID == 0xCB) { 
    Serial.print (" Wii Motion plus connected but not activated"); 
  }
  if (xID == 0xCE) { 
    Serial.print (" Wii Motion plus connected and activated"); 
  }
  if (xID == 0x00) { 
    Serial.print (" Wii Motion plus not connected"); 
  }
  Serial.print ("\r\n");
  delay (100);
  // Now we want to point the read adress to 0xa40008 where the 6 byte data is stored
  Serial.print ("Set reading address at 0x08 .........");
  Wire.beginTransmission(0x52);
  Wire.send(0x08);
  Wire.endTransmission();
  Serial.print(" OK done");
  Serial.print ("\r\n");
  
  //send_zero (); // send the request for next bytes
  
  wmpRead = nunRead = false;

  extrapolateMidWMP();
  
  lastRead=millis();
}

void WMPandNun::extrapolateMidWMP(){
  int sampleNumber = 1000; //about 5 seconds of sample

  midValue[0]=midValue[1]=midValue[2]=0;

  for (int i=0; i < sampleNumber; i++){
    send_zero (); // send the request for next bytes
    //delay (3);
    Wire.requestFrom (0x52,6);
    if (Wire.available() < 6){
      Serial.println("SLOW!");
    }
    outbuf[0] = Wire.receive();
    outbuf[1] = Wire.receive();
    outbuf[2] = Wire.receive();
    outbuf[3] = Wire.receive();
    outbuf[4] = Wire.receive();
    outbuf[5] = Wire.receive();
    if  ((outbuf[5]&0x03)==0x02) {
      //READING WII MOTION PLUS
      midValue[0] += ( ( (outbuf[3]>>2) <<8) + outbuf[0]);
      midValue[1] += ( ( (outbuf[4]>>2) <<8) + outbuf[1]);
      midValue[2] += ( ( (outbuf[5]>>2) <<8) + outbuf[2]);
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
/*
boolean WMPandNun::update(){
  if (lastRead+3 < millis()){
    //send_zero (); // send the request for next bytes
//    Serial.println("ciclo");
    //delay (3);
    Wire.requestFrom (0x52,6);
    if (Wire.available() < 6){
      Serial.println("SLOW!");
    }
    //Serial.println( Wire.available(), DEC );  
    outbuf[0] = Wire.receive();
    outbuf[1] = Wire.receive();
    outbuf[2] = Wire.receive();
    outbuf[3] = Wire.receive();
    outbuf[4] = Wire.receive();
    outbuf[5] = Wire.receive();
    if ((outbuf[4]&0x01)==0x01) {
      Serial.print("Ext con: ");
    }
    if ((outbuf[5]&0x03)==0x00) {
      //READING NUNCHUCK
//      Serial.print("nun ok");
      data[3] = (outbuf[2] << 2) + ((outbuf[5] >> 3) & 2);
      data[4] = (outbuf[3] << 2) + ((outbuf[5] >> 4) & 2);
      data[5] = (outbuf[4] << 2) + ((outbuf[5] >> 5) & 6);
      Serial.print("nun ok: ");
    Serial.print(data[3]);
      Serial.print(" ");
    Serial.print(data[4]);
      Serial.print(" ");
    Serial.print(data[5]);
    Serial.println();
                              
      nunRead=true;
    }
    else if  ((outbuf[5]&0x03)==0x02) {
      //READING WII MOTION PLUS
//      Serial.print("WMP ok");

      data[0] = ( (((outbuf[3]>>2)<<8) + outbuf[0]) - midValue[0] )*numberToVoltage[0];
      data[1] = ( (((outbuf[4]>>2)<<8) + outbuf[1]) - midValue[1] )*numberToVoltage[1];
      data[2] = ( (((outbuf[5]>>2)<<8) + outbuf[2]) - midValue[2] )*numberToVoltage[2];

      /*  
       //
       // FOR DEBUG
       //
       
       data[0] = (((outbuf[3]>>2)<<8) + outbuf[0]);
       Serial.println(data[0]);
       data[1] = (((outbuf[4]>>2)<<8) + outbuf[1]);
       Serial.println(data[1]);
       data[2] = (((outbuf[5]>>2)<<8) + outbuf[2]);
       Serial.println(data[2]);
       
       data[0] = ( data[0] - midValue[0] )*numberToVoltage[0];
       data[1] = ( data[1] - midValue[1] )*numberToVoltage[1];
       data[2] = ( data[2] - midValue[2] )*numberToVoltage[2];
       *//*
      wmpRead=true;

      //At high speed (slow bit = 0) sensibility is 2.27mV/°/s, at low speed is 0.5mV/°/s. Also tolerance is +-0.05V
      if (abs(data[0]) > 0.05){
        if ( outbuf[3] & B00000010 ){
          //slow speed        
          data[0]*=500;
        }
        else{
          data[0]*=2270;
        }
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
      }
      else{
        data[2]=0;
      }
    }

    if (nunRead && wmpRead){
      wmpRead = nunRead = false;
      return true;
    }
    
    send_zero (); // send the request for next bytes

    lastRead=millis();
  }
  return false;
}
*/

int giro=0, nun=0, err=0, lettura=0;
boolean WMPandNun::update(){
  if (lastRead+100 <= millis()){
    lettura++;
    //delay (100);
    Wire.requestFrom (0x52,6);
    if ((outbuf[4]&0x01)==0x01) {
      isNun=true;
    }else{
      isNun=false;
    }
    outbuf[0] = Wire.receive();
    outbuf[1] = Wire.receive();
    outbuf[2] = Wire.receive();
    outbuf[3] = Wire.receive();
    outbuf[4] = Wire.receive();
    outbuf[5] = Wire.receive();

    if ((outbuf[5]&0x03)==0x00) {
      nun++;
      data[3] = (outbuf[2] << 2) + ((outbuf[5] >> 3) & 2);
      data[4] = (outbuf[3] << 2) + ((outbuf[5] >> 4) & 2);
      data[5] = (outbuf[4] << 2) + ((outbuf[5] >> 5) & 6);
      //print ();
    }
    else if  ((outbuf[5]&0x03)==0x02){
      giro++;
      data[0] = ( (((outbuf[3]>>2)<<8) + outbuf[0]) - midValue[0] )*numberToVoltage[0];
      data[1] = ( (((outbuf[4]>>2)<<8) + outbuf[1]) - midValue[1] )*numberToVoltage[1];
      data[2] = ( (((outbuf[5]>>2)<<8) + outbuf[2]) - midValue[2] )*numberToVoltage[2];
      //print ();
    }
    else{
      err++;
    }
    send_zero (); // send the request for next bytes
    lastRead=millis();
  }
  if (giro>0 && nun>0){
    giro=nun=0;
    return true;
  }
  return false;
/*
  if (last+1000 < millis() ){
    //    print ();
    Serial.print("giro:");
    Serial.print(giro);
    Serial.print(" nun:");
    Serial.print(nun);
    Serial.print(" err:");
    Serial.print(err);
    Serial.println();
    last=millis();
    giro=nun=err=0;
  }
*/
}

float* WMPandNun::leggi(){
  return data;
}

void WMPandNun::send_zero (){
  Wire.beginTransmission(0x52);
  Wire.send(0x00);
  Wire.endTransmission();
}

void WMPandNun::debug(){
  Serial.print("Dati: ");
  for (int i=0;i<6;i++){
    Serial.print(" ");
    Serial.print(data[i]);
  }
  Serial.println();

  Serial.println( Wire.available(), DEC );  

  Serial.print("giro:");
  Serial.print(giro);
  Serial.print(" nun:");
  Serial.print(nun);
  Serial.print(" err:");
  Serial.println(err);
  err=0;
  
  Serial.print("letture al secondo:");
  Serial.println(lettura);
  lettura=0;

  Serial.print("There is nun:");
  Serial.println(isNun?"true":"false");  
  wmpRead = nunRead = false;

}

void WMPandNun::print () {
  // check if nunchuk is really connected
  
  if ((outbuf[4]&0x01)==0x01) {
    Serial.print("Ext con: ");
  }else{
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

