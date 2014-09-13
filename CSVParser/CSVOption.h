#pragma once

class CSVOption {
public:
	CSVOption();
	static int getOption(int num);
	static void setOption(int num, int val);
	static void InitOption();

	static int RangeValue(int start, int end);
	static void RangeUp(int start, int end);
	static void RangeDown(int start, int end);
	static void ClearRange(int start, int end);
private:
	static int option[2000];
};