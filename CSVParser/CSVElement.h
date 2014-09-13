#include <vector>
#include <list>
#include <tchar.h>
#pragma once

class CSVData;	// for referencing each other

class CSVDST {
public:
	CSVDST(CSVDST *c);
	CSVDST();
	int dst[20];
	int type;

public:
	int getTime();
	int getX();
	int getY();
	int getX2();
	int getY2();
	int getWidth();
	int getHeight();
	int getAcc();
	int getA();
	int getR();
	int getG();
	int getB();
	int getBlend();
	int getFilter();
	int getAngle();
	int getCenter();
	int getCenterX();
	int getCenterY();
	int getLoop();
	int getTimer();
	int getOP1();
	int getOP2();
	int getOP3();
	bool checkOP();

	void setDST(TCHAR *args[]);
	void setDSTX(int x);
	void setDSTY(int y);
	void setDSTHeight(int height);
	void setDSTWidth(int width);

	bool isContains(int x, int y);
};

class CSVSRC {
public:
	int src[20];
	int type;
	void setSRC(TCHAR *data[]);
	
	int getIndex();
	int getImgNum();
	int getType();
	int getX();
	int getY();
	int getX2();
	int getY2();
	int getWidth();
	int getHeight();
	int getdivX();
	int getdivY();
	int getCycle();
	int getTimer();
	int getOP1();
	int getOP2();
	int getOP3();
	bool checkOP();
	// for NUMBER
	int getNum();
	int getAlign();
	int getKeta();
	// for BARGRAPH
	int getMuki();
	// for Font
	int getFontNum();
	int getTextNum();
	int getTextAlign();
	int getEditable();
	// for Button
	int getClick();
	int getPanel();
	int getButtonType();
	// for slider
	int getSliderMuki();
	int getSliderRange();
	int getSliderType();
	int getSliderDisable();
	// for onmouse
	int getONX();
	int getONY();
	int getONX2();
	int getONY2();
	
	void setX(int val);
	void setY(int val);
	void setWidth(int val);
	void setHeight(int val);
	void setSize(int x, int y, int width, int height);
	void setImgNum(int val);
};

class CSVElement {
public:
	CSVElement();
	~CSVElement();
	int type;
	CSVSRC* src[100];				// for BAR_BODY
	std::vector<CSVDST*> dst[100];	// for BAR_BODY (uses only index 0, normally.)
	CSVData *parent;
	
	// draw filtering
	int srcNum;
	int dstNum;

	// draw option
	int dstOffsetX;
	int dstOffsetY;

public:
	void addSRC(TCHAR *data[]);
	void addDST(TCHAR *data[]);

	// calculation part
	void getSRCFromIdx(CSVSRC *c, int idx);
	void getSRC(CSVSRC *c);
	int getSRCIndex();
	CSVSRC* getCurrentSRC();
	CSVDST* getCurrentDST(int idx);
	CSVDST* getCurrentLastDST();
	bool getDST(CSVDST *c);
	bool getDSTBar(CSVDST *c, int idx);
	void getNUMSRC(CSVSRC *c, int num, bool negative, bool sign=false);

	void setType(TCHAR *str);
	void setType(int type);
	int getType();
	static int getTypeInt(TCHAR *str);

	// relation
	bool isRelatedExists();
	//void setRelatedElement(CSVElement *csv);	TODO: depreciated
	//CSVElement* getRelatedElement();
private:
	//CSVElement* related;	// this will be used with button (ONMOUSE) TODO: depreciated

	int getNumberLength(int num);
};