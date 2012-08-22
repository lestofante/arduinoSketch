void setup(){
  Serial.begin(19200);
  delay(5000);
}

void loop(){

    Serial.println( analogRead(A0) );
  delay(1);
}
