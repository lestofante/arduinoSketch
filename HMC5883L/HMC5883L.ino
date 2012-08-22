#include <Wire.h> //I2C Arduino Library

#define address 0x1E //0011110b, I2C 7bit address of HMC5883

void setup(){
  //Initialize Serial and I2C communications
  Serial.begin(19200);
  delay(1000);
  Wire.begin();
  Serial.print("START ");
  
  //set output at 75Hz (should be 160Hz) and 8 averange
  byte zero = 0x00;
/*  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x78);
  Wire.endTransmission(); */
  
  //set output at 75Hz (should be 160Hz) and no averange
  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x18 + 1);
  Wire.endTransmission();
  
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x01); //single measurement mode
  Wire.endTransmission();
  Serial.println("SETUP OK");
}

long midX, midY, midZ;
long pX, pY, pZ;
long nX, nY, nZ;
long oX, oY, oZ;
float scaleX=0, scaleY=0, scaleZ=0;
float offsetX=0, offsetY=0, offsetZ=0;
unsigned long lastMillis = millis(), tempMillis, up;
void loop(){
  byte zero = 0x00;
  Serial.println("no bias");
  
  //set output at 75Hz (should be 160Hz) and no averange
  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x18);
  Wire.endTransmission();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x01); //single measurement mode
  Wire.endTransmission();
  
  normalRead();
  midX = midY = midZ = 0;
  for (int i=0; i < 100; i++){
      //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x01); //single measurement mode
    Wire.endTransmission();
    normalRead();
  }
  oX = midX/100;
  oY = midY/100;
  oZ = midZ/100;
  Serial.print(midX/100);
  Serial.print(":");
  Serial.print(midY/100);
  Serial.print(":");
  Serial.println(midZ/100);
  

  Serial.println("positive bias");  
  //set output at 75Hz (should be 160Hz) and no averange and positive bias
  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x19);
  Wire.endTransmission();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x01); //single measurement mode
  Wire.endTransmission();
  normalRead();
  midX = midY = midZ = 0;
  for (int i=0; i < 100; i++){
      //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x01); //single measurement mode
    Wire.endTransmission();
    normalRead();
  }
  Serial.print(midX/100);
  Serial.print(":");
  Serial.print(midY/100);
  Serial.print(":");
  Serial.println(midZ/100);
  pX = midX/100;
  pY = midY/100;
  pZ = midZ/100; 
  
  
  Serial.println("negative bias");
  //set output at 75Hz (should be 160Hz) and no averange and negative bias
  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x1A);
  Wire.endTransmission();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x01); //single measurement mode
  Wire.endTransmission();
  normalRead();
  midX = midY = midZ = 0;
  for (int i=0; i < 100; i++){
      //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x01); //single measurement mode
    Wire.endTransmission();
    normalRead();
  }
  Serial.print(midX/100);
  Serial.print(":");
  Serial.print(midY/100);
  Serial.print(":");
  Serial.println(midZ/100);
  
  
  nX = midX/100;
  nY = midY/100;
  nZ = midZ/100;
  
  Serial.println(pX-nX);
  Serial.println(pY-nY);
  Serial.println(pZ-nZ);
  
  float minV = min(pX-nX, min(pY-nY, pZ-nZ));
  scaleX = ( (pX-nX)/minV );
  scaleY = ( (pY-nY)/minV );
  scaleZ = ( (pZ-nZ)/minV );
  
  Serial.println( "Scaled" );
  Serial.println( (pX-nX)/ scaleX );
  Serial.println( (pY-nY)/ scaleY );
  Serial.println( (pZ-nZ)/ scaleZ );
  
  float gauss = ((pX-nX)/scaleX)/2;
  Serial.println( "Gauss" );
  Serial.println( gauss );
  
  offsetX = ( pX/scaleX - gauss );
  offsetY = ( pY/scaleY - gauss );
  offsetZ = ( pZ/scaleZ - gauss );
  
  Serial.println( "Offset" );
  Serial.println( offsetX );
  Serial.println( offsetY );
  Serial.println( offsetZ );
  
  //set output at 75Hz (should be 160Hz) and no averange
  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x18);
  Wire.endTransmission();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write((byte)0x00); //continus measurement mode
  Wire.endTransmission();
  
  //offsetX = offsetY = offsetZ = 0;
  
  normalRead();
  float g, minG=10000, maxG=0;
  for (int i=0; i <500; i++){
    midX = midY = midZ = 0;
    normalRead();
    Serial.print((midX/scaleX)-offsetX);
    Serial.print(":");
    Serial.print((midY/scaleY)-offsetY);
    Serial.print(":");
    Serial.print((midZ/scaleZ)-offsetZ);
    g = sqrt( pow((midX/scaleX)-offsetX, 2) + pow((midY/scaleY)-offsetY, 2) + pow((midZ/scaleZ)-offsetZ, 2) );
    if (g<minG){
      minG = g;
      Serial.print("Min G:");
      Serial.print(minG);
      Serial.print("MAX DIFF:");
      Serial.print(maxG-minG);
    }
    if (g>maxG){
      maxG = g;
      Serial.print("Max G:");
      Serial.print(maxG);
      Serial.print("MAX DIFF:");
      Serial.print(maxG-minG);
    }
    Serial.print("G:");
    Serial.print(g);
    Serial.println();
  }
  
  Serial.print("MAX DIFF:");
  Serial.print(maxG-minG);
}

void normalRead(){
  int x,y,z; //triple axis data
  //Serial.print("START READING 1");
  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  
 //Serial.print("START READING 2");
 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
  up++;
  tempMillis = millis()-lastMillis;
  if (tempMillis >= 1000){
    Serial.println(up);
    lastMillis = millis();
  }
  //Print out values of each axis
  midX+=x;
  midY+=y;
  midZ+=z;
//Serial.print("x: ");
/*
  Serial.print(x);
  Serial.print(":");
  Serial.print(y);
  Serial.print(":");
  Serial.println(z);
  */
  delay(13);
}

