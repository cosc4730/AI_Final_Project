//////////////////////////////////////////////////////////////////////////////
// Timer.h  UNIX SYSTEM VERSION
// =======
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////
// MODIFIED: by tom bailey  1000  19 nov 2012
// change of method names
// change to bool for running flag
// change timeInSeconds from data member to local variable
//
// MODIFIED: by tom bailey   1500  28 feb 2013
// separate the Windows and Unix versions
//////////////////////////////////////////////////////////////////////////////

#ifndef __TIMER_H_
#define __TIMER_H_

#include <iostream>
#include <sys/time.h>

using namespace std;

class Timer {
public:
	Timer(){ // default constructor
		running = false;
		elapsedTimeInSec = 0.0;
	}                                   
	~Timer(){}                                   // default destructor
	void reset(){
		running = false;
		elapsedTimeInSec = 0.0;
	}
	void   start(){                             // start the timer
		if( ! running )
		{
		  timeval startCount;                         //
		  gettimeofday(&startCount, NULL);
		  startTimeInSec = double( startCount.tv_sec ) + 0.000001 * double( startCount.tv_usec );
		  running = true; 
		}
	}
	void   stop(){                              // stop the timer
		   if( running )
		   {
			  timeval endCount;                           //
			  gettimeofday(&endCount, NULL);
			  double endTimeInSec = double( endCount.tv_sec ) + 0.000001 * double( endCount.tv_usec );
			  elapsedTimeInSec += endTimeInSec - startTimeInSec;
			  running = false; 
		   }
	}
	double operator()(){                        // get elapsed time in seconds
		   if( running )
		   {
			  stop();
			  start();
		   }    
		   return elapsedTimeInSec;  
	}

private:
	bool   running;                             // running flag
	double startTimeInSec;                      // start time for current interval
	double elapsedTimeInSec;                    // sum of all intervals on this Timer
};

#endif /* __TIMER_H_ */
