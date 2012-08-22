#include <Wire.h>
byte data[6]; //six data bytes
int yaw, pitch, roll; //three axes
int yaw0, pitch0, roll0; //calibration zeroes

void wmpOn(){
  Serial.println("ON1");
  Wire.beginTransmission(0x53); //WM+ starts out deactivated at address 0x53
  Wire.send(0xfe); //send 0x04 to address 0xFE to activate WM+
  Wire.send(0x04);
  Wire.endTransmission(); //WM+ jumps to address 0x52 and is now active
  Serial.println("ON2");
}

void wmpSendZero(){
//  Serial.println("Zero1");
  Wire.beginTransmission(0x52); //now at address 0x52
//  Serial.println("Zero2");
  Wire.send(0x00); //send zero to signal we want info
//  Serial.println("Zero3");
  Wire.endTransmission();
//  Serial.println("Zero4");
}

void calibrateZeroes(){
  Serial.println("Cal1");
  for (int i=0;i<10;i++){
    Serial.println("Cal2");
    wmpSendZero();
    Serial.println("Cal3");
    Wire.requestFrom(0x52,6);
    Serial.println("Cal4");
    for (int i=0;i<6;i++){
      data[i]=Wire.receive();
    }
    yaw0+=(((data[3]>>2)<<8)+data[0])/10; //average 10 readings
    pitch0+=(((data[4]>>2)<<8)+data[1])/10;
    roll0+=(((data[5]>>2)<<8)+data[2])/10;
  }
  Serial.print("Yaw0:");
  Serial.print(yaw0);
  Serial.print(" Pitch0:");
  Serial.print(pitch0);
  Serial.print(" Roll0:");
  Serial.println(roll0);
}

void receiveData(){
  wmpSendZero(); //send zero before each request (same as nunchuck)
//  Serial.println("WM+ testerA");
  Wire.requestFrom(0x52,6); //request the six bytes from the WM+
//  Serial.println("WM+ testerB");
  for (int i=0;i<6;i++){
    data[i]=Wire.receive();
  }
  
  yaw=(((data[3]>>2)<<8)+data[0])-yaw0;
  if ( data[3] & B00000010 ){
          //slow speed        
    yaw/=20;
  }else{
    yaw/=4.4;
  }
  
  pitch=(((data[4]>>2)<<8)+data[1])-pitch0;
  if ( data[4] & B00000010 ){
    pitch/=20;
  }else{
    pitch/=4.4;
  }
  
  roll=(((data[5]>>2)<<8)+data[2])-roll0;
  if ( data[3] & B00000001 ){
    roll/=20;
    //digitalWrite(13, LOW);
  }else{
    pitch/=4.4;
    //digitalWrite(13, HIGH);
  }
}
//see http://wiibrew.org/wiki/Wiimote/Extension_Controllers#Wii_Motion_Plus
//for info on what each byte represents
void setup(){
//  pinMode(13, OUTPUT);
//  digitalWrite(13, LOW);
  Serial.begin(19200);
  Serial.println("WM+ tester0");
  Wire.begin();
  Serial.println("WM+ tester1");
  wmpOn(); //turn WM+ on
  Serial.println("WM+ tester2");
  calibrateZeroes(); //calibrate zeroes
  Serial.println("WM+ tester3");
  delay(1000);
}
unsigned long time = micros();
unsigned long time2 = micros();
double sumYaw=0,sumPitch=0,sumRoll=0;
int count=0;
void loop(){
  time = time2;
  time2=micros();
  
  receiveData(); //receive data and calculate yaw pitch and roll
  
  if (abs(yaw)>2){
    sumYaw += yaw*( (time2-time)/1000000.0 );
    //digitalWrite(13, LOW);
  }//else
    //digitalWrite(13, HIGH);
  
  if (abs(pitch)>2){
    sumPitch += pitch*( (time2-time)/1000000.0 );
//    digitalWrite(13, LOW);
  }//else
//    digitalWrite(13, HIGH);
  
  if (abs(roll)>2){
    sumRoll += roll*( (time2-time)/1000000.0 );
//    digitalWrite(13, LOW);    
  }//else
//    digitalWrite(13, HIGH);
  
  
  if (count>100){
    Serial.println("values are in degree");
    Serial.print("yaw:");//see diagram on randomhacksofboredom.blogspot.com
    Serial.print(yaw); //for info on which axis is which
    Serial.print(" pitch:");
    Serial.print(pitch);
    Serial.print(" roll:");
    Serial.println(roll);
    //Serial.println();
 
    Serial.print("sumYaw:");//see diagram on randomhacksofboredom.blogspot.com
    Serial.print(sumYaw); //for info on which axis is which
    Serial.print(" sumPitch:");
    Serial.print(sumPitch);
    Serial.print(" sumRoll:");
    Serial.println(sumRoll);
    Serial.println();
    //Serial.println();  
    count=0;
  }
  count++;
  //delay(100);
}

