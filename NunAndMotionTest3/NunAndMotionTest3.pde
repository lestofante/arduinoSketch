//Code for Arduino based communication with Wii Motion Plus and Nunchuck. 
//Nunchuck is plugged into WM+ extension port so it DOES NOT require
//additional expansion board. 
//Code, discussion, links, and references at:
//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1248889032/0
//influenced this code, primarily contributions by
//Knuckles904, Ed Simmons, Adrian Carter, and duckhead
//Additionally, information at 
//http://wiibrew.org/wiki/Wiimote/Extension_Controllers#Wii_Motion_Plus
//was very helpful.
//This code provides bases and/or example for more specific implementations

// Created by automatik (auto11)
// http://voidbot.net

//code and set up instructions at:
//http://voidbot.net/nunchchuk-and-wii-motion-plus-6-DOF.html

#include <Wire.h>

//Nunchuck
uint8_t outbuf[6];           //nunchuck buffer for data
int cnt = 0;                 //nunchuck buffer counter

//Wii Motion Plus
byte data[6];                //buffer for WM+ data
int yaw, pitch, roll;        //three gyroscope axes
int yaw0, pitch0, roll0;     //calibration zeroes
boolean yaw_v, pitch_v, roll_v; //calibration zeroes

void setup ()
{
  Serial.begin (19200);
  Wire.begin();             // join i2c bus as a master
  delay(100);

  //instead of averaging data , I used data
  //based on experiments from my particular WM+
  yaw0=8163;
  pitch0=7859;
  roll0=8536;
}

void loop ()
{
  // Data communciation is sensative hence
  // bunch of timed delays. These are just
  // what works on my setup so YMMV. If you
  // think data is incorect increase vakues 
  // for delays.

  wmpActive();                 //  activate WM+
  delay(50);
  extPort();                   //write 0x00 to 0x(4)A400fb
  delay(180); 
  send_zero();
  DataFlush();
  delay(30);
  send_zero();
  wmpRead();                   // read WM+ data ( gyro )
  wmpDeact();                  // deactivate WMP
  delay(50);
  send_zero();
  delay(70);
  nunchuckRead(); // read Nunchuck data ( accelerometer data and buttons )

  //  delay(250);

  //Serial.println("===============================================");
}

//common

//Request data
void send_zero ()
{
  Wire.beginTransmission (0x52); 
  Wire.send (0x00);		 // send zero
  Wire.endTransmission ();	 
}

void DataFlush() 
{
  int h=1;
  Wire.requestFrom(0x52,100);
  while (Wire.available())
  {
    Wire.receive();
    h++;
  }
}

//ext port
//write 0x00 to 0x(4)A400fb
void extPort()
{
  Wire.beginTransmission(0x52); 
  Wire.send(0xfb);
  Wire.send(0x00);
  Wire.endTransmission();
}

//Nunchuck

void nunchuckRead() 
{
  Wire.requestFrom (0x52, 6);	// request data from nunchuck
  while (Wire.available ())
  {
    outbuf[cnt] = Wire.receive ();	// receive nunchuck data
    cnt++;
  }

  send_zero (); // send the request for next bytes

  // If we recieved the 6 bytes, then print them 
  if (cnt >= 5)
  {
    nunchuckAssembData();
  }
  cnt = 0; //(NunChuck)
}


// Assemble and print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
void nunchuckAssembData()
{
  int joy_x_axis = outbuf[0];
  int joy_y_axis = outbuf[1];
  int accel_x_axis = outbuf[2] * 2 * 2; 
  int accel_y_axis = outbuf[3] * 2 * 2;
  int accel_z_axis = outbuf[4] * 2 * 2;

  int z_button = 0;
  int c_button = 0;

  // byte outbuf[5] contains bits for z and c buttons
  // it also contains the least significant bits for the accelerometer data
  // so we have to check each bit of byte outbuf[5]
  if ((outbuf[5] >> 0) & 1)
  {
    z_button = 1;
  }
  if ((outbuf[5] >> 1) & 1)
  {
    c_button = 1;
  }

  if ((outbuf[5] >> 2) & 1)
  {
    accel_x_axis += 2;
  }
  if ((outbuf[5] >> 3) & 1)
  {
    accel_x_axis += 1;
  }

  if ((outbuf[5] >> 4) & 1)
  {
    accel_y_axis += 2;
  }
  if ((outbuf[5] >> 5) & 1)
  {
    accel_y_axis += 1;
  }

  if ((outbuf[5] >> 6) & 1)
  {
    accel_z_axis += 2;
  }
  if ((outbuf[5] >> 7) & 1)
  {
    accel_z_axis += 1;
  }

  //Print Nunchuck Data
  /*
  Serial.print (joy_x_axis, DEC);
   Serial.print ("\t");
   
   Serial.print (joy_y_axis, DEC);
   Serial.print ("\t");
   
   Serial.print (accel_x_axis, DEC);
   Serial.print ("\t");
   
   Serial.print (accel_y_axis, DEC);
   Serial.print ("\t");
   
   Serial.print (accel_z_axis, DEC);
   Serial.print ("\t");
   
   Serial.print (z_button, DEC);
   Serial.print ("\t");
   
   Serial.print (c_button, DEC);
   Serial.print ("\t");
   
   Serial.print ("\r\n");
   */
  //A ENVOYER AU PORT SERIE
  Serial.print("|acc=");
  Serial.print(accel_x_axis, DEC);
  Serial.print(",");
  Serial.print(accel_y_axis, DEC);
  Serial.print(",");
  Serial.print(accel_z_axis, DEC);
  Serial.print("|but=");
  Serial.print(z_button, DEC);
  Serial.print(",");
  Serial.print(c_button, DEC);
  Serial.print("|joy:");
  Serial.print(joy_x_axis,DEC);
  Serial.print(",");
  Serial.println(joy_y_axis, DEC);
}


//Wii Motion Plus

//Activate Wii Motion Plus
void wmpActive(){
  Wire.beginTransmission(0x53);
  Wire.send(0xfe); 
  Wire.send(0x04);
  Wire.endTransmission(); 
}

//Deactivate Wii Motion Plus
void wmpDeact(){
  Wire.beginTransmission(0x52); 
  Wire.send(0xf0);
  Wire.send(0x55);
  Wire.endTransmission();
}


void wmpRead()
{
  Wire.requestFrom(0x52,6); //request data from WM+
  for (int i=0;i<6;i++)
  {
    data[i]=Wire.receive();
  }
  wmpAssembData (); //assemble WM+ data
}

//assemble Wii Motion Plus data bits
//and get difference from averge values
void wmpAssembData () 
{
  yaw=((data[3]>>2)<<8)+data[0];//-yaw0;
  roll=((data[4]>>2)<<8)+data[1];//-roll0;
  pitch=((data[5]>>2)<<8)+data[2];//-pitch0;


  // get difference from avg values
  // comment this out if you want to
  // get raw data values
  //yaw -= yaw0;
  //roll -= roll0;
  //pitch -= pitch0;

  //scaling  mode data
  yaw_v = data[3] & 2;
  roll_v = data[4] & 2;
  pitch_v = data[3] & 1;

  // print WM+ data  
  wmpPrintData (); 
}

// print Wii Motion Plus data
void wmpPrintData () 
{
  /*
Serial.print("yaw:");
   Serial.print(yaw); 
   Serial.print("\t");
   Serial.print("pitch:");
   Serial.print(pitch);
   Serial.print("\t");
   Serial.print("roll:");
   Serial.println(roll);
   
   Serial.print("yaw_v:");
   Serial.print(yaw_v);
   Serial.print("\t");
   Serial.print("pitch_v:");
   Serial.print(pitch_v);
   Serial.print("\t");
   Serial.print("roll_v:");
   Serial.println(roll_v);
   */
  //A ENVOYER AU PORT SERIE
  Serial.print("Yaw,pitch,roll=");
  Serial.print(yaw); 
  Serial.print(",");
  Serial.print(pitch); 
  Serial.print(",");
  Serial.print(roll); 
}



