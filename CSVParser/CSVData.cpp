#include "Stdafx.h"
#include "CSVData.h"
#include "CSVRenderer.h"

CSVData::~CSVData() {
	// free up all elements
	std::vector<CSVElement*>::iterator i;
	for (i = csvElement.begin(); i != csvElement.end(); i++) {
		free(*i);
		//csvElement.erase(i);
	}
	csvElement.clear();
}

void CSVData::Clear() {
	/*
	 * memory clear
	 */
	csvBar = 0;

	// clear elements
	for (int i=0; i<csvElement.size(); i++) {
		delete csvElement[i];
	}
	csvElement.clear();

	// clear option
	for (int i=0; i<csvOptions.size(); i++) {
		delete csvOptions[i];
	}
	csvOptions.clear();

	// clear CSVFont
	for (int i=0; i<csvFont.size(); i++) {
		delete csvFont[i];
	}
	csvFont.clear();

	// clear images
	for (int i=0; i<images.size(); i++) {
	}
	images.clear();

	csvBarTitle.clear();

	/*
	 * options clear
	 */
	
	barCenter = 0;
	colorKey.r = colorKey.g = colorKey.b = 0;
	loadStartTime = 0;
	loadEndTime=0;
	playStartTime=0;
	sceneTime=0;
	fadeInTime=0;
	fadeOutTime=0;
	inputStartTime=0;
	skipTime=0;
}