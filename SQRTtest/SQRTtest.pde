void setup(){
  Serial.begin(57600);
}

void loop(){
  Serial.println("Starting long calculation:");
  start();
  
  int a;
  unsigned long t;
  float ris=0;
  t = micros();
  for (a=0;a<6000;a++){
    ris+=a;
  }
  t=micros()-t;
  Serial.println(ris);
  Serial.print("Time for:");
  Serial.println(t);  

  ris=0;
  t = micros();
  for (a=0;a<6000;a++){
    ris+=sqrt(a);
    //Serial.print(ris);
  }
  Serial.println(ris);
  Serial.print("Time sqrt:");
  Serial.println(t);  
  
  ris=0;
  t = micros();
  for (a=0;a<6000;a++){
    ris+=Q_rsqrt(a);
//    Serial.print(ris);
  }
  t=micros()-t;
  Serial.println(ris);
  Serial.print("Time Q_rsqrt:");
  Serial.println(t);  

  ris=0;  
  t = micros();
  for (a=0;a<6000;a++){
    ris+=Q_rsqrt2(a);
//    Serial.print(ris);
  }
  t=micros()-t;
  Serial.println(ris);
  Serial.print("Time Q_rsqrt2:");
  Serial.println(t);
  
  delay(1000);
}

void start(){
  double diff;
  double maxDiff=0;
  double midDiff=0;
  double maxDiff2=0;
  double midDiff2=0;
  long count=0;
  
  float a;
  for (a=0;a<1000;a+=0.01){
    //printf("ciclo: %d e %f\n", count, a);
    count++;
    diff = (float)( sqrt(a) ) - 1.0/Q_rsqrt(a);
    midDiff+=diff;
    if (abs(diff)>maxDiff){
      maxDiff=abs(diff);/*
      Serial.print("1_diff is:");
      Serial.print(diff);
      Serial.print(" at:");
      Serial.println(a);*/
    }
    
    diff = (float)( sqrt(a) ) - 1.0/Q_rsqrt2(a);
    midDiff2+=diff;
    if (abs(diff)>maxDiff2){
      maxDiff2=abs(diff);/*
      Serial.print("2_diff is:");
      Serial.print(diff);
      Serial.print(" at:");
      Serial.println(a);*/
    }
  }

  Serial.print("maxdiff is:");
  if (maxDiff!=0)
    Serial.println(maxDiff, 7);
  else
    Serial.println("nodiff");
  Serial.print("middiff is:");
  Serial.println(midDiff/count, 7);
  
  Serial.print("maxdiff2 is:");
  if (maxDiff2!=0)
    Serial.println(maxDiff2, 7);
  else
    Serial.println("nodiff");
  Serial.print("middiff2 is:");
  Serial.println(midDiff2/count, 7);
}

float Q_rsqrt( float number ){
        long i;
        float x2, y;
        const float threehalfs = 1.5F;
 
        x2 = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
 //   y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
        return y;
}

float Q_rsqrt2( float number ){
        long i;
        float x2, y;
        const float threehalfs = 1.5F;
 
        x2 = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
        return y;
}
