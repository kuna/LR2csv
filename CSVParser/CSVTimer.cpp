//#include <ctime> - time wont work
#include "Stdafx.h"
#include "CSVTimer.h"

long CSVTimer::ntime = 0;
long CSVTimer::times[1000];
void (*CSVTimer::callbacks[1000])();
int CSVTimer::callbackTime[1000];

void CSVTimer::invalidateTime() {
	// this must be called when program starts!
	ntime = GetTickCount();

	// and check is there any callback functions ...
	for (int i=0; i<1000; i++) {
		if (callbackTime[i]) {
			if (getTime(i) > callbackTime[i]) {
				callbackTime[i] = 0;	// MUST clear time first!!
				callbacks[i]();
			}
		}
	}
}

void CSVTimer::setTime(int num) {
	times[num] = ntime;
}

void CSVTimer::setTime(int num, int offset) {
	times[num] = ntime + offset;
}

long CSVTimer::getTime(int num) {
	// when not initalized ...
	if (times[num] == 0)
		return 0;

	int r = ntime-times[num];
	if (r < 0)
		r = 0;
	return r;
}

bool CSVTimer::isTimerActiviated(int num) {
	return (times[num]);
}

void CSVTimer::deActiviateTimer(int num) {
	times[num] = 0;
}

long CSVTimer::pureTime() {
	return ntime;
}

void CSVTimer::setCallback(int num, int time, void (*callback)()) {
	callbackTime[num] = time;
	callbacks[num] = callback;
}

void CSVTimer::cancelCallback(int num) {
	callbackTime[num] = 0;
	callbacks[num] = 0;
}