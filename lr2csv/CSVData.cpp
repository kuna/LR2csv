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

// Depreciated method. use CSVRenderer::drawAll and CSVRenderer::SetdrawFunc
void CSVData::drawAll(bool (*drawFunc)(int, TCHAR*, CSVSRC*, CSVDST*)) {
	CSVRenderer::SetdrawFunc(drawFunc);
	for (int i=0; i<csvElement.size(); i++) {
		CSVRenderer::drawElement(csvElement[i]);
	}
}

void CSVData::Clear() {
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
}