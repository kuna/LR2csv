#include "CSVData.h"

class CSVEventHandler {
private:
	// handlers
	void (*scenestart_func)();
	void (*inputstart_func)();
	void (*fadeinstart_func)();
	void (*fadeoutstart_func)();
	void (*fadeinend_func)();
	void (*fadeoutend_func)();
	void (*sceneend_func)();
	void (*loadstart_func)();
	void (*loadend_func)();
	void (*playstart_func)();

	bool scenestart_exec;
	bool inputstart_exec;
	bool fadeinstart_exec;
	bool fadeoutstart_exec;
	bool fadeinend_exec;
	bool fadeoutend_exec;
	bool sceneend_exec;
	bool loadstart_exec;
	bool loadend_exec;
	bool playstart_exec;

	// variables
	CSVData *csvData;

	// fadeoutStartTime
	int fadeoutStartTime;
	int loadEndTime;
	bool isloadComplete;
public:
	void setOnSceneStart(void (*func)());
	void setOnInputStart(void (*func)());
	void setOnFadeInStart(void (*func)());
	void setOnFadeOutStart(void (*func)());
	void setOnFadeInEnd(void (*func)());
	void setOnFadeOutEnd(void (*func)());
	void setOnSceneEnd(void (*func)());
	void setOnLoadStart(void (*func)());
	void setOnLoadEnd(void (*func)());	// same as ready
	void setOnPlayStart(void (*func)());

	double getSceneAlpha();
	void setSceneEndTime(int newTime);
	void loadingComplete();
public:
	// should be called every time (rendering)!
	void checkEvent();
	void setCSVData(CSVData *csvData);
	void endScene();	// not directly ... same as 
};


// TODO: add select moveup/down?
// TODO: add select song?
// TODO: add oncancel/skipscene?
// -> add it to gamemanager. it's to specific...?