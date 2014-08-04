#include "CSVNumber.h"

int CSVNumber::data[1000];

int CSVNumber::getNumber(int num) {
	return data[num];
}

void CSVNumber::setNumber(int num, int val) {
	data[num] = val;
}