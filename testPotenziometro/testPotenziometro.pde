void setup(){
  Serial.begin(19200);
}

void loop(){
  int x = analogRead(0);
  x = map(x, 0, 1023, 1, 100);
  Serial.println(x);
}
