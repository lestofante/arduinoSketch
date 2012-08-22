// .......................................................................
// Code for reading data from a Wii Motion plus device connected to a Nunchuck
// Links to other sites
// http://www.windmeadow.com/node/42   .... first time i saw arduino nunchuk interface
// http://www.kako.com/neta/2009-017/2009-017.html# .....Japanese site with lots of Wii info
// http://wiibrew.org/wiki/Wiimote/Extension_Controllers#Wii_Motion_Plus    .... the one and only
// http://randomhacksofboredom.blogspot.com/2009/06/wii-motion-plus-arduino-love.html
// ....original motion plus but not passthrough
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1248889032/35   .... great Kalman filter code
// thanks to duckhead and knuckles904. I will be using that code for sure.
// http://obex.parallax.com/objects/471/   .... ideas for passthrough
// by Krulkip
// Here is the bitmapping which is changed from the standard nunchuk mapping
// In Nunchuk mode:
//	    Bit
// Byte     7	 6	 5	 4	 3	 2	 1	 0
// 0	 SX<7-----------------------------------------------------0>
// 1	 SY<7-----------------------------------------------------0>
// 2	 AX<9-----------------------------------------------------2>
// 3	 AY<9-----------------------------------------------------2>
// 4	 AZ<9---------------------------------------------3>	1
// 5	 AZ<2-----1>     AY<1>   AX<1>   BC	BZ	 0	 0
// Please note also the loss of the bit0 resolution.
// Byte 5 Bit 0 and 1 is used for nunchuk (0 0) = nunchuk or motion plus  (1 0) is motion plus detection.
// Byte 4 Bit 0 which is the extention controller detection bit. (1 = extension present)
// Hardware Arduino with ATMega 168
// Connections SCA to AD4 (Analog4) and SCL to AD5 (Analog5)
//........................................................................
#include <Wire.h>
#include <string.h>
#include <stdio.h>
uint8_t outbuf[6];		// array to store arduino output
int ledPin = 13;
int xID;
unsigned long lastRead=millis(),last=millis();
void setup (){
  Serial.begin (19200);
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
  xID= outbuf[0] + outbuf[1] + outbuf[2] + outbuf[3] + outbuf[4] + outbuf[5];
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
  
  send_zero (); // send the request for next bytes
  lastRead=millis();
}

void send_zero (){
  Wire.beginTransmission(0x52);
  Wire.send(0x00);
  Wire.endTransmission();
}

int giro=0, nun=0, err=0;
void loop (){
  // now follows conversion command instructing extension controller to get
  // all sensor data and put them into 6 byte register within the extension controller
  if (lastRead+100 < millis()){
    
    //delay (100);
    Wire.requestFrom (0x52,6);
    outbuf[0] = Wire.receive();
    outbuf[1] = Wire.receive();
    outbuf[2] = Wire.receive();
    outbuf[3] = Wire.receive();
    outbuf[4] = Wire.receive();
    outbuf[5] = Wire.receive();

    if ((outbuf[5]&0x03)==0x00) {
      nun++;
      print ();
      //if (last+1000 <= millis() ){
      //  print ();
      //}
    }
    else if  ((outbuf[5]&0x03)==0x02){
      giro++;
    }
    else{
      err++;
    }
    send_zero (); // send the request for next bytes
    lastRead=millis();
  }
  if (last+1000 < millis() ){
    print ();
    if ((outbuf[4]&0x01)==0x01) {
      Serial.print("Ext con: ");
    }
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

}
void print () {
  // check if nunchuk is really connected
  if ((outbuf[4]&0x01)==0x01) {
    Serial.print("Ext con: ");
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



