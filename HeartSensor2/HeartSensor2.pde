void setup(){
  Serial.begin(19200);
  delay(2000);
  attachInterrupt(0, conta, CHANGE);
}

volatile int count=0;

void loop(){
  delay(10000);
  Serial.println(count*3);
  count=0;
}
boolean v=false;
void conta(){
  count++;
  digitalWrite(13, v?HIGH:LOW);
  v!=v;
}



