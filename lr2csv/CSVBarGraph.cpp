#include "CSVBargraph.h"

double CSVBargraph::num[100];

void CSVBargraph::setVal(int id, double val) {
	num[id] = val;
}

double CSVBargraph::getVal(int id) {
	return num[id];
}