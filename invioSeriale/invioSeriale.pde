void setup(){
  Serial.begin(57600);
  delay(2000);
  Serial.println( "partito!" );
}

union u_tag {
    byte b[4];
    float fval;
} u;

void loop(){
  //delay(1000);
  int a = Serial.read();

  if (a!=-1)
    Serial.println( a, BYTE );
  
  if (a=='p'){
    Serial.println("reading float");
    for (int i=0;i<4;i++){
      u.b[i]=Serial.read();
    }
    Serial.println( u.fval );
  }
  /*
  u.fval=10;
  for (int i=0;i<4;i++){
    Serial.print(u.b[i], BIN);
    Serial.print(" ");
  }
  Serial.println("end");
  /*
  if (Serial.available()>4){
    Serial.print( "dispo:");
    Serial.println( Serial.available() );
    int a = Serial.read();

    Serial.println( a, BYTE );
    
    for (int i=0;i<4;i++){
      u.b[i]=Serial.read();
    }
    Serial.println( u.fval );
    
  }
  */
}
