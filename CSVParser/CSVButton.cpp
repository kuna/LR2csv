#include "Stdafx.h"
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
				if (filter == CSVReader::CSVTYPE_BUTTON) {
					// if only button,
					// then check out for valid panel
					CSVSRC *src = c->getCurrentSRC();
					if (src->getPanel() == 0 && !(CSVOption::getOption(CSVOptionConst::PANEL_1START) || 
						CSVOption::getOption(CSVOptionConst::PANEL_2START) ||
						CSVOption::getOption(CSVOptionConst::PANEL_3START) ||
						CSVOption::getOption(CSVOptionConst::PANEL_4START) ||
						CSVOption::getOption(CSVOptionConst::PANEL_5START) ||
						CSVOption::getOption(CSVOptionConst::PANEL_6START) ||
						CSVOption::getOption(CSVOptionConst::PANEL_7START) ||
						CSVOption::getOption(CSVOptionConst::PANEL_8START) ||
						CSVOption::getOption(CSVOptionConst::PANEL_9START))) {
							return c;
					} else if (CSVOption::getOption(20+src->getPanel())) {
						return c;
					} else {
						continue;
					}
				} else {
					return c;
				}
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
	// TODO: check x/2 relativity for option forward/prev
	if (button) {
		CSVDST dst;
		button->getDST(&dst);

		ProcessButtonID(button->getCurrentSRC()->getButtonType(), 
			x-dst.getX(), y-dst.getY(), dst.getWidth(), dst.getHeight());
	}
}

void AutoRange(bool up, int start, int end) {
	if (up) {
		// forward
		CSVOption::RangeUp(start, end);
	} else {
		// prev
		CSVOption::RangeDown(start, end);
	}
}

void CSVButton::ProcessButtonID(int id, int x, int y, int wid, int hei) {
	// 77, 96?
	switch (id) {
		/* *********************************
		 * PANEL PART
		 * *********************************/
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
			
		/* *********************************
		 * OPTION BUTTON PART
		 * *********************************/
		case CSVButtonConst::OPTION_PLAY_GUAGE_1P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::GUAGEMODE_1P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::GUAGEMODE_1P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_GUAGE_2P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::GUAGEMODE_2P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::GUAGEMODE_2P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_ASSIST_1P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::ASSIST_1P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::ASSIST_1P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_ASSIST_2P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::ASSIST_2P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::ASSIST_2P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_BGASIZE:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::BGASIZE);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::BGASIZE);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_HS_1P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::HISPEED_1P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::HISPEED_1P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_HS_2P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::HISPEED_2P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::HISPEED_2P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_RANDOM_1P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::RANDOM_1P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::RANDOM_1P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_RANDOM_2P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::RANDOM_2P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::RANDOM_2P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_BGA:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::BGA);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::BGA);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_BATTLE:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::BATTLE);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::BATTLE);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_DPFLIP:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::FLIP);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::FLIP);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_EFFECT_1P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::EFFECT_1P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::EFFECT_1P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_EFFECT_2P:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::EFFECT_2P);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::EFFECT_2P);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_GHOST:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::GHOST);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::GHOST);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_HSFIX:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::HISPEED_FIX);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::HISPEED_FIX);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_JUDGETIMING:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::JUDGETIMING);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::JUDGETIMING);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_JUDGETIMING_AUTO:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::JUDGETIMING_AUTO);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::JUDGETIMING_AUTO);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_SAVE_REPLAY:
			CSVSettings::playOption.option_next(CSVPlayOptionConst::SAVE_REPLAY);
			break;
		case CSVButtonConst::OPTION_PLAY_SCOREGRAPH:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::SCOREGRAPH);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::SCOREGRAPH);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_SCREENMODE:
			// TODO: fullscreen / window process
			CSVSettings::playOption.option_next(CSVPlayOptionConst::SCREENMODE);
			break;
		case CSVButtonConst::OPTION_PLAY_SHUTTER:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::SHUTTER);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::SHUTTER);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_TARGET:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::TARGET);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::TARGET);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_TARGET_RATE:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::TARGETRATE);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::TARGETRATE);
			}
			break;
		case CSVButtonConst::OPTION_PLAY_VSYNC:
			// TODO ...?
			//CSVSettings::playOption.vsync_onoff();
			break;
		case CSVButtonConst::KEY_FILTER:	// Top menu
		case CSVButtonConst::DIFFICULTY_FILTER:
			if (x > wid/2) {
				// forward
				CSVSettings::playOption.option_next(CSVPlayOptionConst::PLAYMODE);
			} else {
				// prev
				CSVSettings::playOption.option_prev(CSVPlayOptionConst::PLAYMODE);
			}
			break;
	}

	// redeclare text
	// TODO: need optimization
	CSVSettings::playOption.option_applytext();
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
