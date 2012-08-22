void setup(){
  Serial.begin(19200);
  delay(5000);
}

void loop(){
  long sum=0;
  for (int i=0;i<1000;i++){
    sum+=analogRead(0);
  }
  Serial.println(sum);
}
