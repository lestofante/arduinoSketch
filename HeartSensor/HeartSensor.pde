void setup(){
  Serial.begin(19200);
  delay(2000);
}

boolean basso=false;
unsigned long start;
int pulso=1;
void loop(){
  int a=analogRead(0);
  //if (a!=0)
  Serial.println(a);
/*  
  if (a < 512 && !basso){
    basso=true;
  }
  if (a > 521 && basso){
    basso=false;
    Serial.print(pulso);
    Serial.println("pulso");
    pulso++;
  }
*/  
/*
  int b=analogRead(1);
  if (b!=0){
    Serial.print("b:");
    Serial.println(b);
  }
  */  
  /*
  if (a>200){
    if (!conta){
      conta=true;
      start = micros();
    }
  }else{
    if (conta){
      conta=false;
      unsigned long d = micros()-start;
      Serial.println("INFO:");
      Serial.println(d);
      Serial.println( (d/2)/1000 );
      Serial.println();
    }
  }
  /*
  unsigned long d = pulseIn(A0, HIGH, 2000000L);
  Serial.println(d);
  Serial.println( (d/2)/1000 );
  Serial.println();
  /*
  int a=analogRead(0);
  
  
  if (a!=0){
    Serial.print( a );
    Serial.print("\n");
    zero=true;
  }else{
    if (zero){
      Serial.println("0");
      zero=false;
    }
  }
  
  //int b=analogRead(1);

//  Serial.print( b );
//  Serial.print("\n");
*/
}

