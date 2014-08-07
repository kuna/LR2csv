#include <vector>
#include <string>
#pragma once
#include "CSVElement.h"
#include "CSVdrawarg.h"
#include "CSVFont.h"
#include "CSVSettings.h"

class CSVData {
public:
	~CSVData();
	std::vector<CSVElement*> csvElement;
	std::vector<std::wstring> images;
	std::vector<CSVFont*> csvFont;
	CSVElement *csvBar;				// for select screen
	int barCenter;					// basic is 0

	/* under - not for CSV data, but for rendering */
	// TODO: refactoring it to CSVRenderer...?
	// bar drawing part
	CSVElement* csvBarLevel;		// level
	CSVElement* csvFlash;			// flash
	std::vector<CSVElement*> csvBarTitle;

	// customoption part
	std::vector<CSVCustomOption*> csvOptions;

	// nowcombo part (used with nowjudge)
	CSVElement* csvNowCombo1P;
	CSVElement* csvNowCombo2P;

	// note / LN note part
	CSVElement *csvNote;
	CSVElement *csvLNStart;
	CSVElement *csvLNBody;
	CSVElement *csvLNEnd;
	CSVElement *csvMine;
	CSVElement *csvLine;
private:
	// metadatas

	// some etcs
	std::vector<CSVElement*> csvButtons;		// button (onmouse?)

public:
	void addcsvBarSRC(CSVElement *ele);

	// index, src, dst, argb, rotation, filter, blending
	void drawAll(bool (*drawFunc)(int, TCHAR*, CSVSRC*, CSVDST*));

	void Clear();

	TCHAR path[256];
	// TODO ...?
	//std::vector<CSVElement*> getElementsByTag(TCHAR *tag);
};