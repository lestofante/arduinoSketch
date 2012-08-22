int midA, midB, midC;

void setup(){
  Serial.begin(19200);
  delay(1000);
  analogReference(EXTERNAL);
  unsigned long time1 = millis()+1000;
  unsigned long a =0, b=0, c=0;
  int count =0;
  while( time1>millis() ){
    count++;
    a+=analogRead(0);
    b+=analogRead(1);
    c+=analogRead(2);
    //            delayMicroseconds(222);
  }

  a/=count;
  midA=517;
  Serial.print( a );
  Serial.print(" ");

  b/=count;
  midB=510;
  Serial.print( b );
  Serial.print(" ");

  c/=count;  
  midC=513;
  Serial.print( c );
  Serial.print(" ");

  Serial.print("\t");
  Serial.print( count );
  Serial.println();
}

void loop(){
  unsigned long time1 = millis()+1000;
  long a =0, b=0, c=0;
  int count =0;
  while( time1>millis() ){
    count++;
    a+=analogRead(0);
    b+=analogRead(1);
    c+=analogRead(2);
    //delayMicroseconds(222);
  }

  a/=count;
  Serial.print( a );
  Serial.print(" ");

  b/=count;
  Serial.print( b );
  Serial.print(" ");

  c/=count;  
  Serial.print( c );
  Serial.print(" ");

  Serial.print("\t");
  Serial.print( count );
  Serial.println();

  a-=midA;
  Serial.print( a );
  Serial.print(" ");

  b-=midB;
  Serial.print( b );
  Serial.print(" ");

  c-=midC;  
  Serial.print( c );
  Serial.print(" ");

  Serial.print("\t");
  Serial.print( sqrt(a*a+b*b+c*c) );
  Serial.println();
  Serial.println();
  delay(500);
}

