#include "CSVRenderer.h"
#include "CSVSelectList.h"
#include "CSVData.h"		// for parent
#include "CSVNumber.h"
#include "CSVReader.h"		// for using const
#include "CSVBargraph.h"
#include "CSVConst.h"
#include "CSVOption.h"
#include "CSVButton.h"
#include "CSVSlider.h"

int CSVRenderer::cursorX;
int CSVRenderer::cursorY;
bool (*CSVRenderer::drawFunc)(int, TCHAR*, CSVSRC*, CSVDST*);
void (*CSVRenderer::notedrawFunc)();


void CSVRenderer::SetCurPos(int x, int y) {
	cursorX = x;
	cursorY = y;
}

void CSVRenderer::SetdrawFunc(bool (*pFunc)(int, TCHAR*, CSVSRC*, CSVDST*)) {
	drawFunc = pFunc;
}

void CSVRenderer::SetnotedrawFunc(void (*pFunc)()) {
	notedrawFunc = pFunc;
}

void CSVRenderer::drawBar(CSVElement *csvElement, int idx) {
	CSVDST cdst, cdst_next;
	CSVSRC csrc;

	// get Object
	CSVSelectSong *current = CSVSelectList::getSongData(idx - csvElement->parent->barCenter);

	// get DSTs
	if (!(csvElement->getDSTBar(&cdst, idx) && (csvElement->getDSTBar(&cdst_next, idx+1)))) {
		return;
	}
	
	// morphing X/Y coordination
	double a = CSVSelectList::moveProgress;
	cdst.setDSTX( cdst.getX() * (1-a)
		+ cdst_next.getX() * a );
	cdst.setDSTY( cdst.getY() * (1-a)
		+ cdst_next.getY() * a );

	// search BAR_BODY for SRC
	csvElement->srcNum = current->parent->index;
	csvElement->getSRC(&csrc);

	// draw bar
	drawFunc(csvElement->src[csvElement->srcNum]->getImgNum(), 0, &csrc, &cdst);

	// draw BAR_FLASH
	// - this will be implemented in drawElement
	
	// draw BAR_LEVEL
	CSVElement *cele_level = csvElement->parent->csvBarLevel;
	if (cele_level) {
		cele_level->srcNum = cele_level->dstNum = current->difficulty+1;	// 0 means NONE
		cele_level->dstOffsetX = cdst.getX();
		cele_level->dstOffsetY = cdst.getY();
		drawNumber(cele_level, current->level);
	}

	// draw BAR_LAMP
		
	// draw BAR_MY_LAMP
		
	// draw BAR_RIVAL_LAMP
		
	// draws BAR_TITLE
	for (int i=0; i<csvElement->parent->csvBarTitle.size(); i++) {
		CSVElement *cele_title = csvElement->parent->csvBarTitle[i];
		cele_title->dstOffsetX = cdst.getX();
		cele_title->dstOffsetY = cdst.getY();
		drawText(cele_title, current->path);
	}

	// restore everything to default
	csvElement->srcNum = 0;
}

void CSVRenderer::drawNumber(CSVElement *csvElement, int num) {
	// get DST first
	CSVDST cdst;
	if (csvElement->getDST(&cdst)) {
		// INFO
		// if src is duplication of 10
		// then align works
			
		// set startX, width per number
		int nWidth = cdst.getWidth();
		int orgX = cdst.getX();
		int sx = 0;
		int keta = csvElement->src[csvElement->srcNum]->getKeta();
		int numberSize = getNumberLength(num);	// how big this number is?
		int multiply = 10;	// basic
		CSVSRC *currentSRC = csvElement->src[csvElement->srcNum];

		if ((currentSRC->getdivX() * currentSRC->getdivY())%10 == 0) {
			// check align. left align is default
			if (currentSRC->getAlign() == 0) {			// right align
				sx = nWidth * (keta - numberSize);
			} else if (currentSRC->getAlign() == 2) {	// center align
				sx = nWidth * (keta - numberSize) / 2;
			}
			multiply = 10;
			sx += nWidth * numberSize;	// start drawing from right
		} else if ((currentSRC->getdivX() * currentSRC->getdivY()) % 11 == 0) {
			multiply = 11;
			sx += nWidth * keta;	// start drawing from right
		} else if ((currentSRC->getdivX() * currentSRC->getdivY()) % 24 == 0) {
			multiply = 24;
			sx += nWidth * keta;	// start drawing from right
		}

		// draw number
		CSVSRC csrc;
			
		// check negative number
		bool negative = (num<0);
		if (multiply == 24)
			sx += nWidth;		// for drawing sign
		else
			negative = false;	// negative is only for 24
		// convert negative number to positive
		if (num < 0) {
			num *= -1;
		}

		for (int i=0; i<keta; i++) {
			int currentNum = num%10;
			if (i >= numberSize) {
				if (multiply == 10)
					continue;
				else
					currentNum = 10;
			}
			csvElement->getNUMSRC(&csrc, currentNum, negative);
			sx -= nWidth;
			// set cdst again
			cdst.setDSTX(sx+orgX);
			drawFunc(currentSRC->getImgNum(), 0, &csrc, &cdst);

			num /= 10;
		}

		// if multiply == 24, then draw sign
		if (multiply == 24) {
			csvElement->getNUMSRC(&csrc, 0, negative, true);
			sx -= nWidth;
			cdst.setDSTX(sx+orgX);
			drawFunc(currentSRC->getImgNum(), 0, &csrc, &cdst);
		}
	}
}

void CSVRenderer::drawNumber(CSVElement *csvElement) {
	int num = CSVNumber::getNumber(csvElement->src[csvElement->srcNum]->getNum());
	drawNumber(csvElement, num);
}

void CSVRenderer::drawElement(CSVElement *csvElement) {
	int type = csvElement->getType();
	CSVSRC *currentSRC = csvElement->src[csvElement->srcNum];

	if (type == CSVReader::CSVTYPE_IMAGE || 
		type == CSVReader::CSVTYPE_JUDGELINE) {
		// in case of IMG
		drawImage(csvElement);
	} else if (type == CSVReader::CSVTYPE_BUTTON) {
		// similar to IMG, but considers click
		CSVSRC csrc;
		CSVDST cdst;
		int idx = csvElement->getSRCIndex();
		if (csvElement->getCurrentSRC()->getClick() == 1) {
			idx += CSVButton::GetClick(csvElement->getCurrentSRC()->getButtonType());
		}
		csvElement->getSRCFromIdx(&csrc, idx);
		
		// INFO: we won't check SRC OP
		if (csvElement->getDST(&cdst)) {
			// convert negative height/width
			// cause CSV won't support this
			if (cdst.getWidth() < 0) {
				cdst.dst[2] = cdst.getX() + cdst.getWidth();
				cdst.dst[4] = -cdst.getWidth();
			}
			if (cdst.getHeight() < 0) {
				cdst.dst[3] = cdst.getY() + cdst.getHeight();
				cdst.dst[5] = -cdst.getHeight();
			}

			// draw image
			drawFunc(csvElement->src[csvElement->srcNum]->getImgNum(), 0, &csrc, &cdst);
		
			// check ONMOUSE
			// (draw onmouse first)
			CSVElement *onmouse = csvElement->getRelatedElement();
			if (onmouse && isCursorInside(&cdst)) {
				drawImage(onmouse);
			}
		}
	} else if (type == CSVReader::CSVTYPE_TEXT) {
		// draw text
		drawText(csvElement);
	} else if (type == CSVReader::CSVTYPE_NUMBER) {
		// draw Number
		drawNumber(csvElement);
	} else if (type == CSVReader::CSVTYPE_BARGRAPH) {
		CSVSRC csrc;
		CSVDST cdst;

		csvElement->getSRC(&csrc);
		if (csvElement->getDST(&cdst)) {
			if (currentSRC->getMuki() == 0) {
				// change width
				cdst.setDSTWidth(cdst.getWidth() * CSVBargraph::getVal(currentSRC->getNum()));
			} else {
				// change height
				cdst.setDSTHeight(cdst.getHeight() * CSVBargraph::getVal(currentSRC->getNum()));
			}
			
			// convert negative height/width
			// cause CSV won't support this
			if (cdst.getWidth() < 0) {
				cdst.dst[2] = cdst.getX() + cdst.getWidth();
				cdst.dst[4] = -cdst.getWidth();
			}
			if (cdst.getHeight() < 0) {
				cdst.dst[3] = cdst.getY() + cdst.getHeight();
				cdst.dst[5] = -cdst.getHeight();
			}

			drawFunc(currentSRC->getImgNum(), 0, &csrc, &cdst);
		}
	} else if (type == CSVReader::CSVTYPE_GROOVEGUAGE) {
		// TODO: change algorithm for graph (blinking effect)
		// get guage
		int guage = CSVNumber::getNumber(CSVNumConst::PLAYER_1P_GUAGE);
		int addx = currentSRC->getOP1();
		int addy = currentSRC->getOP2();

		// draw 50
		CSVSRC csrc;
		CSVDST cdst;

		// TODO: if guage has no Alphablend ...? -> change it with preload...?
		// TODO: if 2P plays...?
		if (csvElement->getDST(&cdst)) {
			int orgX = cdst.getX();
			int orgY = cdst.getY();

			bool blink;

			for (int i=0; i<guage; i+=2) {
				// blinking effect - display faint when true
				blink = false;
				if (i != 0 && i != guage-2) {
					if (i == guage - 4) {
						if (rand() % 100 < 70)
							blink = true;
					} else if (i == guage - 6) {
						if (rand() % 100 < 50)
							blink = true;
					}
				}

				if (CSVOption::getOption(CSVOptionConst::PLAYER1_NORMAL_GUAGE)) {
					if (i >= 80) {
						if (!blink)
							csvElement->getSRCFromIdx(&csrc, 0);	// survival guage color
						else
							csvElement->getSRCFromIdx(&csrc, 2);
					} else {
						if (!blink)
							csvElement->getSRCFromIdx(&csrc, 1);
						else
							csvElement->getSRCFromIdx(&csrc, 3);
					}
				} else if (CSVOption::getOption(CSVOptionConst::PLAYER1_HARD_GUAGE)) {
					if (!blink)
						csvElement->getSRCFromIdx(&csrc, 0);
					else
						csvElement->getSRCFromIdx(&csrc, 2);
				}
				
				cdst.setDSTX(orgX);
				cdst.setDSTY(orgY);

				drawFunc(currentSRC->getImgNum(), 0, &csrc, &cdst);
				orgY += addy;
				orgX += addx;
			}
			for (int i=guage; i<100; i+=2) {
				if (CSVOption::getOption(CSVOptionConst::PLAYER1_NORMAL_GUAGE)) {
					if (i >= 80)
						csvElement->getSRCFromIdx(&csrc, 2);	// survival guage color
					else
						csvElement->getSRCFromIdx(&csrc, 3);
				} else if (CSVOption::getOption(CSVOptionConst::PLAYER1_HARD_GUAGE)) {
					csvElement->getSRCFromIdx(&csrc, 2);
				}
				
				cdst.setDSTX(orgX);
				cdst.setDSTY(orgY);

				drawFunc(currentSRC->getImgNum(), 0, &csrc, &cdst);

				orgY += addy;
				orgX += addx;
			}
		}
	} else if (type == CSVReader::CSVTYPE_NOWJUDGE_1P) {
		// TODO: support 2P
		// nowcombo is drawed with NOWJUDGE, so we won't make case for that.
		CSVSRC src_nowjudge;
		CSVDST dst_nowjudge, dst_nowcombo;

		int judge = 5;

		// TODO: set judge case
		csvElement->srcNum = csvElement->dstNum = judge;

		csvElement->getSRC(&src_nowjudge);
		if (csvElement->getDST(&dst_nowjudge)) {
			// get nowcombo
			CSVElement *csvCombo = csvElement->parent->csvNowCombo1P;
			// TODO: set judge case
			csvCombo->srcNum = csvCombo->dstNum = judge;

			int width = 0;
			bool comboDrawable = false;
			int combo = 123;	// TODO: set combo

			if (csvCombo->getDST(&dst_nowcombo)) {
				comboDrawable = true;
				// src op1 is noshift (dstOffset is 0)
				if (csvCombo->getCurrentSRC()->getOP1() == 0) {
					// get total number size
					width = dst_nowcombo.getWidth() * getNumberLength(combo);
				}
			}

			// draw image
			csvElement->dstOffsetX = -width/2;
			drawImage(csvElement);
			if (comboDrawable) {
				csvCombo->dstOffsetX = dst_nowjudge.getX()-width/2;
				csvCombo->dstOffsetY = dst_nowjudge.getY();
				drawNumber(csvCombo, combo);
				// it effects to next getDST, so init them
				csvCombo->dstOffsetX = 0;
				csvCombo->dstOffsetY = 0;
			}
			csvElement->dstOffsetX = 0;
		}
	} else if (type == CSVReader::CSVTYPE_SLIDER) {
		// muki - 0: top, 1:right, 2:bottom, 3:left
		CSVSRC *src = csvElement->getCurrentSRC();
		int muki = src->getSliderMuki();
		int offsetX, offsetY;
		offsetX = offsetY = CSVSlider::getSliderValue(src->getSliderType())
			* src->getSliderRange();
		switch (muki) {
		case 0:
			offsetX = 0;
			offsetY = src->getSliderRange() - offsetY;
			break;
		case 1:
			offsetY = 0;
			break;
		case 2:
			offsetX = 0;
			break;
		case 3:
			offsetX = src->getSliderRange() - offsetX;
			offsetY = 0;
			break;
		}
		csvElement->dstOffsetX = offsetX;
		csvElement->dstOffsetY = offsetY;
		drawImage(csvElement);
	} else if (type == CSVReader::CSVTYPE_BAR_BODY) {
		// calculate moving first
		CSVSelectList::CalculateMoving();

		// call drawbar with current argument
		for (int i=0; i<30; i++) {
			drawBar(csvElement, i);
		}

		// draw BAR_FLASH
		CSVSRC csrc;
		CSVDST cdst;
		csvElement->parent->csvFlash->getSRC(&csrc);
		if (csvElement->getDSTBar(&cdst, csvElement->parent->barCenter)) {
			csvElement->parent->csvFlash->dstOffsetX = cdst.getX();
			csvElement->parent->csvFlash->dstOffsetY = cdst.getY();
			drawImage(csvElement->parent->csvFlash);
		}
	} else if (type == CSVReader::CSVTYPE_NOTE) {
		// draw note/lnnote/mine/line
		if (notedrawFunc) {
			notedrawFunc();
		}
	}
}

void CSVRenderer::drawImage(CSVElement *csvElement) {
	CSVSRC csrc;
	CSVDST cdst;

	// test code
	csvElement->getSRC(&csrc);
	//if (csrc.src[1] == 3)
	//	csvElement->getSRCFromIdx(&csrc, 6);
		
	// INFO: we won't check SRC OP
	if (csvElement->getDST(&cdst)) {
		drawImage(csvElement, &csrc, &cdst);
	}
}

void CSVRenderer::drawImage(CSVElement *csvElement, CSVSRC *src, CSVDST *dst) {
	// convert negative height/width
	// cause CSV won't support this
	if (dst->getWidth() < 0) {
		dst->dst[2] = dst->getX() + dst->getWidth();
		dst->dst[4] = -dst->getWidth();
	}
	if (dst->getHeight() < 0) {
		dst->dst[3] = dst->getY() + dst->getHeight();
		dst->dst[5] = -dst->getHeight();
	}

	// draw image
	drawFunc(csvElement->src[csvElement->srcNum]->getImgNum(), 0, src, dst);
}

void CSVRenderer::drawText(CSVElement *csvElement, TCHAR *text) {
	CSVDST cdst;
	if (csvElement->getDST(&cdst)) {
		drawFunc(0, text, csvElement->src[csvElement->srcNum], &cdst);
	}
}

void CSVRenderer::drawText(CSVElement *csvElement) {
	// TODO... need to implement CSVText
	drawText(csvElement, L"TEST");
}

void CSVRenderer::drawNote(CSVElement *csvElement, int key, int yPos) {
	csvElement->dstNum = csvElement->srcNum = key;
	csvElement->dstOffsetY = -yPos;
	drawImage(csvElement);
}

void CSVRenderer::drawLNNote(CSVElement *csvNote, CSVElement *csvStart, CSVElement *csvBody, CSVElement *csvEnd, int key, int syPos, int eyPos) {
	CSVSRC src_start, src_body, src_end;
	CSVDST cdst;

	csvNote->dstNum = csvNote->srcNum = 
		csvStart->dstNum = csvStart->srcNum = 
		csvBody->dstNum = csvBody->srcNum = 
		csvEnd->dstNum = csvEnd->srcNum = key;

	csvStart->getSRC(&src_start);
	csvBody->getSRC(&src_body);
	csvEnd->getSRC(&src_end);
		
	if (csvNote->getDST(&cdst)) {
		// set note position and draw
		int h = cdst.getHeight();
		int y = cdst.getY();

		cdst.setDSTHeight(eyPos - syPos);
		cdst.setDSTY(y - eyPos);
		drawImage(csvBody, &src_body, &cdst);
		
		cdst.setDSTHeight(h);
		cdst.setDSTY(y - syPos);
		drawImage(csvStart, &src_start, &cdst);

		cdst.setDSTY(y - eyPos);
		drawImage(csvEnd, &src_end, &cdst);
	}
}

void CSVRenderer::drawLine(CSVElement *csvElement, int yPos) {
	csvElement->dstOffsetY = -yPos;
	drawImage(csvElement);
}

// TODO: need to convert pos (when width or height is negative)
bool CSVRenderer::isCursorInside(CSVDST *dst) {
	return dst->isContains(cursorX, cursorY);
}

int CSVRenderer::getNumberLength(int num) {
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