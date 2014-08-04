
// check event about clicking somewhere 

#include "CSVElement.h"
#include "CSVData.h"

#define MAXID 1000

class CSVButton {
private:
	static CSVData* currentCSV;
	static int btnstatus[MAXID];
public:
	static void SetCurrentCSV(CSVData *csv);
	static CSVElement *GetElementOfPos(CSVData *csv, int x, int y, int filter=-1);
	static void Click(int x, int y);
	static void ProcessButtonID(int id);
	
	static bool GetClick(int btnnum);
	static void SetClickDown(int btnnum);
	static void SetClickUp(int btnnum);
	static void SetClickToggle(int btnnum);
	static void SetClickClear();
};