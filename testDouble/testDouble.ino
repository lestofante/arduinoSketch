#include "avr_f64.h"

#define RADS (180/M_PI)
#define Metri_Per_Rad 6366692,0724

float Lat1,Lat2,Lon1,Lon2;
void setup(){
  Serial.begin(115200);
  delay(2000);
  
    //42°51′28″N
  Lat1 = 42.857818;
  //012°33′49″E
  Lon1 = 12.563545;
  
  //42°51′31″N
  Lat2 = 42.858689;
  //012°33′42″E
  Lon2 = 12.561801;
}


float i=1;
unsigned long time;
void loop()
{
  
  astro();
  
  astro2();
  
  Serial.println( i );
  Serial.println( sin(i) );
  Serial.println( f_to_string( f_sin(f_sd(i)), 20, 20) );
  Serial.println();
  delay(1000);
  i++;
  
  //lesto's formula are taken from http://williams.best.vwh.net/avform.htm
  //lesto1();
  
  //lesto2();
  
  //Lat1+=1;
}
float cAstro;
float dAstro;

void astro(){
  float lon1 = Lon1/RADS;
  float lon2 = Lon2/RADS;
  float lat1 = Lat1/RADS;
  float lat2 = Lat2/RADS;
  float a,b,c;
  
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

float64_t cAstro2;
float64_t dAstro2;
void astro2(){
  
  float64_t lon1 = f_sd(Lon1/RADS );
  float64_t lon2 = f_sd(Lon2/RADS);
  float64_t lat1 = f_sd(Lat1/RADS);
  float64_t lat2 = f_sd(Lat2/RADS);
  float64_t a, a1, a2, a3, a4 ,b,c,c1,c2,c3,c4;
  
  time = micros();

  //for (int i=0; i < 10; i++){
  a1 = f_mult( f_sin(lat1), f_sin(lat2) );
  
  a2 = f_mult( f_cos(lat1), f_cos(lat2) );
  a3 = f_mult( a2, f_cos( f_sub(lon1,lon2) ) );
  
  a4 = f_add( a1, a3);
  
  a = f_arccos( a4 );
  //b = a * Metri_Per_Rad;
  c1 = f_mult( f_cos(lat1), f_sin(a) );
  
  c2 = f_mult( f_sin(lat1), f_cos(a) );
  c3 = f_sub( f_sin(lat2), c2 );
  
  c4 = f_div( c3, c1);
  c = f_arccos( c4 );
  /*
  if ( f_sin( f_sub(lon2,lon1))>=0 )
    c=f_mult(f_sd(2*M_PI),c);
//  c *= RADS;
  //}
*/
  time = micros()-time;
  cAstro2 = c;
  dAstro2 = a;
  Serial.println ("Astro2:");
  Serial.print ("distance:");
  float64_t rads = f_sd(RADS);
  float64_t d = f_mult(c, rads);
  Serial.println (f_to_string(d, 20, 20));
  Serial.print ("distance radians:");
  Serial.println (f_to_string(c, 20, 20));
  Serial.print("time:");
  Serial.println (time);
}
