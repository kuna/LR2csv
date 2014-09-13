#include "Stdafx.h"
#include "CSVOption.h"
#include "CSVConst.h"

// should redeclare (+ initalize)
int CSVOption::option[2000];

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

int CSVOption::RangeValue(int start, int end) {
	for (int i=start; i<=end; i++) {
		if (option[i])
			return i;
	}
	return -1;
}

void CSVOption::RangeUp(int start, int end) {
	int v = RangeValue(start, end);
	v++;
	if (v < start) v = end;
	if (v > end) v = start;
	ClearRange(start, end);
	option[v] = 1;
}

void CSVOption::RangeDown(int start, int end) {
	int v = RangeValue(start, end);
	v--;
	if (v < start) v = end;
	if (v > end) v = start;
	ClearRange(start, end);
	option[v] = 1;
}

void CSVOption::ClearRange(int start, int end) {
	for (int i=start; i<=end; i++)
		option[i] = 0;
}