// muki: direction (0 - clock)
// range
// type: number
// disable

#include "CSVData.h"

class CSVSlider {
private:
	static double data[100];

	static CSVData* csvData;
	static CSVElement* currentSlider;
public:
	static double getSliderValue(int num);
	static void setSliderValue(int num, double val);

	static void setCSVData(CSVData *data);
	static int mouseDown(int x, int y);	// returns current slider type
	static int mouseMove(int x, int y);	// returns current slider type
	static void mouseUp();
};