/*
  swRTC.h - An RTC implemented in software
  using the timer2 from Arduino
  Leonardo Miliani <leonardo AT leonardomiliani DOT com>
  with the help of lesto <www.arduino.cc>
  
  
  Based on MsTimer2 by Javier Valencia
  
  History:
  v. 0.1: first release
	
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

#include "WProgram.h"
#include "swRTC.h"
#include <avr/interrupt.h>

swRTC *lib;
volatile byte starter;
volatile int counter;

swRTC::swRTC(void) {
	isRunning=false;
	counter=0;
	hours=0;
	minutes=0;
	seconds=0;
	day=0;
	month=0;
	year=0;
	daysPerMonth[0] = 31;
	daysPerMonth[1] = 28;
	daysPerMonth[2] = 31;
	daysPerMonth[3] = 30;
	daysPerMonth[4] = 31;
	daysPerMonth[5] = 30;
	daysPerMonth[6] = 31;
	daysPerMonth[7] = 31;
	daysPerMonth[8] = 30;
	daysPerMonth[9] = 31;
	daysPerMonth[10] = 30;
	daysPerMonth[11] = 31;
	starter=0;

        lib=this;
}


//sets the RTC to an initial value
boolean swRTC::setRTC(byte hourT, byte minuteT, byte secondT, byte dayT, byte monthT, int yearT) {
	
	if (!isRunning) { return false; }
	
	//check if the params are correct
	if ((hourT < 0) || (hourT > 23)) { return false; }
	if ((minuteT < 0) || (minuteT > 59)) { return false; }
	if ((secondT < 0) || (secondT > 59)) { return false; }
	if ((dayT < 1) || (dayT > 31)) { return false; }
	if ((monthT < 1) || (monthT > 12)) { return false; }
	if (yearT < 0)  { return false; }
	
	hours=hourT;
	minutes=minuteT;
	seconds=secondT;
	day=dayT;
	month=monthT;
	year=yearT;
	setTimer(); //set the timer2
	startRTC(); //starts the RTC
	return true;
}


void swRTC::setTimer() {
	float prescaler = 0.0;
	
    //during setup, disable all the interrupts based on timer2
    TIMSK2 &= ~(1<<TOIE2); 
    TIMSK2 &= ~((1<<OCIE2A) | (1<<OCIE2B));
    //prescaler source clock set to internal Atmega clock
    ASSR &= ~(1<<AS2);
    //this sets the timer to increment the counter until overflow
    TCCR2A &= ~((1<<WGM21) | (1<<WGM20)); 
    TCCR2B &= ~(1<<WGM22);
    
    //the following code sets the prescaler depending on the system clock
	if (F_CPU == 16000000UL) {	// prescaler set to 64
		TCCR2B |= (1<<CS22);
		TCCR2B &= ~((1<<CS21) | (1<<CS20));
		prescaler = 64.0;
	} else if (F_CPU == 8000000UL) { // prescaler set to 32
		TCCR2B &= ~(1<<CS22); 
		TCCR2B |= ((1<<CS21) | (1<<CS20)); 
		prescaler = 32.0;
	} else if (F_CPU == 1000000UL) { // prescaler set to 8
		TCCR2B &= ~((1<<CS22) | (1<<CS20));
		TCCR2B |= (1<<CS21);
		prescaler = 8.0;
	}
	
	//set the initial value of the counter depending on the prescaler
	starter = 256 - (int)((float)F_CPU * 0.001 / prescaler);
}


void swRTC::startRTC() {
	counter = 0;
	TCNT2 = starter;
	TIMSK2 |= (1<<TOIE2);
	isRunning=true;
}


void swRTC::stopRTC() {
	TIMSK2 &= ~(1<<TOIE2);
	isRunning=false;
}


ISR(TIMER2_OVF_vect) {
	TCNT2 = starter;
	counter++;
    if (counter>999) { //1000 ms equals to 1 s
        counter=0;
        (*lib).updateTime();
    }
}


void swRTC::updateTime() {
	byte dayT;
	seconds++;
	if (seconds>59) {
		seconds=0;
		minutes++;
		if (minutes>59) {
			minutes=0;
			hours++;
			if (hours>23) {
				hours=0;
				day++;
				if (month == 2) { //february?
					if (isLeapYear()) {
						dayT=29;
					} else {
						dayT=28;
					}
				} else {
					dayT=daysPerMonth[month-1];
				}
				if (day>dayT) {
					day=1;
					month++;
					if (month>12) {
						month=1;
						year++;
					}
				}
			}
		}
	}
}


byte swRTC::getSeconds() {
	return seconds;
}


byte swRTC::getMinutes() {
	return minutes;
}


byte swRTC::getHours() {
	return hours;
}


byte swRTC::getDay() {
	return day;
}


byte swRTC::getMonth() {
	return month;
}


int swRTC::getYear() {
	return year;
}


boolean swRTC::isLeapYear() {
	if (((year % 4) == 0) && ((year % 100) != 0) or ((year % 400) == 0)) {
    return true;
  } else {
    return false;
  }
}


