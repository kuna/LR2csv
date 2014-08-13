#include "StdAfx.h"
#include "CSVEventHandler.h"
#include "CSVTimer.h"
#include "CSVConst.h"

void CSVEventHandler::setOnSceneStart(void (*func)()) {
	scenestart_func = func;
}

// in this function, generally set input decice.
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

// in this function, generally prepare next scene.
void CSVEventHandler::setOnSceneEnd(void (*func)()) {
	sceneend_func = func;
}

// prepare loading BMS.
// must call loadingComplete() after loading finished.
void CSVEventHandler::setOnLoadStart(void (*func)()) {
	loadstart_func = func;
}

// when loading end (same as ready timing)
void CSVEventHandler::setOnLoadEnd(void (*func)()) {
	loadend_func = func;
}

// when play starts.
void CSVEventHandler::setOnPlayStart(void (*func)()) {
	playstart_func = func;
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

void CSVEventHandler::setSceneEndTime(int newTime) {
	fadeoutStartTime = newTime;
}

void CSVEventHandler::loadingComplete() {
	isloadComplete = true;
}

void CSVEventHandler::checkEvent() {
	if (CSVTimer::isTimerActiviated(CSVTimerConst::MAIN)) {
#define SAFE_EXECUTE(func, chk) if (!(chk) && (func)) { (chk) = true; (func)(); }
		// onscenestart
		SAFE_EXECUTE(scenestart_func, scenestart_exec);

		// onfadeinstart
		SAFE_EXECUTE(fadeinstart_func, fadeinstart_exec);
		
		int sceneTime = CSVTimer::getTime(CSVTimerConst::MAIN);

		// oninputstart
		if (csvData->inputStartTime <= sceneTime)
			SAFE_EXECUTE(inputstart_func, inputstart_exec);

		// onfadein end
		if (sceneTime >= csvData->fadeInTime)
			SAFE_EXECUTE(fadeinend_func, fadeinend_exec);

		// onfadeout start
		if (fadeoutStartTime && fadeoutStartTime <= sceneTime)
			SAFE_EXECUTE(fadeoutstart_func, fadeoutstart_exec);

		// onfadeout end
		if (fadeoutStartTime && fadeoutStartTime+csvData->fadeOutTime <= sceneTime)
			SAFE_EXECUTE(fadeoutend_func, fadeoutend_exec);

		// scene end
		if (fadeoutStartTime && fadeoutStartTime+csvData->fadeOutTime <= sceneTime)
			SAFE_EXECUTE(sceneend_func, sceneend_exec);

		// onloadstart
		if (csvData->loadStartTime && csvData->loadStartTime <= sceneTime)
			SAFE_EXECUTE(loadstart_func, loadstart_exec);
		
		// onloadend
		if (isloadComplete && csvData->loadStartTime+csvData->loadEndTime <= sceneTime) {
			if (!loadend_exec && loadend_func) {
				// set loadend time
				loadEndTime = sceneTime;

				loadend_func();
				loadend_exec = true;
			}
		}

		// onplaystart (occurs after onloadend)
		if (loadEndTime && loadEndTime+csvData->playStartTime <= sceneTime) {
			SAFE_EXECUTE(playstart_func, playstart_exec);
		}
#undef SAFE_EXECUTE
	}
}

void CSVEventHandler::setCSVData(CSVData *csvData) {
	this->csvData = csvData;

	// reset
	scenestart_exec = false;
	inputstart_exec = false;
	fadeinstart_exec = false;
	fadeoutstart_exec = false;
	fadeinend_exec = false;
	fadeoutend_exec = false;
	sceneend_exec = false;
	loadstart_exec = false;
	loadend_exec = false;
	playstart_exec = false;

	// reset fadeout, or set scene time.
	fadeoutStartTime = csvData->sceneTime;
	loadEndTime = 0;
	isloadComplete = false;
}

void CSVEventHandler::endScene() {
	// set onFadeOut
	fadeoutStartTime = CSVTimer::getTime(CSVTimerConst::MAIN);
}