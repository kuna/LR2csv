#include "Stdafx.h"
#include "CSVSlider.h"
#include "CSVButton.h"	// to use search function
#include "CSVReader.h"	// to use const

double CSVSlider::data[100];
CSVData* CSVSlider::csvData;
CSVElement* CSVSlider::currentSlider;

double CSVSlider::getSliderValue(int num) {
	return data[num];
}

void CSVSlider::setSliderValue(int num, double val) {
	data[num] = val;
}

void CSVSlider::setCSVData(CSVData *data) {
	csvData = data;
}

int CSVSlider::mouseDown(int x, int y) {
	if (csvData) {
		currentSlider = CSVButton::GetElementOfPos(csvData, x, y, CSVReader::CSVTYPE_SLIDER);
		if (currentSlider != 0) {
			if (currentSlider->getCurrentSRC()->getSliderDisable() == 1) {
				// cannot slide
				currentSlider = 0;
			} else {
				return currentSlider->getCurrentSRC()->getSliderType();
			}
		}
	}

	return 0;
}

inline double setRange0to1(double v) {
	if (v < 0) return 0;
	else if (v > 1) return 1;
	else return v;
}

int CSVSlider::mouseMove(int x, int y) {
	if (csvData && currentSlider) {
		CSVSRC *e = currentSlider->getCurrentSRC();
		CSVDST dst;
		
		// restore value for CSVSlider class (mouse position calculate)
		currentSlider->dstOffsetX = 0;
		currentSlider->dstOffsetY = 0;
		currentSlider->getDST(&dst);

		int range = e->getSliderRange();
		double dx = (double)(x - dst.getX()) / range;
		double dy = (double)(y - dst.getY()) / range;

		double d = 0;
		switch (e->getSliderMuki()) {
		case 0:
			d = 1-dy;
			break;
		case 1:
			d = dx;
			break;
		case 2:
			d = dy;
			break;
		case 3:
			d = 1-dx;
			break;
		}

		setSliderValue(e->getSliderType(), setRange0to1(d));

		return e->getSliderType();
	}
	
	return 0;
}

void CSVSlider::mouseUp() {
	currentSlider = 0;
}