#include "StdAfx.h"
#include "CSVEventHandler.h"
#include "CSVTimer.h"
#include "CSVConst.h"

void CSVEventHandler::setOnSceneStart(void (*func)()) {
	scenestart_func = func;
}

void CSVEventHandler::setOnInputStart(void (*func)()) {
	inputstart_func = func;
}

void CSVEventHandler::setOnFadeInStart(void (*func)()) {
	fadeinstart_func = func;
}

void CSVEventHandler::setOnFadeOutStart(void (*func)()) {
	fadeoutstart_func = func;
}

void CSVEventHandler::setOnFadeInEnd(void (*func)()) {
	fadeinend_func = func;
}

void CSVEventHandler::setOnFadeOutEnd(void (*func)()) {
	fadeoutend_func = func;
}

void CSVEventHandler::setOnSceneEnd(void (*func)()) {
	sceneend_func = func;
}

double CSVEventHandler::getSceneAlpha() {
	int sceneTime = CSVTimer::getTime(CSVTimerConst::MAIN);

	// if on fadein?
	if (csvData->fadeInTime > sceneTime) {
		return (1 - (double)sceneTime / csvData->fadeInTime);
	}

	// if on fadeout?
	else if (fadeoutStartTime && sceneTime > fadeoutStartTime) {
		double a = (double)(sceneTime - fadeoutStartTime) / csvData->fadeOutTime;
		if (a > 1) return 1;
		else return a;
	}

	// else, then 0
	else {
		return 0;
	}
}

void CSVEventHandler::checkEvent() {
	if (CSVTimer::isTimerActiviated(CSVTimerConst::MAIN)) {
#define SAFE_EXECUTE(func) if ((func) != 0) { (func)(); (func) = 0; }
		// onscenestart
		SAFE_EXECUTE(scenestart_func);

		// onfadeinstart
		SAFE_EXECUTE(fadeinstart_func);
		
		int sceneTime = CSVTimer::getTime(CSVTimerConst::MAIN);

		// oninputstart
		if (csvData->inputStartTime <= sceneTime)
			SAFE_EXECUTE(inputstart_func);

		// onfadein end
		if (sceneTime >= csvData->fadeInTime)
			SAFE_EXECUTE(fadeinend_func);

		// onfadeout start
		if (fadeoutStartTime && fadeoutStartTime <= sceneTime)
			SAFE_EXECUTE(fadeoutstart_func);

		// onfadeout end
		if (fadeoutStartTime+csvData->fadeOutTime >= sceneTime)
			SAFE_EXECUTE(fadeoutend_func);

		// scene end
		if (fadeoutStartTime+csvData->fadeOutTime >= sceneTime)
			SAFE_EXECUTE(fadeoutend_func);
#undef SAFE_EXECUTE
	}
}

void CSVEventHandler::setCSVData(CSVData *csvData) {
	this->csvData = csvData;

	// reset fadeout, or set scene time.
	fadeoutStartTime = csvData->sceneTime;
}

void CSVEventHandler::endScene() {
	// set onFadeOut
	fadeoutStartTime = CSVTimer::getTime(CSVTimerConst::MAIN);
}