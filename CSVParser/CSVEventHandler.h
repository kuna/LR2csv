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

	// variables
	CSVData *csvData;

	// fadeoutStartTime
	int fadeoutStartTime;
public:
	void setOnSceneStart(void (*func)());
	void setOnInputStart(void (*func)());
	void setOnFadeInStart(void (*func)());
	void setOnFadeOutStart(void (*func)());
	void setOnFadeInEnd(void (*func)());
	void setOnFadeOutEnd(void (*func)());
	void setOnSceneEnd(void (*func)());

	double getSceneAlpha();

public:
	// should be called every time (rendering)!
	void checkEvent();
	void setCSVData(CSVData *csvData);
	void endScene();
};


// TODO: add load_start time?
// TODO: add load_end callback?
// TODO: add ready callback?