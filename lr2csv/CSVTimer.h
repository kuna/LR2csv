class CSVTimer {
public:
	static void invalidateTime();
	static long getTime(int num);
	static void setTime(int num);
	static void setTime(int num, int offset);
	static bool isTimerActiviated(int num);
	static void deActiviateTimer(int num);
	static long pureTime();
private:
	static long ntime;
	static long times[1000];
};