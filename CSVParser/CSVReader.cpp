#include "Stdafx.h"
#include "CSVReader.h"
#include "CSVOption.h"
#include "CSVFile.h"
#include "CSVButton.h"
#include "CSVSlider.h"

// statics
int CSVReader::CSVTYPE_IMAGE = CSVElement::getTypeInt(L"IMAGE");
int CSVReader::CSVTYPE_BUTTON = CSVElement::getTypeInt(L"BUTTON");
int CSVReader::CSVTYPE_ONMOUSE = CSVElement::getTypeInt(L"ONMOUSE");
int CSVReader::CSVTYPE_TEXT = CSVElement::getTypeInt(L"TEXT");
int CSVReader::CSVTYPE_BARGRAPH = CSVElement::getTypeInt(L"BARGRAPH");
int CSVReader::CSVTYPE_GROOVEGUAGE = CSVElement::getTypeInt(L"GROOVEGUAGE");
int CSVReader::CSVTYPE_BGA = CSVElement::getTypeInt(L"BGA");
int CSVReader::CSVTYPE_JUDGELINE = CSVElement::getTypeInt(L"JUDGELINE");
int CSVReader::CSVTYPE_SLIDER = CSVElement::getTypeInt(L"SLIDER");
int CSVReader::CSVTYPE_LINE = CSVElement::getTypeInt(L"LINE");
int CSVReader::CSVTYPE_NOTE = CSVElement::getTypeInt(L"NOTE");
int CSVReader::CSVTYPE_LNSTART = CSVElement::getTypeInt(L"LN_START");
int CSVReader::CSVTYPE_LNEND = CSVElement::getTypeInt(L"LN_END");
int CSVReader::CSVTYPE_LNBODY = CSVElement::getTypeInt(L"LN_BODY");
int CSVReader::CSVTYPE_MINE = CSVElement::getTypeInt(L"MINE");
int CSVReader::CSVTYPE_NUMBER = CSVElement::getTypeInt(L"NUMBER");
int CSVReader::CSVTYPE_NOWJUDGE_1P = CSVElement::getTypeInt(L"NOWJUDGE_1P");
int CSVReader::CSVTYPE_NOWCOMBO_1P = CSVElement::getTypeInt(L"NOWCOMBO_1P");
int CSVReader::CSVTYPE_NOWJUDGE_2P = CSVElement::getTypeInt(L"NOWJUDGE_2P");
int CSVReader::CSVTYPE_NOWCOMBO_2P = CSVElement::getTypeInt(L"NOWCOMBO_2P");
	
int CSVReader::CSVTYPE_BAR_BODY = CSVElement::getTypeInt(L"BAR_BODY");			// SRC
int CSVReader::CSVTYPE_BAR_BODY_OFF = CSVElement::getTypeInt(L"BAR_BODY_OFF");	// DST
int CSVReader::CSVTYPE_BAR_BODY_ON = CSVElement::getTypeInt(L"BAR_BODY_ON");	// DST
int CSVReader::CSVTYPE_BAR_CENTER = CSVElement::getTypeInt(L"BAR_BODY_CENTER");
int CSVReader::CSVTYPE_BAR_AVAILABLE = CSVElement::getTypeInt(L"BAR_AVAILABLE");
int CSVReader::CSVTYPE_BAR_FLASH = CSVElement::getTypeInt(L"BAR_FLASH");
int CSVReader::CSVTYPE_BAR_LEVEL = CSVElement::getTypeInt(L"BAR_LEVEL");
int CSVReader::CSVTYPE_BAR_LAMP = CSVElement::getTypeInt(L"BAR_LAMP");
int CSVReader::CSVTYPE_BAR_MY_LAMP = CSVElement::getTypeInt(L"BAR_MY_LAMP");
int CSVReader::CSVTYPE_BAR_RIVAL_LAMP = CSVElement::getTypeInt(L"BAR_RIVAL_LAMP");
int CSVReader::CSVTYPE_BAR_TITLE = CSVElement::getTypeInt(L"BAR_TITLE");
int CSVReader::CSVTYPE_BAR_RANK = CSVElement::getTypeInt(L"BAR_RANK");
int CSVReader::CSVTYPE_BAR_RIVAL = CSVElement::getTypeInt(L"BAR_RIVAL");

CSVElement *CSVReader::prevBtn;	// for BUTTON~ONMOUSE event
CSVData *CSVReader::currentCSV;
int CSVReader::condition[256];		// used #IF~#ELSEIF~#ELSE~#ENDIF. true: 1, falsed: 0, used: 2
int CSVReader::conditionDepth;
std::vector<TCHAR*> CSVReader::lines;
std::vector<TCHAR*> CSVReader::recycled;	// used after parsing all datas (release Data resource)
CSVElement* CSVReader::prev_element[CACHESIZE];

bool CSVReader::readCSVFile(TCHAR *path, CSVData *csvData) {
	// init
	lines.clear();
	for (int i=0; i<CACHESIZE; i++) {
		prev_element[i] = 0;
	}

	// convert path
	TCHAR absolutePath[256];
	CSVFile::GetPathFromSettings(path, absolutePath);

	// read file
	TCHAR *fileData;
	std::wstring fileData_wstr;
	if (!AutoEncoder::readfileEncodingAuto(absolutePath, fileData_wstr))
		return false;
	fileData = (TCHAR*)calloc(fileData_wstr.length()+10, sizeof(TCHAR));
	wcscpy(fileData, fileData_wstr.c_str());

	wcscpy(csvData->path, absolutePath);

	// split lines & initalize
	conditionDepth = 0;
	currentCSV = csvData;
	if (!parseCSVData(fileData)) {
		free(fileData);
		return false;
	}
	recycled.push_back(fileData);

	// parse
	processCSVLines();

	// finish
	for (int i=0; i<recycled.size(); i++)
		free(recycled[i]);
	recycled.clear();
	lines.clear();

	// automatic init
	CSVButton::SetCurrentCSV(csvData);
	CSVSlider::setCSVData(csvData);

	return true;
}

bool CSVReader::parseCSVData(TCHAR *data) {
	
	// line parse
	TCHAR *n = NULL;
	n = wcstok(data, L"\n");
	while (n != NULL)
	{
		if (n[wcslen(n)-1] == L'\r')
			n[wcslen(n)-1] = 0;
		
		lines.push_back(n);

		n = wcstok(NULL, L"\n");
	}

	return true;
}

void CSVReader::processCSVLines() {
	for (int i=0; i<lines.size(); i++) {	
		processCSVLine(lines[i]);
	}
}

void CSVReader::processCSVLine(TCHAR *data) {
	if (memcmp(data, L"//", 2*sizeof(TCHAR)) == 0) return;	// comment: do nothing
	if (data[0] != L'#') return; // wrong format: do nothing

	// parse All argument
	// max argument: 20
	int idx = 0;
	TCHAR *args[20];
	memset(args, 0, sizeof(args));
	
	TCHAR *n;
	n = wcstok(data, L",");
	while (n != NULL && idx < 20) {
		args[idx] = n;
		idx++;

		n = wcstok(NULL, L",");
	}

	// check argument
	if (wcscmp(args[0], L"#IF")==0) {
		// check condition first
		condition[conditionDepth++] = checkCondition(_wtoi(args[1]));
		return;
	} else if (wcscmp(args[0], L"#ELSE") == 0) {
		if (conditionDepth > 0) {
			if (condition[conditionDepth-1] == 0) {
				condition[conditionDepth-1] = 1;
			} else {
				condition[conditionDepth-1] = 2;
			}
		} else {
			// error: no condition found
		}
	} else if (wcscmp(args[0], L"#ELSEIF") == 0) {
		if (conditionDepth > 0) {
			if (condition[conditionDepth-1] == 0) {
				int c = checkCondition(_wtoi(args[1]));
				condition[conditionDepth-1] = c;
			} else {
				condition[conditionDepth-1] = 2;
			}
		} else {
			// error: no condition found
		}
	} else if (wcscmp(args[0], L"#ENDIF") == 0) {
		if (conditionDepth <= 0) {
			// error: no condition found
		} else {
			conditionDepth --;
		}
	}

	// check argument (when condition true)
	if ((conditionDepth > 0 && condition[conditionDepth-1] == 1) ||
		conditionDepth == 0) {
		if (conditionDepth > 0) {
			OutputDebugString(L"PASS - ");
			OutputDebugString(args[0]);
			OutputDebugString(L"\n");
		}

		if (wcsncmp(args[0], L"#SRC_", 5) == 0) {
			int mode = CSVElement::getTypeInt(args[0]+5);
			CSVElement *csvElement;
			
			// if type is IMG/ONMOUSE/BUTTON
			// then set index 0 (NONE)
			if (mode == CSVReader::CSVTYPE_IMAGE ||
				mode == CSVReader::CSVTYPE_ONMOUSE ||
				mode == CSVReader::CSVTYPE_JUDGELINE ||
				mode == CSVReader::CSVTYPE_SLIDER ||
				mode == CSVReader::CSVTYPE_BUTTON)
				wcscpy(args[1], L"0");

			// if same index comes out or prev_element is null, then put new one
			// else, then insert it to previous one
			int src_index = _wtoi(args[1]);
			if (!prev_element[mode%CACHESIZE] || prev_element[mode%CACHESIZE]->src[src_index]) {
				csvElement = (CSVElement*)malloc(sizeof(CSVElement));
				memset(csvElement, 0, sizeof(CSVElement));
				// -> this will cause dstNum Zero. don't do it.
				csvElement->parent = currentCSV;
				csvElement->setType(args[0]);
				csvElement->addSRC(args);
				currentCSV->csvElement.push_back(csvElement);
			} else {
				csvElement = prev_element[mode%CACHESIZE];
				csvElement->addSRC(args);
			}

			// set previous element
			prev_element[mode%CACHESIZE] = csvElement;

			// refactoring:
			// search or caching function ... ?
			if (mode == CSVTYPE_BAR_LEVEL) {
				currentCSV->csvBarLevel = csvElement;
			} else if (mode == CSVTYPE_BAR_FLASH) {
				currentCSV->csvFlash = csvElement;
			} else if (mode == CSVTYPE_BAR_TITLE) {
				currentCSV->csvBarTitle.push_back(csvElement);
			} else if (mode == CSVTYPE_NOWCOMBO_1P) {
				currentCSV->csvNowCombo1P = csvElement;
			} else if (mode == CSVTYPE_NOWCOMBO_2P) {
				currentCSV->csvNowCombo2P = csvElement;
			} else if (mode == CSVTYPE_NOTE) {
				currentCSV->csvNote = csvElement;
			} else if (mode == CSVTYPE_LNSTART) {
				currentCSV->csvLNStart = csvElement;
			} else if (mode == CSVTYPE_LNEND) {
				currentCSV->csvLNEnd = csvElement;
			} else if (mode == CSVTYPE_LNBODY) {
				currentCSV->csvLNBody = csvElement;
			} else if (mode == CSVTYPE_MINE) {
				currentCSV->csvMine = csvElement;
			} else if (mode == CSVTYPE_LINE) {
				currentCSV->csvLine = csvElement;
			}

			// if ONMOUSE, then relate to previous Button
			if (mode == CSVTYPE_ONMOUSE && prevBtn) {
				prevBtn->setRelatedElement(csvElement);
			}

			// if button, then store to previous one
			if (mode == CSVTYPE_BUTTON) {
				prevBtn = csvElement;
			}
		} else if (wcsncmp(args[0], L"#DST_", 5) == 0) {
			int mode = CSVElement::getTypeInt(args[0]+5);
			
			// if type is IMG/ONMOUSE/BUTTON
			// then set index 0 (NONE)
			if (mode == CSVReader::CSVTYPE_IMAGE ||
				mode == CSVReader::CSVTYPE_ONMOUSE ||
				mode == CSVReader::CSVTYPE_JUDGELINE ||
				mode == CSVReader::CSVTYPE_SLIDER ||
				mode == CSVReader::CSVTYPE_BUTTON)
				wcscpy(args[1], L"0");

			// if BAR_BODY_OFF / BAR_BODY_ON, then add to BAR_BODY
			// else, then add to previous obj
			if (mode == CSVTYPE_BAR_BODY_OFF || mode == CSVTYPE_BAR_BODY_ON) {
				prev_element[CSVTYPE_BAR_BODY % CACHESIZE]->addDST(args);
			} else {
				//prev_element[mode % CACHESIZE]->dstNum = _wtoi(args[1]);	// to prevent rare error
				prev_element[mode % CACHESIZE]->addDST(args);
			}

			// TODO: if CSVTYPE_BAR_TITLE, set All OP = 0
			if (mode == CSVTYPE_BAR_TITLE)
			{
			}
		} else if (wcscmp(args[0], L"#IMAGE") == 0) {
			currentCSV->images.push_back(args[1]);
		} else if (wcscmp(args[0], L"#BAR_CENTER") == 0) {
			currentCSV->barCenter = _wtoi(args[1]);
		} else if (wcscmp(args[0], L"#FONT") == 0) {
			// 
			CSVFont *csvFont = new CSVFont();
			memset(csvFont, 0, sizeof(csvFont));

			csvFont->fontHeight = _wtoi(args[1]);
			csvFont->fontThickness = _wtoi(args[2]);

			currentCSV->csvFont.push_back(csvFont);
		} else if (wcscmp(args[0], L"#LR2FONT") == 0) {
			// ignore. useless in this version
			OutputDebugString(L"#LR2FONT detected, but it\'ll be ignored.\n");
		} else if (wcscmp(args[0], L"#FONTEX") == 0) {
			// only for LR2CSV option
			// (index), (fontpath), (border), (fontcolor)r, g, b, (bordercolor)r, g, b, (texture)
			int idx = _wtoi(args[1]);
			currentCSV->csvFont[idx]->SetData(args);
		} else if (wcscmp(args[0], L"#ENDOFHEADER") == 0) {
			// if CSVSettings hadn't loaded, then load one
			// else, let unsetted settings to be set
			if (wcscmp(CSVSettings::GetCurrentCSV(), currentCSV->path) != 0)
				CSVSettings::LoadSettings(currentCSV->path);
			CSVSettings::SetDefaultValue(currentCSV, false);
		} else if (wcscmp(args[0], L"#SCRATCHSIDE") == 0) {
		} else if (wcscmp(args[0], L"#TRANSCOLOR") == 0) {
			// set transcolor
			currentCSV->colorKey.r = _wtoi(args[1]);
			currentCSV->colorKey.g = _wtoi(args[2]);
			currentCSV->colorKey.b = _wtoi(args[3]);
		} else if (wcscmp(args[0], L"#STARTINPUT") == 0) {
		} else if (wcscmp(args[0], L"#LOADSTART") == 0) {
			currentCSV->loadStartTime = _wtoi(args[1]);
		} else if (wcscmp(args[0], L"#LOADEND") == 0) {
			currentCSV->loadEndTime = _wtoi(args[1]);
		} else if (wcscmp(args[0], L"#SCENETIME") == 0) {
			currentCSV->sceneTime = _wtoi(args[1]);
		} else if (wcscmp(args[0], L"#PLAYSTART") == 0) {
			currentCSV->playStartTime = _wtoi(args[1]);
		} else if (wcscmp(args[0], L"#FADEOUT") == 0) {
			currentCSV->fadeOutTime = _wtoi(args[1]);
		} else if (wcscmp(args[0], L"#SKIP") == 0) {
		} else if (wcscmp(args[0], L"#CLOSE") == 0) {
		} else if (wcscmp(args[0], L"#CUSTOMOPTION") == 0) {
			// TODO: need to implement #CUSTOMOPTION with CSVData
			CSVCustomOption *csvOption = new CSVCustomOption();
			wcscpy(csvOption->optName, args[1]);
			wcscpy(csvOption->optKey, args[2]);
			wcscpy(csvOption->optValue, args[3]);

			// TODO: parse optValues
			currentCSV->csvOptions.push_back(csvOption);
		} else if (wcscmp(args[0], L"#INCLUDE") == 0) {
			// convert path
			TCHAR absolutePath[256];
			CSVFile::GetPathFromSettings(args[1], absolutePath);
			
			// add arg file to processing line.
			TCHAR *fileData;
			std::wstring fileData_wstr;
			if (!AutoEncoder::readfileEncodingAuto(absolutePath, fileData_wstr))
				return;
			fileData = (TCHAR*)calloc(fileData_wstr.length()+10, sizeof(TCHAR));
			wcscpy(fileData, fileData_wstr.c_str());

			// split lines
			recycled.push_back(fileData);
			if (!parseCSVData(fileData)) {
				return;
			}
		} else if (wcscmp(args[0], L"#SELECT") == 0) {
			currentCSV->sound_select = args[1];
		} else if (wcscmp(args[0], L"#DECIDE") == 0) {
			currentCSV->sound_decide = args[1];
		} else if (wcscmp(args[0], L"#EXSELECT") == 0) {
			currentCSV->sound_exselect = args[1];
		} else if (wcscmp(args[0], L"#EXDECIDE") == 0) {
			currentCSV->sound_exdecide = args[1];
		} else if (wcscmp(args[0], L"#FOLDER_OPEN") == 0) {
			currentCSV->sound_folder_open = args[1];
		} else if (wcscmp(args[0], L"#FOLDER_CLOSE") == 0) {
			currentCSV->sound_folder_close = args[1];
		} else if (wcscmp(args[0], L"#PANEL_OPEN") == 0) {
			currentCSV->sound_panel_open = args[1];
		} else if (wcscmp(args[0], L"#PANEL_CLOSE") == 0) {
			currentCSV->sound_panel_close = args[1];
		} else if (wcscmp(args[0], L"#SCREENSHOT") == 0) {
			currentCSV->sound_screenshot = args[1];
		} else if (wcscmp(args[0], L"#CLEAR") == 0) {
			currentCSV->sound_clear = args[1];
		} else if (wcscmp(args[0], L"#FAIL") == 0) {
			currentCSV->sound_fail = args[1];
		} else if (wcscmp(args[0], L"#STOP") == 0) {
			currentCSV->sound_stop = args[1];
		} else if (wcscmp(args[0], L"#MINE") == 0) {
			currentCSV->sound_mine = args[1];
		} else if (wcscmp(args[0], L"#SCRATCH") == 0) {
			currentCSV->sound_scratch = args[1];
		} else if (wcscmp(args[0], L"#COURSECLEAR") == 0) {
			currentCSV->sound_courseclear = args[1];
		} else if (wcscmp(args[0], L"#COURSEFAIL") == 0) {
			currentCSV->sound_coursefail = args[1];
		} else if (wcscmp(args[0], L"#SETOPTION") == 0) {
			CSVOption::setOption(_wtoi(args[1]), _wtoi(args[2]));
		}
	}
}

int CSVReader::checkCondition(int num) {
	// call DstOption for validation

	return CSVOption::getOption(num);
}