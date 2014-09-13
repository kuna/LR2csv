#include "Stdafx.h"
#include "CSVElement.h"
#include "CSVTimer.h"
#include "CSVOption.h"
#include "CSVReader.h"
#include "CSVNumber.h"
#include "CSVBargraph.h"
#include "CSVConst.h"
#include "CSVSelectList.h"
#include "CSVData.h"

/* 
BGA
JUDGELINE - SAME DRAWING
SLIDER
//SRC定義,(null),gr,x,y,w,h,div_x,div_y,cycle,timer,muki,range,type,disable,,,,,,
//DST定義,(null),time,x,y,w,h,acc,a,r,g,b,blend,filter,angle,center,loop,timer,op1,op2,op3
BUTTON
//hi-speed_fix,//button55 HS-FIX(OFF/MAXBPM/MINBPM/AVERAGE/CONSTANT),,,,,,,,,,,,,,,,,,,
//SRC定義,index,gr,x,y,w,h,div_x,div_y,cycle,timer,type,click,panel,plusonly,,,,,,
LINE

NOTE
LNSTART
LNEND
MINE

grooveguage
//SRC定義,index,gr,x,y,w,h,div_x,div_y,cycle,timer,add_x,add_y,,,,,,,,

number
// 10, 11, 24배수로 분할.
// num - 으로부터 숫자 받아옴.
//SRC定義,(null),gr,x,y,w,h,div_x,div_y,cycle,timer,num,align,keta,,,,,,,
//DST定義,(NULL),time,x,y,w,h,acc,a,r,g,b,blend,filter,angle,center,loop,timer,op1,op2,op3

NOWJUDGE_1P
same as SRC_IMAGE, but automatically moves left for combo
noshift1 - don't move
//SRC定義,index,gr,x,y,w,h,div_x,div_y,cycle,timer,noshift,op2,op3,,,,,,,

NOWCOMBO_1P
relative to dst_judge. same as DST_NUMBER

TEXT
// edit - 편집 가능하나 반영하는 것이 무의미.
// st - text
// panel - 해당 패널이 열려있는 상태에서만 편집 가능. (-1: 닫힌 상태에서만 편집가능)
// align - 0,1,2 / left,center,right
// if text is too long, then auto compression.
//SRC定義,(NULL),font,st,align,edit,panel,,,,,,,,,,,,,,
//DST定義,(NULL),time,x,y,w,size,acc,a,r,g,b,blend,filter,(NULL),(NULL),loop,timer,op,op,op

BARGRAPH
// uses muki (0: horizontal, 1: vertical)
// type is value (100% is full)
//SRC定義,(NULL),gr,x,y,w,h,div_x,div_y,cycle,timer,type,muki,op3,,,,,,,
//DST定義,(NULL),time,x,y,w,h,acc,a,r,g,b,blend,filter,angle,center,loop,timer,op1,op2,op3


BUTTON
 - type: 버튼의 기능
 - 애니메이션효과: 아래에서 위로 올라오는 방식..? acc2: 감속 
//SRC定義,(NULL),gr,x,y,w,h,div_x,div_y,cycle,timer,type,click,panel,,,,,,,

ONMOUSE - button에 마우스 올렸을때 표시할 이미지.
//SRC定義,(NULL),gr,x,y,w,h,div_x,div_y,cycle,timer,panel,x2,y2,w2,h2,,,,,
//DST定義,(NULL),time,x,y,w,h,acc,a,r,g,b,blend,filter,angle,center,loop,timer,op1,op2,op3

MOUSECURSOR - image랑 비슷.

RELOADBANNER(decide)
HELPFILE

FONT

(select)
SRC
BAR_BODY
/ / 노래 바 이미지를 잘라냅니다. , 
/ / index : 0 곡 1 폴더 2 사용자 정의 폴더 3 신곡 폴더 4 라이벌 폴더 5 곡 (경쟁 모드) 6 코스 폴더 7 코스 만들기 8 코스 9 랜덤 코스,
DST
BAR_BODY_OFF
BAR_BODY_ON
/ / 노래 모음을 배치합니다. 보이지 않는 부분 제대로 배치한다. ,,,,,,,,,,,,, 
/ / index : 0 - 29까지 사용 가능. LR1.5와 달리 첫 번째 애니메이션도 개별적으로 정의 할 수 있습니다. ,,,,,,,,,,,,,
//DST定義,index,time,x,y,w,h,acc,a,r,g,b,blend,filter,angle,center,loop,timer,(NULL),(NULL),(NULL)
//DST定義,index,time,x,y,w,h,acc,a,r,g,b,blend,filter,angle,center,loop,timer,(NULL),(NULL),(NULL)
#DST_BAR_BODY_OFF,0,625,685,-31,267,26,2,255,255,255,255,0,0,0,0,775,0,0,0,0
#DST_BAR_BODY_OFF,0,775,397,-31,267,26,2,255,255,255,255,0,0,0,0,,,,,
//DST定義,index,time,x,y,w,h,acc,a,r,g,b,blend,filter,angle,center,loop,timer,(NULL),(NULL),(NULL)
#DST_BAR_BODY_ON ,0,625,673,-31,267,26,2,255,255,255,255,0,0,0,0,775,0,0,0,0
#DST_BAR_BODY_ON ,0,775,381,-31,267,26,2,255,255,255,255,0,0,0,0,,,,,

BAR_CENTER - 노래 바 중앙의 번호를 지정하십시오. 기본값은 0입니다.

BAR_AVAILABLE - 마우스 커서를 가져 가면 포커스 맞출 이동 막대를 지정하십시오. 
기본값은 0-30입니다. 범위를 벗어난 바의 위치에 맞추면, 바 스크롤합니다.

BAR_FLASH - focus된 곡의 상태를 지시함.

BAR_LEVEL - 곡 레벨. #NUMBER과 비슷. 노래 바에서의 상대 거리로 지정.
6종류 사용 - 미정의, beginner, normal, hyper, another, insane, ranking

BAR_LAMP - gr 부분에 index가 들어감. ...?
설정 안 되어 있는 램프는 그리지 않는 걸로. (혹은 커스터마이징 필요)
/ / () 추가 할지도 모른다 클리어 랭크입니다. ,,,,,,,,,,,,,,,,,,,, 
/ / 0 noplay,,,,,,,,,,,,,,,,,,,, 
/ / 1 fail,,,,,,,,,,,,,,,,,,,, 
/ / 2 easy clear,,,,,,,,,,,,,,,,,,,, 
/ / 3 groove clear,,,,,,,,,,,,,,,,,,,, 
/ / 4 survival clear,,,,,,,,,,,,,,,,,,,, 
/ / 5 (g+ s clear),,,,,,,,,,,,,,,,,,,, 
/ / 6 fullcombo,,,,,,,,,,,,,,,,,,,, 
/ / 7 (perfect),,,,,,,,,,,,,,,,,,,,

BAR_MY_LAMP

BAR_RIVAL_LAMP

BAR_TITLE - 상대 위치 지정.

BAR_RANK - relative position.

BAR_RIVAL - relative position. (win / lose / draw / not played) - index 항목.

README
// DST_TEXT 형식으로 첫번째 행만 지정할 것. (kankaku: 본문)
//SRC定義,(NULL),font,(NULL),(NULL),kankaku,(NULL),,,,,,,,,,,,,,
//DST定義,(NULL),time,x,y,w,size,acc,a,r,g,b,blend,filter,(NULL),(NULL),loop,timer,(NULL),(NULL),(NULL)

미지:
- 1. 패널은 어떻게 띄움? - 버튼으로. 열리고 난 이후에 element 그리기 시작함.
- 2. readme는 어떻게 띄움? (button의 action) - button 200-209에 정의. #HELPFILE에 정의함.
- 3. body_off랑 body_on의 차이점은? -> on은 선택한 놈의 인덱스를 의미함.

*/

CSVElement::CSVElement() {
	dstNum = 0;
	srcNum = 0;
	dstOffsetX = 0;
	dstOffsetY = 0;
}

CSVElement::~CSVElement() {
	// TODO: check fully delete
	// clear dst (check)
	for (int dstidx=0; dstidx<100; dstidx++){
		for (int i=0; i<dst[dstidx].size(); i++) {
			delete dst[dstidx][i];
		}
		// there's bug in VS 2010
		// and this'll be automatically removed 
		// so we won't care ...
		//dst[dstidx].clear();
	}

	// clear src
	for (int i=0; i<100; i++) {
		if (src[i])
			delete src[i];
	}
}

int CSVElement::getTypeInt(TCHAR *str) {
	// calculate type hash
	// for ease of comparing

	int r = 0;
	for (int i=0; i<wcslen(str); i++) {
		r += *(str+i);
		r %= 10000000;
	}
	return r;
}

void CSVElement::setType(int type) {
	this->type = type;
}

void CSVElement::setType(TCHAR *str) {
	this->type = getTypeInt(str+5);
}

int CSVElement::getType() {
	return type;
}

void CSVElement::addSRC(TCHAR *args[]) {
	// (NULL)	gr	x	y	w	h	div_x	div_y	cycle	timer	op1	op2	op3
	CSVSRC* src = new CSVSRC();
	memset(src, 0, sizeof(CSVSRC));

	src->setSRC(args);

	this->src[src->getIndex()] = src;
}

void CSVElement::addDST(TCHAR *args[]) {
	// (NULL)	time	x	y	w	h	acc	a	r	g	b	blend	filter	angle	center	loop	timer	op1	op2	op3
	CSVDST* dst = new CSVDST();
	memset(dst, 0, sizeof(CSVDST));

	dst->setDST(args);

	this->dst[dst->dst[0] + 50*(dst->type == CSVReader::CSVTYPE_BAR_BODY_ON)].push_back(dst);
}

void CSVElement::getSRCFromIdx(CSVSRC *c, int idx) {
	// if idx is over maximum ...?
	int wid = src[srcNum]->getWidth() / src[srcNum]->getdivX();
	int hei = src[srcNum]->getHeight() / src[srcNum]->getdivY();
	c->setSize(src[srcNum]->getX() + wid * (idx%src[srcNum]->getdivX()),
		src[srcNum]->getY() + hei * (idx/src[srcNum]->getdivX()),
		wid, hei);
	c->setImgNum(src[srcNum]->getImgNum());
}

void CSVElement::getSRC(CSVSRC *c) {
	getSRCFromIdx(c, getSRCIndex());
}

int CSVElement::getSRCIndex() {
	long time = CSVTimer::getTime(src[srcNum]->getTimer());

	// 0 cycle: don't animate.
	// prevent 0 DIV error!
	int idx = 0;
	if (src[srcNum]->getCycle() > 0) {
		int TotalFrame = (src[srcNum]->getdivX()*src[srcNum]->getdivY());
		int TimePerFrame = src[srcNum]->getCycle() / TotalFrame;
		idx = (time/TimePerFrame) % (TotalFrame) ;
	}
	return idx;
}

CSVSRC* CSVElement::getCurrentSRC() {
	return src[srcNum];
}

CSVDST* CSVElement::getCurrentDST(int idx) {
	return dst[dstNum][idx];
}

CSVDST* CSVElement::getCurrentLastDST() {
	int idx = dst[dstNum].size()-1;
	return getCurrentDST(idx);
}

void CSVElement::getNUMSRC(CSVSRC *c, int num, bool negative, bool sign) {
	// INFO
	// negative num only works
	// in 24 mode.
	// if num is smaller then 0 && !(10 mode),
	// then it'll show zero of non-number. (ind 10)

	// NUMBER has no OP condition,
	// so we skip checkOP.

	long time = CSVTimer::getTime(src[srcNum]->getTimer());
	
	int multiply = 10;	// basic
	if ((src[srcNum]->getdivX() * src[srcNum]->getdivY())%10 == 0) {
		multiply = 10;
	} else if ((src[srcNum]->getdivX() * src[srcNum]->getdivY()) % 11 == 0) {
		multiply = 11;
	} else if ((src[srcNum]->getdivX() * src[srcNum]->getdivY()) % 24 == 0) {
		multiply = 24;
	}

	// get idx of cycle
	int idx = 0;
	if (src[srcNum]->getCycle() > 0) {
		int TotalFrame = (src[srcNum]->getdivX()*src[srcNum]->getdivY()) / multiply;
		int TimePerFrame = src[srcNum]->getCycle() / TotalFrame;
		idx = (time/TimePerFrame) % TotalFrame;
	}
	
	// convert num
	if (sign) {
		if (negative) {
			num = 23;
		} else {
			num = 11;
		}
	} else {
		if (num < 0) {	// non-number zero
			if (negative) {
				num = 22;
			} else {
				num = 10;
			}
		} else {
			// for negative number
			if (negative) {
				if (multiply == 24) {
					num = num + 12;
				}
			}
		}
	}

	// get idx
	int nidx = num + multiply*idx;

	// and get rect
	getSRCFromIdx(c, nidx);
}

bool CSVElement::getDST(CSVDST *c) {
	// if no dsts, then DONT draw
	if (dst[dstNum].size() <= 0)
		return false;

	// check condition
	// skip if CSVTYPE_BAR_TITLE
	if (dst[dstNum][0]->type == CSVReader::CSVTYPE_BAR_TITLE) {
	} else {
		if (!dst[dstNum][0]->checkOP())
			return false;		// DONT draw if OP dont matches
	}

	int loop = dst[dstNum][0]->getLoop();
	int timer = dst[dstNum][0]->getTimer();
	int starttime = dst[dstNum][0]->getTime();
	int endtime = dst[dstNum][dst[dstNum].size()-1]->getTime();
	long time = CSVTimer::getTime(timer);

	if (!CSVTimer::isTimerActiviated(timer))
		return false;		// DONT draw if timer off

	if (time < starttime)
		return false;		// DONT draw under starttime

	// check LOOP (loop = time to start loop ~ endtime)
	// ref https://github.com/kuna/Rhythmus/blob/master/rhythmus/Skin/SkinElement.cpp
	if (time > loop && loop >= 0) {
		if (loop == endtime) {
			time = loop;
		} else {
			time = (time - loop)%(loop - endtime) + loop;
		}
	}

	// 1~n - get internal value
	for (int i = 1; i < dst[dstNum].size(); i++) {
		if (time >= dst[dstNum][i-1]->getTime() && time < dst[dstNum][i]->getTime()) {
			// draw between dst[dstNum]s
#define SQUARE(x) ((x)*(x))
			CSVDST *prev = dst[dstNum][i-1];
			CSVDST *cur = dst[dstNum][i];
			int accl = prev->getAcc();

			double a = (double)(time - prev->getTime()) / (cur->getTime() - prev->getTime()); // TODO: CAUTION: 0 div
			double b;
			if (accl == 0) {		// normal
				b = 1-a;
			} else if (accl == 1) {	// 가속
				a = SQUARE(a);
				b = 1-a;
			} else if (accl == 2) {	// 감속
				b = 1-a;
				b = SQUARE(b);
				a = 1-b;
			} else if (accl == 3) {	// EaseInOut (불규칙)
				double x = SQUARE(0.5-a);
				if (a>0.5) a = 0.5+x;
				else a = 0.5-x;
				b = 1-a;
			}

			// apply a & b to element
			for (int cidx=0; cidx<15; cidx++) {
				if (cidx == 0 ||	// index
					cidx == 1 ||	// imgnum
					cidx == 6 ||	// acc
					cidx == 11 ||	// blend
					cidx == 12 ||	// filter
					cidx == 14) {	// center
					c->dst[cidx] = prev->dst[cidx];
					continue;
				}
				c->dst[cidx] = (int)(prev->dst[cidx] * b + cur->dst[cidx] * a + 0.001/* TODO more precision */);
			}
#undef SQUARE

			// set offset
			c->setDSTX( c->getX() + dstOffsetX );
			c->setDSTY( c->getY() + dstOffsetY );

			return true;
		}
	}

	// after end - return end status
	// if loop < 0, then it shouldn't be drawn.
	if (time >= dst[dstNum][dst[dstNum].size()-1]->getTime()) {
		if (loop < 0) {
			return false;
		} else {
			for (int cidx=0; cidx<16; cidx++) {
				c->dst[cidx] = dst[dstNum][dst[dstNum].size()-1]->dst[cidx];
			}

			// set offset
			c->setDSTX( c->getX() + dstOffsetX );
			c->setDSTY( c->getY() + dstOffsetY );
			return true;
		}
	}

	// no case...
	return false;
}

bool CSVElement::getDSTBar(CSVDST *c, int idx) {
	dstNum = idx;
	if (idx == parent->barCenter)
		dstNum += 50;

	bool r = getDST(c);

	// restore everything to default
	dstNum = 0;

	return r;
}

void CSVSRC::setSRC(TCHAR *args[]) {
	type = CSVElement::getTypeInt(args[0]+5);

	for (int i=1; i<20 && args[i]; i++) {
		src[i-1] = _wtoi(args[i]);
	}
}

int CSVSRC::getIndex() {
	return src[0];
}

int CSVSRC::getImgNum() {
	return src[1];
}

int CSVSRC::getX() {
	return src[2];
}

int CSVSRC::getY() {
	return src[3];
}

int CSVSRC::getX2() {
	return getX() + getWidth();
}

int CSVSRC::getY2() {
	return getY() + getHeight();
}

void CSVSRC::setX(int val) {
	src[2] = val;
}

void CSVSRC::setY(int val) {
	src[3] = val;
}

void CSVSRC::setWidth(int val) {
	src[4] = val;
}

void CSVSRC::setHeight(int val) {
	src[5] = val;
}

void CSVSRC::setImgNum(int val) {
	src[1] = val;
}

void CSVSRC::setSize(int x, int y, int width, int height) {
	setX(x);
	setY(y);
	setWidth(width);
	setHeight(height);
}

int CSVSRC::getWidth() {
	return src[4];
}

int CSVSRC::getHeight() {
	return src[5];
}

int CSVSRC::getdivX() {
	if (src[6] < 1) return 1;
	return src[6];
}

int CSVSRC::getdivY() {
	if (src[7] < 1) return 1;
	return src[7];
}

int CSVSRC::getCycle() {
	return src[8];
}

int CSVSRC::getTimer() {
	return src[9];
}

int CSVSRC::getOP1() {
	return src[10];
}

int CSVSRC::getOP2() {
	return src[11];
}

int CSVSRC::getOP3() {
	return src[12];
}

int CSVSRC::getNum() {
	return src[10];
}

int CSVSRC::getAlign() {
	// 0: right, 1: left, 2: center
	return src[11];
}

int CSVSRC::getTextAlign() {
	// 0: right, 1: left, 2: center
	return src[3];
}

int CSVSRC::getKeta() {
	// number count
	return src[12];
}

int CSVSRC::getMuki() {
	return src[11];
}

int CSVSRC::getFontNum() {
	return src[1];
}

int CSVSRC::getTextNum() {
	return src[2];
}

int CSVSRC::getEditable() {
	return src[4];
}

int CSVSRC::getClick() {
	return getOP2();
}

int CSVSRC::getPanel() {
	return getOP3();
}

int CSVSRC::getButtonType() {
	return getOP1();
}

int CSVSRC::getSliderMuki() {
	return src[10];
}

int CSVSRC::getSliderRange() {
	return src[11];
}

int CSVSRC::getSliderType() {
	return src[12];
}

int CSVSRC::getSliderDisable() {
	return src[13];	// longest argument!
}

int CSVSRC::getONX() {
	return src[11];
}

int CSVSRC::getONY() {
	return src[12];
}

int CSVSRC::getONX2() {
	return src[13];
}

int CSVSRC::getONY2() {
	return src[14];	// longest argument!
}

bool CSVSRC::checkOP() {
	return (CSVOption::getOption(getOP1()) && CSVOption::getOption(getOP2()) && CSVOption::getOption(getOP3()));
}

int CSVElement::getNumberLength(int num) {
	int r = 1;
	while (true) {
		num /= 10;
		if (num > 0)
			r++;
		else
			break;
	}
	
	return r;
}

// CSVDST
CSVDST::CSVDST() {
}

CSVDST::CSVDST(CSVDST *c) {
	memcpy(dst, c->dst, sizeof(dst));
}

int CSVDST::getTime() {
	return dst[1];
}

int CSVDST::getX() {
	return dst[2];
}

int CSVDST::getY() {
	return dst[3];
}

int CSVDST::getX2() {
	return getX()+getWidth();
}

int CSVDST::getY2() {
	return getY()+getHeight();
}

int CSVDST::getWidth() {
	return dst[4];
}

int CSVDST::getHeight() {
	return dst[5];
}

int CSVDST::getAcc() {
	return dst[6];
}

int CSVDST::getA() {
	return dst[7];
}

int CSVDST::getR() {
	return dst[8];
}

int CSVDST::getG() {
	return dst[9];
}

int CSVDST::getB() {
	return dst[10];
}

int CSVDST::getBlend() {
	return dst[11];
}

int CSVDST::getFilter() {
	return dst[12];
}

int CSVDST::getAngle() {
	return dst[13];
}

int CSVDST::getCenter() {
	return dst[14];
}

int CSVDST::getCenterX() {
	// 7, 4, 1 : left
	// 8, 5, 2 : center
	// 9, 6, 3 : right

	int c = getCenter();
	if (c == 7 || c == 4 || c == 1) {
		return 0;
	//} else if (c == 8 || c == 5 || c == 2) {
	} else if (c == 9 || c == 6 || c == 3) {
		return getWidth();
	}

	// basic: center
	return getWidth()/2;
}

int CSVDST::getCenterY() {
	// 7, 8, 9 : top
	// 4, 5, 6 : center
	// 1, 2, 3 : bottom
	
	int c = getCenter();
	if (c == 7 || c == 8 || c == 9) {
		return 0;
	//} else if (c == 4 || c == 5 || c == 6) {
	} else if (c == 1 || c == 2 || c == 3) {
		return getHeight();
	}

	// basic: center
	return getHeight()/2;
}

int CSVDST::getLoop() {
	return dst[15];
}

int CSVDST::getTimer() {
	return dst[16];
}

int CSVDST::getOP1() {
	return dst[17];
}

int CSVDST::getOP2() {
	return dst[18];
}

int CSVDST::getOP3() {
	return dst[19];
}

bool CSVDST::checkOP() {
	return (CSVOption::getOption(getOP1()) && CSVOption::getOption(getOP2()) && CSVOption::getOption(getOP3()));
}

void CSVDST::setDST(TCHAR *args[]) {	
	type = CSVElement::getTypeInt(args[0]+5);

	for (int i=1; i<20 && args[i]; i++) {
		this->dst[i-1] = _wtoi(args[i]);
	}
}

void CSVDST::setDSTX(int x) {
	dst[2] = x;
}

void CSVDST::setDSTY(int y) {
	dst[3] = y;
}

void CSVDST::setDSTWidth(int width) {
	dst[4] = width;
}

void CSVDST::setDSTHeight(int height) {
	dst[5] = height;
}

bool CSVDST::isContains(int x, int y) {
	return (x >= getX() && x <= getX2() &&
		y >= getY() && y <= getY2());
}