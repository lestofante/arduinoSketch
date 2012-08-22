/*
	swRTC.h - A software implementation of a Real-Time Clock (RTC) using the timer2 of Arduino
	Written by Leonardo Miliani <leonardo AT leonardomiliani DOT com>
	with contribution of lesto <www.arduino.cc>
	Inspired by MsTimer2 by Javier Valencia

	History:
        swRTC2:
        v. 0.9.1: use timestamp instad of date in ISR routine - by lesto
	v. 0.9:   forked the original swRTC and introduced a new system of time computation
	
        swRTC:
        v. 0.8.2-lesto: use timestamp instad of date in ISR routine
	v. 0.8.2: fixed a glitch in deltaT and introduced support for deltaT with decimals
	v. 0.8.1: new deltaT based on seconds/day
	v. 0.8:   introduced deltaT to correct deviations between computed & real time & Timestamp
	v. 0.7.1: dropped millis sketch due to the fact that milliseconds is no longer required
	v. 0.7:   corrected some bug; the support for Attinyx4 now works
	v. 0.6:   added supporto for Attiny2313
	v. 0.5:   corrected some bugs 
	v. 0.4:   addedd support for Atmega640/1280/1281/2560/2561
	v. 0.3:   added support for Attiny24/44/84 and Atmega644/Atmega644P (Attinyx4 don't work at the moment: FIXED)
	v. 0.2:   added support for Attiny25/45/85 and Atmega8/8A
	v. 0.1:   first release - only works on Arduino and Atmega48/88/168/328/328P at 1/8/16 MHz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


//include required libraries
#include "WProgram.h"
#include "swRTC.h"
#include <avr/interrupt.h>

//declare some global variables
swRTC *lib;

volatile unsigned long counterT=0;
volatile unsigned long timestamp=0;

volatile unsigned long overflows;
volatile unsigned long secondsRapresented;
volatile byte starter=0;
volatile unsigned long delta=0;

byte daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//instructions to execute when a new istance of the lib is created
swRTC::swRTC(void) {
	isRunning=false;
	lib=this;
	setTimer();
}


//
//private methods
//

//set the timer
void swRTC::setTimer() {
#if defined (ATMEGAx8) || defined (ATMEGA644) || defined (ATMEGAx0)
	//on those chips, we are going to use timer 2
	
	//during setup, disable all the interrupts based on timer 2
	TIMSK2 &= ~(1<<TOIE2); 
	TIMSK2 &= ~((1<<OCIE2A) | (1<<OCIE2B));
	//prescaler source clock set to internal Atmega clock (asynch mode)
	ASSR &= ~(1<<AS2);
	//this sets the timer to increment the counter until overflow
	TCCR2A &= ~((1<<WGM21) | (1<<WGM20)); 
	TCCR2B &= ~(1<<WGM22);
	//the following code sets the prescaler to 1024
	TCCR2B |= ((1<<CS22) | (1<<CS21) | (1<<CS20));

#elif defined (ATTINYx5) || defined (__AVR_ATtiny2313__)
	//on those chips, we are going to use timer 0
	
	//during setup, disable all the interrupts based on timer 0
	TIMSK &= ~(1<<TOIE0); 
	TIMSK &= ~((1<<OCIE0A) | (1<<OCIE0B));
	//normal mode: counter not connected to external pins
	TCCR0A &= ~((1<<COM0A0) | (1<<COM0A1));
	//this sets the timer to increment the counter until overflow
	TCCR0A &= ~((1<<WGM01) | (1<<WGM00));
	TCCR0B &= ~(1<<WGM02);
	//the following code sets the prescaler to 1024
	TCCR0B |= ((1<<CS02) | (1<<CS00));
	TCCR0B &= ~(1<<CS01);

#elif defined (ATTINYx4)
	//on those chips, we are going to use timer 0
		
	//during setup, disable all the interrupts based on timer 0
	TIMSK0 &= ~(1<<TOIE0);
	TIMSK0 &= ~((1<<OCIE0A) | (1<<OCIE0B));
	//normal mode: increment counter until overflow
	TCCR0B &= ~(1<<WGM02);
	TCCR0A &= ~((1<<WGM01) | (1<<WGM00));
	//normal mode: disconnect timer from pins
	TCCR0A &= ~((1<<COM0A0) | (1<<COM0A1));
	//the following code sets the prescaler to 1024
	TCCR0B |= ((1<<CS02) | (1<<CS00));
	TCCR0B &= ~(1<<CS01);

#elif defined (ATMEGA8)
	//on those chips, we are going to use timer 1
	
	//during setup, disable all the interrupts based on timer 1
	TIMSK &= ~(1<<TOIE2);
	TIMSK &= ~(1<<OCIE2);
	//normal mode: counter incremented until overflow
	TCCR2 &= ~((1<<WGM21) | (1<<WGM20));
	//prescaler source clock set to internal Atmega clock (asynch mode)
	ASSR &= ~(1<<AS2);
	//the following code sets the prescaler to 1024
	TCCR2 |= ((1<<CS22) | (1<<CS21) | (1<<CS20));
#endif

	//computes the number of overflows per second
	overflows = 1.0/((F_CPU/1024.0)/256.0);
	//overflows = 1000000000/((F_CPU/1024.0/256.0);

  unsigned long duration = F_CPU;
  secondsRapresented=1;
  while(duration%262144!=0){
    duration*=10;
    secondsRapresented*=10;//the number of seconds duration rapresent
  }
  overflows = duration/262144;
	//set the initial value of the counter depending on the prescaler choosen and
	//number of overflows per second
	starter = 0; // 256 - (int)((float)F_CPU * overflows / 1024);
}


//interrupt-driven routine to update the software clock
#if defined (ATMEGAx8) || defined (ATMEGA8) || defined (ATMEGA644) || defined (ATMEGAx0)
ISR(TIMER2_OVF_vect) {
	TCNT2 = starter;
#elif defined (__AVR_ATtiny2313__)
ISR(TIMER0_OVF_vect) {
	TCNT0 = starter;
#elif defined (ATTINYx4) || defined (ATTINYx5)
ISR (TIM0_OVF_vect) {
	TCNT0 = starter;
#endif
	counterT++;
	
	if (counterT>=overflows+delta){
	  timestamp+= secondsRapresented;
	  counterT-=overflows+delta;
	}
}


//start the software RTC
void swRTC::startRTC() {
	setTimer();
	counterT = 0;
#if defined (ATMEGAx8) || defined (ATMEGA644) || defined (ATMEGAx0)
	TCNT2 = starter;
	TIMSK2 |= (1<<TOIE2);
#elif defined (ATMEGA8)
	TCNT2 = starter;
	TIMSK |= (1<<TOIE2);
#elif defined (ATTINYx5) || defined (__AVR_ATtiny2313__)
	TCNT0 = starter;
	TIMSK |= (1<<TOIE0);
#elif defined (ATTINYx4)
	TCNT0 = starter;
	TIMSK0 |= (1<<TOIE0);
#endif
	SREG |= (1<<SREG_I);
	isRunning=true;
}


void swRTC::stopRTC() {
#if defined (ATMEGAx8) || defined (ATMEGA644) || defined (ATMEGAx0)
	TIMSK2 &= ~(1<<TOIE2);
#elif defined (ATMEGA8)
	TIMSK &= ~(1<<TOIE2);
#elif defined (ATTINYx5) || defined (__AVR_ATtiny2313__)
	TIMSK &= ~(1<<TOIE0);
#elif defined (ATTINYx4)
	TIMSK0 &= ~(1<<TOIE0);
#endif
	SREG &= ~(1<<SREG_I);
	isRunning=false;
}

//set deltaT to correct the deviation between computed & real time
//(given as seconds per day)
boolean swRTC::setDelta(unsigned long deltaT) {
  if ( deltaT<=overflows)
    return false;
  delta=deltaT;
  return true;
}

unsigned long swRTC::getTimestamp() {
/*  
  Serial.print( "hourDurationMS:" );
  Serial.print( hourDurationMS );
  Serial.print( " counterT:" );
  Serial.print( counterT );
  Serial.print( " map1:" );
  Serial.print( map( counterT, 0, hourDurationMS, 0, 3600 ) );
  Serial.print( " map2:" );
  Serial.print( map( counterT, 0, hourDurationMS, 0, 3600000 ) );
  Serial.print( " map3:" );
  Serial.println( map( counterT%1000, 0, hourDurationMS/1000, 0, 3600 ) );
*/ 
  return timestamp + map( counterT, 0, overflows+delta, 0, secondsRapresented );
}

int swRTC::updateDate(){
  unsigned long time = getTimestamp();
/*  
  Serial.print( "timestamp: " );
  Serial.println( time );
*/
  unsigned long day = time/(60L*60L*24L);
  float remaining = time-day*(60L*60L*24L);
/*  
  Serial.println( remaining );
  
  Serial.print( "day: " );
  Serial.println( day );
*/  
  int year = (day / 365.2422);
  float dayRemaining = day-year*365.2422;
  year+=1900;
/*  
  Serial.print( "year: " );
  Serial.println( year );
  Serial.println( dayRemaining );
*/ 
  if (dayRemaining>=365.2422)
    return 1;//my math is wrong!
  
  if (year<1900)
    return 2;//year not supported!
/*  
  Serial.print( "dayRemaining: " );
  Serial.println( dayRemaining );
*/
  int month=0;
  while (dayRemaining > daysPerMonth[month]){
    dayRemaining -= daysPerMonth[month];
//    Serial.print( "added month: " );
//    Serial.println( dayRemaining );
    month++;
  }
  
  month++;//because month 0 doesn't exist
/*  
  Serial.print( "month: " );
  Serial.println( month );
*/  
  if (month>12)
    return 3;//my math is wrong!
  
//  Serial.println( dayRemaining );
  
  if (dayRemaining>=(60L*60L*24L))
    return 4;//my math is wrong!
/*
  Serial.print( "dayRemaining: " );
  Serial.println( dayRemaining );
*/
  day=dayRemaining;
  if (dayRemaining-day>0){ //add partial day!
    day++;
  }
  
  int hours = remaining/(60L*60L);
  remaining = remaining-hours*(60L*60L);
/*  
  Serial.println( remaining );
  
  Serial.print( "hours: " );
  Serial.println( hours );
*/
  if (remaining>=(60L*60L))
    return 5;//my math is wrong!
  
  int minutes = remaining/(60L);
  remaining = remaining-minutes*(60L);
/*
  Serial.print( "minutes: " );
  Serial.println( minutes );
  
  Serial.print( "seconds: " );
  Serial.println( remaining );
*/  
  if (remaining>=60)
    return 6;//my math is wrong!
    
  now.year = year;
  now.month = month;
  now.day = day;
  now.hour = hours;
  now.minute = minutes;
  now.second = remaining;  
  return 0;
}

int swRTC::setDate(int year, byte month, byte day, byte hour, byte minute, byte second){

  if (isRunning){
    return 1;
  }
  
  if (year<1900){
    return 2;
  }
  unsigned long time;

  //One revolution of the Earth is not 365 days but accurately 365.2422days.It is leap year that adjusts this decimal fraction.   
  time = (year-1900)*365.2422; //find day from year
  
  for (int i=0; i < month-1; i++){
    time = time + daysPerMonth[i]; //find day from month
  }
  time = ( time + day ) * 24; //find hour from day
  time = ( time + hour ) * 60; //find minute from hours
  time = ( time + minute ) * 60; //find seconds from minute
  time += second; // add seconds
  
  counterT=0;
  timestamp=time;
  
  //startRTC();
  return 0;
}
