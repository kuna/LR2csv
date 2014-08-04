#include "CSVButton.h"
#include "CSVReader.h"
#include "CSVConst.h"
#include "CSVTimer.h"
#include "CSVOption.h"

CSVData* CSVButton::currentCSV = 0;
int CSVButton::btnstatus[MAXID];

void CSVButton::SetCurrentCSV(CSVData *csv) {
	currentCSV = csv;
}

// filter == -1 -> only button (default)
// filter == 0 -> all elements
CSVElement *CSVButton::GetElementOfPos(CSVData *csv, int x, int y, int filter) {
	if (!csv)
		return 0;

	if (filter < 0)
		filter = CSVReader::CSVTYPE_BUTTON;

	// scan all elements...
	for (int i=0; i<csv->csvElement.size(); i++) {
		CSVElement *c = csv->csvElement[i];
		if (c->getType() == filter || filter == 0) {
			CSVDST cdst;
			if (c->getDST(&cdst) && cdst.isContains(x, y)) {
				return c;
			}
		}
	}

	// nope!
	return 0;
}

void CSVButton::Click(int x, int y) {
	// find obj
	CSVElement *button = GetElementOfPos(currentCSV, x, y);

	// if found, then process
	// TODO: when id = -1, it's deactivating...?
	if (button) {
		ProcessButtonID(button->getCurrentSRC()->getButtonType());
	}
}

void CSVButton::ProcessButtonID(int id) {
	switch (id) {
		case CSVButtonConst::PANEL1:
			if (!GetClick(id)) {
				// open panel 1
				CSVTimer::setTime(CSVTimerConst::PANEL1_START);
				CSVTimer::deActiviateTimer(CSVTimerConst::PANEL1_END);
				CSVOption::setOption(CSVOptionConst::PANEL_1START, 1);
			} else {
				// close panel 1
				CSVTimer::deActiviateTimer(CSVTimerConst::PANEL1_START);
				CSVTimer::setTime(CSVTimerConst::PANEL1_END);
				CSVOption::setOption(CSVOptionConst::PANEL_1START, 0);
			}
			SetClickToggle(id);
			break;
	}
}

bool CSVButton::GetClick(int btnnum) {
	return btnstatus[btnnum];
}

void CSVButton::SetClickDown(int btnnum) {
	btnstatus[btnnum]++;
}

void CSVButton::SetClickUp(int btnnum) {
	btnstatus[btnnum]--;
}

void CSVButton::SetClickToggle(int btnnum) {
	btnstatus[btnnum] = (btnstatus[btnnum]==1)?0:1;
}

void CSVButton::SetClickClear() {
	for (int i=0; i<MAXID; i++)
		btnstatus[i] = 0;
}
