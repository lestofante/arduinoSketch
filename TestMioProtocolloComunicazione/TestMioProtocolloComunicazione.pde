

void setup(){
  Serial.begin(19200);
}

void loop(){
  int a;
  unsigned long time2;
  
  pinMode(12, INPUT);
  unsigned long time1 = micros();
  for (int i=0; i < 1000; i++){
    a=digitalRead(12);
  }
  time2 = micros()-time1;
  
  Serial.print("1000 read duration:");
  Serial.println(time2);
  
  pinMode(12, OUTPUT);
  boolean stat=false;
  time1 = micros();
  for (int i=0; i < 1000; i++){
    digitalWrite(12, stat?HIGH:LOW);
    stat=!stat;
  }
  time2 = micros()-time1;
  
  Serial.print("1000 write duration:");
  Serial.println(time2);
}
