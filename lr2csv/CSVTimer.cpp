//#include <ctime> - time wont work
#include <Windows.h>
#include "CSVTimer.h"

long CSVTimer::ntime = 0;
long CSVTimer::times[1000];

void CSVTimer::invalidateTime() {
	// this must be called when program starts!
	ntime = GetTickCount();
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