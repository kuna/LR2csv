// part of CSVElement, originally.
#include "CSVElement.h"
#include "CSVData.h"

class CSVRenderer {
private:
	static int cursorX, cursorY;
	static bool (*drawFunc)(int, TCHAR*, CSVSRC*, CSVDST*);
	static void (*notedrawFunc)();

public:
	static void SetCurPos(int x, int y);
	static void SetdrawFunc(bool (*pFunc)(int, TCHAR*, CSVSRC*, CSVDST*));
	static void SetnotedrawFunc(void (*pFunc)());
	
	// for BAR (SELECT)
	static void drawBar(CSVElement *csvElement, int idx);
	// for NUMBER
	static void drawNumber(CSVElement *csvElement, int num);
	static void drawNumber(CSVElement *csvElement);
	static void drawElement(CSVElement *csvElement);
	static void drawImage(CSVElement *csvElement);
	static void drawImage(CSVElement *csvElement, CSVSRC *src, CSVDST *dst);
	static void drawText(CSVElement *csvElement, TCHAR *text);
	static void drawText(CSVElement *csvElement);

	// also availabe with mine
	static void drawNote(CSVElement *csvElement, int key, int yPos);
	static void drawLNNote(CSVElement *csvNote, CSVElement *csvStart, CSVElement *csvBody, CSVElement *csvEnd, int key, int syPos, int eyPos);
	static void drawLine(CSVElement *csvElement, int yPos);
private:
	static int getNumberLength(int num);
	static bool isCursorInside(CSVDST *dst);
};