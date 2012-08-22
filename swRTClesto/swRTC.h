#ifndef swRTC_H
#define swRTC_H

#include "WProgram.h"

//check if the micro is supported
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#define ATMEGAx8
#elif defined (__AVR_ATtiny25__) || defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__)
#define ATTINYx5
#elif defined (__AVR_ATmega8__) || defined (__AVR_ATmega8A__)
#define ATMEGA8
#elif defined (__AVR_ATtiny24__) || defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)
#define ATTINYx4
#elif defined (__AVR_ATmega640__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega1281__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega2561__)
#define ATMEGAx0 
#elif defined (__AVR_ATmega644__) || defined (__AVR_ATmega644P__)
#define ATMEGA644
#else
#error Microcontroller not supported
#endif

//check if the frequency is supported
#if ((F_CPU!=1000000UL) && (F_CPU!=8000000UL) && (F_CPU!=16000000UL))
#error Clock frequency not supported
#endif

class swRTC {
	public: 
                //public struct
                struct data{
                  unsigned int year;
                  unsigned int month;
                  unsigned int day;
                  unsigned int hour;
                  unsigned int minute;
                  unsigned int second;
                }now;
		//public methods
		swRTC();
		void startRTC();
		void stopRTC();
		boolean setDelta(unsigned long);
		unsigned long getTimestamp();
                int updateDate();
                int setDate(int year, byte month, byte day, byte hour, byte minute, byte second);
	private:
		//private methods
		void setTimer();
		//private variables
		boolean isRunning;
};

#endif
