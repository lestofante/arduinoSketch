void setup(){
//  analogReference(EXTERNAL);
  Serial.begin(57600);
  delay(2000);
}

void loop(){
  Serial.print(analogRead(0));
  Serial.print(" ");
  Serial.print(analogRead(1));
  //Serial.print(" ");
  //Serial.print(analogRead(2));
  //Serial.print(" ");
  Serial.println();
}
