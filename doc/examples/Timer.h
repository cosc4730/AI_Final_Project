//
// Jack Murdock
//

#ifndef __TIMER_H_
#define __TIMER_H_

#include <iostream>
#include <ctime>

using namespace std;

class Timer {
private:
	clock_t begin;
	clock_t end;
public:
	void start(){
		begin = clock();
	}

	// Time in Secs
	double stop(){
		end = clock();
		double time_secs = double(end - begin) / CLOCKS_PER_SEC;
		return time_secs;
	}
};

#endif /* __TIMER_H_ */
