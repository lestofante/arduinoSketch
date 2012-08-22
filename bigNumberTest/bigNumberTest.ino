#include "BigNumber.h"

#define RADS (180/M_PI)
#define Metri_Per_Rad 6366692,0724

const int precision = 20;
const int precisionTaylorSine = 20;
const int precisionAtan = 10;

BigNumber doublePI;
BigNumber halfPI;
BigNumber bigPI;

int myputc( char c, FILE *t){
  Serial.write(c);
  Serial.flush();
  return 0;
}

void setup(){
  Serial.begin(115200);
  delay(2000);
  fdevopen(&myputc, 0);
  BigNumber::begin ();
  BigNumber::setScale (precision);
  bigPI = BigNumber ("3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706");
  halfPI = bigPI;
  halfPI /= 2;
  doublePI = bigPI;
  doublePI *= 2;
  
  preloadAtanTable();
}

int i=1;
unsigned long time;
void loop()
{

  Serial.println( i );
  
  Serial.print( sin(i) );
  Serial.print(" ");
  printBignum( sine( i, precisionTaylorSine ) );
  Serial.println();

  Serial.print( cos(i) );
  Serial.print(" ");
  printBignum( cosine( i, precisionTaylorSine ) );
  Serial.println();
  Serial.println();
  delay(1000);
  i++;
  
  float same = -PI/2.0;
  BigNumber foo;
  foo = halfPI;
     printBignum (foo); 
  foo *= -1;
  printBignum (foo);
  Serial.println();
  for (; foo <= halfPI; foo += BigNumber ("0.1"))
    {
      
     Serial.print ("arctan of ");
     printBignum (foo);
     Serial.print (" = ");
     printBignum (arctangent (foo, precisionAtan));
     Serial.print (" atan of ");
     Serial.print (same); 
     Serial.print (" = ");
     Serial.print (atan(same));
     Serial.println ();
     same += 0.1;
    } // end of for
  
 // astro();
 // astro2();
  Serial.println();
  Serial.println();
    while(1);
}

// function to display a big number and free it afterwards
void printBignum (BigNumber n)
{
  char * s = n.toString ();
  Serial.print (s);
  free (s);
}  // end of printBignum

BigNumber sine (BigNumber x){
  return sine(x, precisionTaylorSine);
}

BigNumber sine (BigNumber x, BigNumber p)
{
//  Serial.println();
  while (x > doublePI){//stay in range!
     x -= doublePI;
//     Serial.print("m");
  }
  
  while (x < 0){//stay in range!
     x += doublePI;
//     Serial.print("M");
  }
  
  BigNumber one = 1;
  BigNumber two = 2;
  BigNumber val = one;

  while (p > 0)
  {
    val = one - val * x * x / (two * p) / (two * p + one);
    p = p - one;
  }
//  Serial.println("e");
  val = x * val;
  return val;
} // end of sine

BigNumber cosine (BigNumber x){
  return cosine(x, precisionTaylorSine);
}

BigNumber cosine (BigNumber x, BigNumber p)
{
  return sine(x+halfPI, p);
}  // end of cosine

BigNumber tangent (BigNumber x){
  return tangent(x, precisionTaylorSine);
}

BigNumber tangent (BigNumber x, BigNumber p)
{
  return (sine (x, p) / cosine (x, p));
} // end of tangent

uint8_t * heapptr, * stackptr;
void check_mem() {
  stackptr = (uint8_t *)malloc(4);          // use stackptr temporarily
  heapptr = stackptr;                     // save value of heap pointer
  free(stackptr);      // free up the memory again (sets stackptr to 0)
  stackptr =  (uint8_t *)(SP);           // save value of stack pointer
}

// ArcTan-table used for CORDIC (located in code space)
BigNumber atanTable[] = { BigNumber("0.78539816339745") ,  BigNumber("0.46364760900081") ,  BigNumber("0.24497866312686") ,  BigNumber("0.12435499454676") ,
    BigNumber("0.06241880999596") ,  BigNumber("0.03123983343027")  , BigNumber("0.01562372862048") ,  BigNumber("0.00781234106010") ,
    BigNumber("0.00390623013197") ,  BigNumber("0.00195312251648") };
    /*,  BigNumber("0.00097656218956") ,  BigNumber("0.00048828121119") ,
    BigNumber("0.00024414062015") ,  BigNumber("0.00012207031189") ,  BigNumber("0.00006103515617") ,  BigNumber("0.00003051757812") ,
    BigNumber("0.00001525878906") ,  BigNumber("0.00000762939453")  , BigNumber("0.00000381469727") ,  BigNumber("0.00000190734863") ,
    BigNumber("0.00000095367432") ,  BigNumber("0.00000047683716") ,  BigNumber("0.00000023841858") ,  BigNumber("0.00000011920929") ,
    BigNumber("0.00000005960464") ,  BigNumber("0.00000002980232") ,  BigNumber("0.00000001490116") ,  BigNumber("0.00000000745058") };
*/
void preloadAtanTable(){
  /* must call this first to initialise the constants.
     * of course, here i use the maths library, but the
     * values would be precomputed.
     */
/*    
    double t = 1;
    int i;
    for (i = 0; i < precision; ++i) {
        atanTable[i] = atan(t);
        t /= 2;
    }
    */
}

BigNumber arctangent(BigNumber y, BigNumber p)
{
  
  //http://www.voidware.com/cordic.htm
    /* this is the circular method. 
     * one slight change from the other methods is the y < vecmode 
     * test. this is to implement arcsin, otherwise it can be
     * y < 0 and you can compute arcsin from arctan using
     * trig identities, so its not essential.
     */

    BigNumber t = BigNumber(1);
    BigNumber x = BigNumber(1);
    BigNumber z = BigNumber(0);
    BigNumber x1 = x;
    
    BigNumber zero = BigNumber (0);
    
    BigNumber i = BigNumber(0);
    for (; i < p; i+=1) {


        if (y < zero) {
            x1 = x - y*t;
            y = y + x*t;
            z = z - atanTable[(long)i];
        }
        else {
            x1 = x + y*t;
            y = y - x*t;
            z = z + atanTable[(long)i];
        }

        x = x1;
        t /= 2;
    }
    return z;
}
/*
BigNumber arctangent(BigNumber x, BigNumber p) {
  BigNumber one = 1;
  BigNumber two = 2;
  BigNumber c = one;
  BigNumber den;
  BigNumber val = x;
  BigNumber xT;
  BigNumber countMul;
  boolean sign = false;
  for (c = one; c <= p; c+=one){
    {
      den = two * c + one;
    }
//    Serial.print("\nr");
//    check_mem();
//    Serial.print( stackptr - heapptr);
    xT = x;
    Serial.print("\ne");
    printBignum( xT );
    for (countMul = one; countMul < den; countMul+= one){
      xT *= x;
      Serial.print("\ne");
      printBignum( xT );
    }
        
/*
    Serial.print(" ");
    printBignum( x.pow(den) );
*/  /*
    xT /= den;
    if (sign){
      val += xT;
    }else{
      val -= xT;
    }
//    Serial.print("s");
//    Serial.print(sign);
    sign = !sign;
    Serial.println();
  }
  return val;
}


/*
BigNumber arctangent(BigNumber x, BigNumber p) {
  BigNumber one = 1;
  BigNumber two = 2;
  BigNumber den = 1;
  BigNumber oldDen = two * p + one;
  BigNumber xT = x.pow(oldDen);
  BigNumber val = 0;
  boolean sign;
  if ((long)p % two == 0){
    sign = true;
  }else{
    sign = false;
  }

  while (p > 0){
    den = two * p + one;
    Serial.print("\nd");
    printBignum( den );
    
    Serial.print("o");
    printBignum( oldDen );
    Serial.print("e");
    printBignum( xT );
    while (oldDen > den){//because we decrease!!
      Serial.print("D");
      xT/=x;
      Serial.print("e");
      printBignum( xT );
      oldDen = oldDen - one;
    }
    if (sign){
      val = val + (xT/den);
    }else{
      val = val - (xT/den);
    }
    sign = !sign;
    Serial.print("v");
    printBignum( val );
    p = p - one;
    Serial.println();
  }
  return x+val;
}

/*
BigNumber acosine(BigNumber x) {
  boolean negate = false;
  
  if (x.isNegative ()){
    x -= x;
    negate = true;
  }

  BigNumber ret = -0.0187293;
  ret *= x;
  ret += 0.0742610;
  ret *= x;
  ret -= 0.2121144;
  ret *= x;
  ret += 1.5707288;
  BigNumber bigSqrt = x - BigNumber (1);
  ret *= bigSqrt.sqrt();
  ret -= 2 * ret;
  if (negate)
    return - bigPI - ret;
  else
    return bigPI + ret;
}


float cAstro;
float dAstro;

void astro(){
  float lon1 = 12.563545/RADS;
  float lon2 = 12.561801/RADS;
  float lat1 = 42.857818/RADS;
  float lat2 = 42.858689/RADS;
  float a,c;
  
  time = micros();

  a = acos(sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(lon1-lon2));
  c = acos((sin(lat2) - sin(lat1) * cos(a)) / (cos(lat1) * sin(a)));
  if ( sin(lon2-lon1)>=0 )
    c=2*M_PI-c;
    
  time = micros()-time;
  cAstro = c;
  dAstro = a;
  Serial.println ("Astro:");
//  Serial.print ("distance:");
//  Serial.println ((c*RADS));
  Serial.print ("distance radians:");
  Serial.println ((c));
  Serial.print("time:");
  Serial.println (time);
}

float cAstro2;
float dAstro2;

void astro2(){
  BigNumber lon1 = BigNumber("12.563545");
  lon1 /= RADS;
  BigNumber lon2 = BigNumber("12.561801");
  lon2 /= RADS;
  BigNumber lat1 = BigNumber("42.857818");
  lat1 /= RADS;
  BigNumber lat2 = BigNumber("42.858689");
  lat2 /= RADS;
  BigNumber a,c;
  
  time = micros();

  BigNumber sinLat1 = sine(lat1);
//  Serial.print("a ");
  BigNumber sinLat2 = sine(lat2);
//  Serial.print("b ");
  BigNumber cosLat1 = cosine(lat1);  
//  Serial.print("c ");

  BigNumber part1a = sinLat1*sinLat2;
//  Serial.print("c1 ");
  BigNumber part1b = cosLat1*cosine(lat2);
//  Serial.print("c2 ");
  BigNumber part1c = cosine(lon1-lon2);
//  Serial.print("c3 ");
  BigNumber part1d = part1a+part1b*part1c;
//  Serial.print("c4 ");
  a = acos(part1d);
  
  
//  Serial.print("d ");
  BigNumber partD1 = sinLat2 - sinLat1;
//  Serial.print("dA:");
//  printBignum( a );
//  Serial.print(" ");
  BigNumber partD2 = cosine(a);
//  Serial.print("dB ");
  BigNumber part1 = partD1 * partD2;
//  Serial.print("d1 ");
  BigNumber part2 = (cosLat1 * sine(a));
//  Serial.print("d2 ");
  BigNumber part3 = part1/part2;
//  Serial.print("d3 ");
  c = acos( part3 );
//  Serial.print("e ");
  if ( sine(lon2-lon1)>=0 )
    c=2*M_PI-c;
    
  time = micros()-time;
  cAstro = c;
  dAstro = a;
  Serial.println ("Astro2:");
//  Serial.print ("distance:");
//  Serial.println ((c*RADS));
  Serial.print ("distance radians:");
  //Serial.println ((c));
  printBignum( c );
  Serial.println();
  Serial.print("time:");
  Serial.println (time);
}
/*
void lesto2(){
  BigNumber lon1 = BigNumber("12.563545");
  lon1 /= RADS;
  BigNumber lon2 = BigNumber("12.561801");
  lon2 /= RADS;
  BigNumber lat1 = BigNumber("42.857818");
  lat1 /= RADS;
  BigNumber lat2 = BigNumber("42.858689");
  lat2 /= RADS;
  BigNumber c;
  
  time = micros();

  c=mod(arctan2(sine(lon1-lon2)*cosine(lat2), cosine(lat1)*sine(lat2)-sine(lat1)*cosine(lat2)*cosine(lon1-lon2)), doublePI);
    
  time = micros()-time;
  cAstro = c;
  dAstro = a;
  Serial.println ("Lesto2:");
//  Serial.print ("distance:");
//  Serial.println ((c*RADS));
  Serial.print ("distance radians:");
  //Serial.println ((c));
  printBignum( c );
  Serial.println();
  Serial.print("time:");
  Serial.println (time);
}

BigNumber mod(BigNumber y, BigNumber x){
  BigNumber ris=y - x * int(y/x);
  if ( ris < 0) 
    ris = ris + x;
  return ris;
}*/


