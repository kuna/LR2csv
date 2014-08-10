#pragma once

class CSVOption {
public:
	CSVOption();
	static int getOption(int num);
	static void setOption(int num, int val);
	static void InitOption();

private:
	static int option[2000];
};