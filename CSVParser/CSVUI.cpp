#include "StdAfx.h"
#include "CSVConst.h"
#include "CSVOption.h"
#include "CSVTimer.h"
#include "CSVUI.h"

void CSVUI::panelShow(int id) {
	if (id < 1 || id > 9) return;

	// only when panel is hidden
	if (!CSVOption::getOption(20+id)) {
		CSVTimer::setTime(20+id);
		CSVTimer::deActiviateTimer(30+id);
		CSVOption::setOption(20+id, 1);
	}
}

void CSVUI::panelHide(int id) {
	if (id < 1 || id > 9) return;

	// only when panel is shown
	if (CSVOption::getOption(20+id)) {
		CSVTimer::deActiviateTimer(20+id);
		CSVTimer::setTime(30+id);
		CSVOption::setOption(20+id, 0);
	}
}