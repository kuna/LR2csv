#include "CSVData.h"
#include <tchar.h>
//#include <map>

#define CACHESIZE 100

class CSVReader {
public:
	static bool readCSVFile(TCHAR *path, CSVData *csvData);
	static bool parseCSVData(TCHAR *data, CSVData *csvData);

private:
	static std::vector<TCHAR*> lines;
	static std::vector<TCHAR*> recycled;			// used when memory free after parse finished
	static CSVElement* prev_element[CACHESIZE];

	static void processCSVLines();
	static void processCSVLine(TCHAR *data);
	static int checkCondition(int num);
	static bool convert(const char *input, int len, const char *from, TCHAR **output);
	static bool readFile(TCHAR *path, TCHAR **out);	// with Automatic encoding

private:
	static CSVData *currentCSV;
	static int condition[256];		// used #IF~#ELSEIF~#ELSE~#ENDIF. true: 1, falsed: 0, used: 2
	static int conditionDepth;

public:
	// these should be initalized
	static int CSVTYPE_IMAGE;
	static int CSVTYPE_BUTTON;
	static int CSVTYPE_ONMOUSE;
	static int CSVTYPE_TEXT;
	static int CSVTYPE_BARGRAPH;
	static int CSVTYPE_GROOVEGUAGE;
	static int CSVTYPE_BGA;
	static int CSVTYPE_JUDGELINE;
	static int CSVTYPE_SLIDER;
	static int CSVTYPE_LINE;
	static int CSVTYPE_NOTE;
	static int CSVTYPE_LNSTART;
	static int CSVTYPE_LNEND;
	static int CSVTYPE_LNBODY;
	static int CSVTYPE_MINE;
	static int CSVTYPE_NUMBER;
	static int CSVTYPE_NOWJUDGE_1P;
	static int CSVTYPE_NOWCOMBO_1P;
	static int CSVTYPE_NOWJUDGE_2P;
	static int CSVTYPE_NOWCOMBO_2P;
	
	static int CSVTYPE_BAR_BODY;		// SRC
	static int CSVTYPE_BAR_BODY_OFF;	// DST
	static int CSVTYPE_BAR_BODY_ON;		// DST
	static int CSVTYPE_BAR_CENTER;
	static int CSVTYPE_BAR_AVAILABLE;
	static int CSVTYPE_BAR_FLASH;
	static int CSVTYPE_BAR_LEVEL;
	static int CSVTYPE_BAR_LAMP;
	static int CSVTYPE_BAR_MY_LAMP;
	static int CSVTYPE_BAR_RIVAL_LAMP;
	static int CSVTYPE_BAR_TITLE;
	static int CSVTYPE_BAR_RANK;
	static int CSVTYPE_BAR_RIVAL;

	static CSVElement *prevBtn;	// for BUTTON~ONMOUSE event
};