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

void CSVData::drawAll(bool (*drawFunc)(int, TCHAR*, CSVSRC*, CSVDST*)) {
	CSVRenderer::SetdrawFunc(drawFunc);
	for (int i=0; i<csvElement.size(); i++) {
		CSVRenderer::drawElement(csvElement[i]);
	}
}

void CSVData::Clear() {
	csvBar = 0;

	for (int i=0; i<csvElement.size(); i++) {
		free(csvElement[i]);
	}
	csvElement.clear();

	for (int i=0; i<csvOptions.size(); i++) {
		free(csvOptions[i]);
	}
	csvOptions.clear();

	// TODO: DSTOff/DSTOn should be cleared
	csvBarTitle.clear();
}