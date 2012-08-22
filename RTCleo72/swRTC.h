#ifndef swRTC_H
#define swRTC_H

#include "WProgram.h"

class swRTC {
	public: 
		swRTC();
		boolean setRTC(byte hour, byte minute, byte second, byte day, byte month, int year);
		void startRTC();
		void stopRTC();
		byte getSeconds();
		byte getMinutes();
		byte getHours();
		byte getDay();
		byte getMonth();
		int getYear();
		boolean isLeapYear();
		
                void updateTime();
	private:
		void setTimer();
		

		boolean isRunning;
		byte hours;
		byte minutes;
		byte seconds;
		byte day;
		byte month;
		int year;
		
		byte daysPerMonth[12];
		
};

#endif
