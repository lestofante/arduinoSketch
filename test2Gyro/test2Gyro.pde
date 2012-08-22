#define gyroX 1
#define gyroY 2
#define gyroZ 3

unsigned long time;
void setup(){
  time = micros();
  Serial.begin(19200);
}

int x, y, z, i=0;
unsigned long timeMid=0;
unsigned long time2;
void loop(){
  i++;
  time2=micros();
  if (timeMid==0){
    timeMid=time2-time;
  }else{
    timeMid= (timeMid+(time2-time) )/2;
  }
  time=time2;
  /*
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(z);
  */
  if (i>1000){
    float value = (gyro-mid)*giroToRadiantAtSecond;
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(z);
    Serial.println(timeMid);
    i=0;
  }
  
  x=analogRead(gyroX);
  y=analogRead(gyroY);
  z=analogRead(gyroZ);
}
