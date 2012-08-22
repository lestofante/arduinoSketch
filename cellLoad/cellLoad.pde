int midA, midB, midC;

void setup(){
  Serial.begin(19200);
  delay(2000);
}

void loop(){
  int a=analogRead(0);

  Serial.print( a );
  Serial.print("\n");
/*
  int b=analogRead(1);

  Serial.print( b );
  Serial.print(" ");
  
  Serial.print( a-b );
  Serial.print("\n");
  */
}

