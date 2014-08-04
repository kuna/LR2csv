#include "CSVOption.h"
#include "CSVConst.h"

// should redeclare (+ initalize)
int CSVOption::option[1000];

void CSVOption::InitOption() {
	// MUST CALL WHEN GAME STARTS
	setOption(CSVOptionConst::ALWAYS_TRUE, 1);
	setOption(910, 1);	// is LR2?
	setOption(911, 0);	// is LR1?
}

int CSVOption::getOption(int num) {
	if (num < 0)
		return !option[-num];
	else
		return option[num];
}

void CSVOption::setOption(int num, int val) {
	option[num] = val;
}