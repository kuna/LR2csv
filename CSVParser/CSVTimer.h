#pragma once

class CSVTimer {
public:
	static void invalidateTime();
	static long getTime(int num);
	static void setTime(int num);
	static void setTime(int num, int offset);
	static bool isTimerActiviated(int num);
	static void deActiviateTimer(int num);
	static long pureTime();

	static void setCallback(int num, int time, void (*callback)());
	static void cancelCallback(int num);
private:
	static long ntime;
	static long times[1000];
	static void (*callbacks[1000])();
	static int callbackTime[1000];
};