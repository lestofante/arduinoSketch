void setup(){
  Serial.begin(19200);
  
}

void loop(){
  int i;
  float a;
  unsigned long time;
  
  time = micros();
  for (i=0; i < 1000; i++){
    a=sqrt(i);
    Serial.print(a);
    Serial.print(" ");
  }
  time = micros()-time;
  Serial.println();
  Serial.print("sqrt:");
  Serial.println(time);
  
  time = micros();
  for (i=0; i < 1000; i++){
    a=invSqrt(i);
    Serial.print(a);
    Serial.print(" ");
  }
  time = micros()-time;
  Serial.println();  
  Serial.print("invSqrt:");
  Serial.println(time);
  
  float diff, maxDiff=0, t;
  for (i=0; i < 1000; i++){
    t = ( invSqrt(i) )-((float) sqrt(i) );
    Serial.print(invSqrt(i));
    Serial.print(" ");
    Serial.print(1/sqrt(i));
    Serial.print(" ");
    Serial.print(t);
    Serial.println(" ");
    
    if (  abs(t) > abs(maxDiff) )
      maxDiff=t;
  }
  Serial.print("maxDiff:");
  Serial.println(maxDiff, 10);
}

float invSqrt(float number) {
  long i;
  float x, y;
  const float f = 1.5F;

  x = number * 0.5F;
  y = number;
  i = * ( long * ) &y;
  i = 0x5f375a86 - ( i >> 1 );
  y = * ( float * ) &i;
  y = y * ( f - ( x * y * y ) );
  return y;
}
