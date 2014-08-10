#pragma once
#include "CSVElement.h"
#include "CSVdrawarg.h"
#include "CSVFont.h"
#include "CSVSettings.h"
#include "Stdafx.h"

struct CSVTransColor {
	BYTE r, g, b;
};

class CSVData {
public:
	~CSVData();
	std::vector<CSVElement*> csvElement;
	std::vector<std::wstring> images;
	std::vector<CSVFont*> csvFont;
	CSVElement *csvBar;				// for select screen
	int barCenter;					// basic is 0
	CSVTransColor colorKey;			// basic is 0
	int sceneTime;
	int loadStartTime;
	int loadEndTime;
	int playStartTime;
	int fadeOutTime;
	int skipTime;

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

	// sound
	std::wstring sound_select;
	std::wstring sound_decide;
	std::wstring sound_exselect;
	std::wstring sound_exdecide;
	std::wstring sound_folder_open;
	std::wstring sound_folder_close;
	std::wstring sound_panel_open;
	std::wstring sound_panel_close;
	std::wstring sound_option_change;
	std::wstring sound_difficulty;
	std::wstring sound_screenshot;
	std::wstring sound_clear;
	std::wstring sound_fail;
	std::wstring sound_stop;
	std::wstring sound_mine;
	std::wstring sound_scratch;
	std::wstring sound_courseclear;
	std::wstring sound_coursefail;
private:
	// metadatas

	// some etcs
	std::vector<CSVElement*> csvButtons;		// button (onmouse?)

public:
	void addcsvBarSRC(CSVElement *ele);

	void Clear();

	TCHAR path[256];
	// TODO ...?
	//std::vector<CSVElement*> getElementsByTag(TCHAR *tag);
};