#include <math.h>;

#define RADS (180/M_PI)
#define Metri_Per_Rad 6366692,0724

float a,b,c;
float Lat1,Lat2,Lon1,Lon2;
byte tmp;

void setup()
{
  // set the digital pin as output:
  pinMode(13, OUTPUT);   
  Serial.begin(9600);
  delay(1000);
  
  //42°51′28″N
  Lat1 = 42.857818;
  //012°33′49″E
  Lon1 = 12.563545;
  
  //42°51′31″N
  Lat2 = 42.858689;
  //012°33′42″E
  Lon2 = 12.561801;

  Serial.println("inizio test");   
}

  float lon1 = Lon1/RADS;
  float lon2 = Lon2/RADS;
  float lat1 = Lat1/RADS;
  float lat2 = Lat2/RADS;
  
unsigned long time;
void loop()
{
  lon1 = Lon1/RADS;
  lon2 = Lon2/RADS;
  lat1 = Lat1/RADS;
  lat2 = Lat2/RADS;
  
  astro();
  
  
  //lesto's formula are taken from http://williams.best.vwh.net/avform.htm
  lesto1();
  
  lesto2();
  
  Lat1+=1;
}
float cAstro;
float dAstro;

float cLesto1;
float dLesto1;

float cLesto2;
void astro(){
  
  time = micros();

  //for (int i=0; i < 10; i++){
  a = acos(sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(lon1-lon2));
  //b = a * Metri_Per_Rad;
  c = acos((sin(lat2) - sin(lat1) * cos(a)) / (cos(lat1) * sin(a)));
  if ( sin(lon2-lon1)>=0 )
    c=2*M_PI-c;
//  c *= RADS;
  //}

  time = micros()-time;
  cAstro = c;
  dAstro = a;
  Serial.println ("Astro:");
  Serial.print ("distance:");
  Serial.println ((c*RADS));
  Serial.print ("distance radians:");
  Serial.println ((c));
  Serial.print("time:");
  Serial.println (time);
}

void lesto1(){
  float d;
  float da;
  float db;
  time = micros();
  
  //for (int i=0; i < 10; i++){
  //b = a * Metri_Per_Rad;
    da = sin((lat1-lat2)/2);
    db = cos(lat1)*cos(lat2)*(sin((lon1-lon2)/2));
    d=2*asin(sqrt( da*da+db*db ));
    c = acos((sin(lat2) - sin(lat1) * cos(d)) / (cos(lat1) * sin(d)));
    if ( sin(lon2-lon1)>=0 )
      c=2*M_PI-c;
//  c *= RADS;
  //}
  time = micros()-time;
  
  cLesto1 = c;
  dLesto1 = d;
  
  Serial.println ("lesto1:");
  Serial.println ( (d* Metri_Per_Rad), DEC);
  Serial.print ("distance:");
  Serial.println ((c*RADS));
  Serial.print ("distance radians:");
  Serial.println ((c));
  Serial.print("time:");
  Serial.println (time);
}

void lesto2(){
  time = micros();
  //for (int i=0; i < 10; i++){
    c=mod(atan2(sin(lon1-lon2)*cos(lat2), cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon1-lon2)), 2*M_PI);
  //}
  time = micros()-time;
  
  cLesto2 = c;
  
  Serial.println ("Lesto2:");
//  Serial.println (b);
  Serial.println ((c*RADS));
  Serial.print("time:");
  Serial.println (time);
  Serial.println ();
  
  float a, b;
  
  time = micros();
  a = sin(lon1-lon2)*cos(lat2);
  time = micros()-time;
  Serial.print("time1:");
  Serial.println (time);
  
  time = micros();
  b = cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon1-lon2);
  time = micros()-time;
  Serial.print("time2:");
  Serial.println (time);
  
  time = micros();  
  c=mod(atan2(a, b), 2*M_PI);
  time = micros()-time;
  Serial.print("time3:");
  Serial.println (time);
}

float safeAsin(float a){
  return asin(max(-1, min(a, 1)));
}

float safeCos(float a){
  return asin(max(-1, min(a, 1)));
}

float mod(float y, float x){
  float ris=y - x * int(y/x);
  if ( ris < 0) 
    ris = ris + x;
  return ris;
}
