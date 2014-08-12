#include "Stdafx.h"
#include "BMSData.h"
#include "BMSUtil.h"
#include <algorithm>	// sort

BMSData::~BMSData() {
	dispose();
}

double BMSData::getBeatFromTime(int millisec) {
	double bpm = BPM;
	double beat = 0;
		
	// for more precision set vals as Double
	double time = 0;
	double newtime = 0;
		
	for (int i=0; i<bmsdata.size(); i++) {
		BMSKeyData *d = bmsdata[i];
			
		// Beat is effected by midi length ... check midi length
		while (d->getBeat() > (int)beat+1) {
			newtime = time + ((int)beat+1-beat) * (1.0f/bpm*60*4) * 1000 * getBeatLength((int) beat);	// millisec
			if (newtime >= millisec) {
				return beat + (millisec-time)*(bpm/60000/4.0f)/getBeatLength((int) beat);
			}
				
			time = newtime;
			beat = (int)beat+1;
		}
			
		if (d->isSTOPChannel()) {	// STOP
			time += d->getValue() * 1000;
			if (time >= millisec)
				return beat;
			continue;
		}
			
		if (d->isBPMChannel() || d->isBPMExtChannel()) {	// BPM
			newtime = time + (d->getBeat()-beat) * (1.0f/bpm*60*4) * 1000 * getBeatLength((int) beat);	// millisec
			if (newtime >= millisec) {
				return beat + (millisec-time)*(bpm/60000/4.0f)/getBeatLength((int) beat);
			}
				
			beat = d->getBeat();
			bpm = d->getValue();
			time = newtime;
		}
	}
		
	// get beat from left beat
	// BUG FIXED: 140713 140714
	while (true) {
		newtime = time + ((int)beat+1-beat) * (1.0f/bpm*60*4) * 1000 * getBeatLength((int) beat);	// millisec
		if (newtime >= millisec) {
			return beat + (millisec-time)*(bpm/60000/4.0f)/getBeatLength((int) beat);
		}
			
		time = newtime;
		beat = (int)beat+1;
	}
}

double BMSData::getTimeFromBeat(std::vector<BMSKeyData> &bpmarr, double beat) {
	double bpm = BPM;
	int nbeat = 0;
	double decimal = 0;
	double time = 0;
		
	for (int i=0; i<bpmarr.size(); i++) {
		BMSKeyData &b = bpmarr[i];
		while (nbeat < (int)b.getBeat() && nbeat < (int)beat) {
			time += (1.0 / bpm * 60) * getBeatLength(nbeat) * (1-decimal);
			decimal = 0;
			nbeat++;
		}
			
		// new BPM applies first
		if (nbeat == (int)b.getBeat()) {
			decimal = fmod(b.getBeat(), 1);
			time += (1.0 / bpm * 60) * getBeatLength(nbeat) * decimal;
			bpm  = b.getValue();
		}
			
		if (nbeat == (int)beat)
			break;
	}
		
	// calculate left one
	while (nbeat < (int)beat) {
		time += (1.0 / bpm * 60) * getBeatLength(nbeat) * (1-decimal);
		decimal = 0;
		nbeat++;
	}

	time += (1.0 / bpm * 60) * getBeatLength(nbeat) * fmod(beat, 1);
		
	return time*4;
}

double BMSData::getBPMFromBeat(double beat) {
	double bpm = BPM;
	for (int i=0; i<bmsdata.size(); i++) {
		if (bmsdata[i]->getBeat() > beat)
			break;
		if (bmsdata[i]->getKey() == 3 || bmsdata[i]->getKey() == 8)
			bpm = bmsdata[i]->getValue();
	}
	return bpm;
}

double BMSData::getNotePositionWithBPM(int beatHeight, const std::vector<BMSKeyData*> &bpmarr, double b) {
	// may need lots of calculation
	int beat = (int)b;
	double decimal = fmod(b, 1);
	double nbpm = BPM;
		
	int beatNum = 0;
	double beatDecimal = 0;
	double r = 0;
	for (int bpmidx=0; bpmidx<bpmarr.size(); bpmidx++) {
		BMSKeyData *bpm = bpmarr[bpmidx];

		while (beatNum < (int)bpm->getBeat() && beatNum < beat) {
			r += beatHeight * getBeatLength(beatNum) * (1-beatDecimal) * nbpm / GENERAL_BPM;
			beatNum++;
			beatDecimal = 0;
		}
			
		// new BPM applies first (in case of xx.0)
		// PATCHED BUG: 140713 (2)
		boolean BPMCheck = false;
		if (beatNum == (int)bpm->getBeat() && b > bpm->getBeat()) {
			r += beatHeight * getBeatLength(beatNum) * (fmod(bpm->getBeat(), 1) - beatDecimal) * nbpm / GENERAL_BPM;
			beatDecimal = fmod(bpm->getBeat(), 1);
			nbpm = bpm->getValue();
			BPMCheck = true;
		}
			
		if (beatNum == beat && !BPMCheck)
			break;
	}
		
	// calculate left beat
	while (beatNum < beat) {
		r += beatHeight * getBeatLength(beatNum) * (1-beatDecimal) * nbpm / GENERAL_BPM;
		beatNum++;
		beatDecimal = 0;
	}
	r += beatHeight * getBeatLength(beatNum) * nbpm / GENERAL_BPM * (decimal - beatDecimal);
	return r;
}

double BMSData::getNotePosition(int beatHeight, int beat, double decimal) {
	int beatNum = 0;
	double r = 0;
	while (beatNum < beat) {
		// calculate new sbeatNum
		r += beatHeight * getBeatLength(beatNum);
		beatNum++;
	}
		
	r += beatHeight * getBeatLength(beatNum)
			* decimal;
	return r;
}

int BMSData::getBeatNumerator(int beat) {
	if (beat_denominator[beat] == 0)
		return 4;
	else
		return beat_numerator[beat];
}

int BMSData::getBeatDenominator(int beat) {
	if (beat_denominator[beat] == 0)
		return 4;
	else
		return beat_denominator[beat];
}

void BMSData::setNumeratorFit(BMSKeyData bkd, int fit) {
	if (fit == 0)
		return;
	int divnum = 192 / fit;
	bkd.setNumerator(bkd.getNumerator() - bkd.getNumerator()%divnum);
	bkd.setBeat((int)(bkd.getBeat()) + (double)bkd.getNumerator()
		/ (192 * getBeatNumerator((int) (bkd.getBeat())) / getBeatDenominator((int) (bkd.getBeat()))));
}
	
double BMSData::getBeatLength(int beat) {
	if (beat_denominator[beat] == 0)
		return 1;	// default
	return (double)beat_numerator[beat] / beat_denominator[beat];
}
	
double BMSData::getNotePosition(int beatHeight, int beat, int numerator) {
	int beatNum = 0;
	double r = 0;
	while (beatNum < beat) {
		// calculate new sbeatNum
		r += beatHeight * getBeatLength(beatNum);
		beatNum++;
	}
		
	r += beatHeight * getBeatLength(beatNum)
			* numerator / (192 * getBeatNumerator(beatNum) / getBeatDenominator(beatNum));
	return r;
}





void BMSData::fillNotePosition(std::vector<BMSKeyData*> &arr, int beatHeight, boolean considerBPM) {
	if (!considerBPM) {
		for (int i=0; i<arr.size(); i++) {
			BMSKeyData *bkd = arr[i];
			bkd->setPosY( getNotePosition(beatHeight, (int)bkd->getBeat(), fmod(bkd->getBeat(), 1)) );
		}
	} else {
		std::vector<BMSKeyData*> bpmarr = BMSUtil::ExtractChannel(bmsdata, 3);	// BPM channel
		for (int i=0; i<arr.size(); i++) {
			BMSKeyData *bkd = arr[i];
			bkd->setPosY( getNotePositionWithBPM(beatHeight, bpmarr, bkd->getBeat()) );
		}
	}
}
	
BMSKeyData *BMSData::getBeatFromPosition(int beatHeight, int sy) {
	BMSKeyData *bk = (BMSKeyData*)malloc(sizeof(BMSKeyData));
	memset(bk, 0, sizeof(BMSKeyData));
		
	int beatNum = 0;
	int y = 0, by = 0;
	while (y < sy) {
		// calculate new sbeatNum
		by = y;
		y += (int) (beatHeight * getBeatLength(beatNum));
		beatNum++;
	}
		
	y = by;
	beatNum--;
		
	double beat = beatNum;
	beat += (double)(sy-y) / beatHeight / getBeatLength(beatNum);
		
	bk->setBeat(beat);
	bk->setNumerator(fmod(beat, 1) * 192 / getBeatDenominator(beatNum) * getBeatNumerator(beatNum));
		
	return bk;
}
	
BMSKeyData *BMSData::getPairLN(BMSKeyData *lnData) {
	// get another LN pair
	BMSKeyData* LNPair = 0;
	boolean returnAtNext = false;
		
	for (int i=0; i<bmsdata.size(); i++) {
		BMSKeyData *bkd = bmsdata[i];
		if (bkd->getChannel() != lnData->getChannel())
			continue;
			
		if (returnAtNext)
			return bkd;
			
		if (bkd == lnData) {
			if (LNPair)
				return LNPair;
			else
				returnAtNext = true;
		}
			
		if (LNPair)
			LNPair = 0;
		else
			LNPair = bkd;
	}
		
	return 0;	// No matching LN pair found.
}
	
bool BMSData::isNoteAlreadyExists(int beat, int numerator, int channel, int layer) {
	return (getNote(beat, numerator, channel, layer) != 0);
}
	
BMSKeyData* BMSData::getNote(int beat, int numerator, int channel, int layer) {
	if (channel == 1 /*BGM*/) {
		for (int i=0; i<bgmdata.size(); i++) {
			BMSKeyData &bkd = *bgmdata[i];
			if ((int)bkd.getBeat() == beat && bkd.getNumerator() == numerator && bkd.getLayerNum() == layer)
				return &bkd;
		}
			
		return 0;
	} else if (channel == 4 || channel == 6 || channel == 7) {
		for (int i=0; i<bgadata.size(); i++) {
			BMSKeyData &bkd = *bgadata[i];
			if ((int)bkd.getBeat() == beat && bkd.getNumerator() == numerator && bkd.getChannel() == channel)
				return &bkd;
		}
			
		return 0;
	} else {
		for (int i=0; i<bmsdata.size(); i++) {
			BMSKeyData &bkd = *bmsdata[i];
			if ((int)bkd.getBeat() == beat && bkd.getNumerator() == numerator && bkd.getChannel() == channel)
				return &bkd;
		}
			
		return 0;
	}
}
	
bool BMSData::removeNote(int beat, int numerator, int channel, int layer) {
	if (channel == 1 /*BGM*/) {
		for (int i=0; i<bgmdata.size(); i++) {
			BMSKeyData &bkd = *bgmdata[i];
			if ((int)bkd.getBeat() == beat && bkd.getNumerator() == numerator && bkd.getLayerNum() == layer) {
				bgmdata.erase(bgmdata.begin()+i);
				return true;
			}
		}
		return false;
	} else if (channel == 4 || channel == 6 || channel == 7) {
		for (int i=0; i<bgadata.size(); i++) {
			BMSKeyData &bkd = *bgadata[i];
			if ((int)bkd.getBeat() == beat && bkd.getNumerator() == numerator && bkd.getChannel() == channel) {
				bgadata.erase(bgadata.begin()+i);
				return true;
			}
		}
		return false;
	} else {
		for (int i=0; i<bmsdata.size(); i++) {
			BMSKeyData &bkd = *bmsdata[i];
			if ((int)bkd.getBeat() == beat && bkd.getNumerator() == numerator && bkd.getChannel() == channel) {
				bmsdata.erase(bmsdata.begin()+i);
				return true;
			}
		}
		return false;
	}
}
	
bool BMSData::removeNote(BMSKeyData* bkd) {
	for (int i=0; i<bmsdata.size(); i++) {
		if (bmsdata[i] == bkd) {
			bmsdata.erase(bmsdata.begin()+i);
			return true;
		}
	}
	for (int i=0; i<bgadata.size(); i++) {
		if (bgadata[i] == bkd) {
			bgadata.erase(bgadata.begin()+i);
			return true;
		}
	}
	for (int i=0; i<bgmdata.size(); i++) {
		if (bgmdata[i] == bkd) {
			bgmdata.erase(bgmdata.begin()+i);
			return true;
		}
	}
	return false;
}
	
void BMSData::removeChannel(std::vector<int> &channels) {
	/* in real, it doesn't removed but inserted into BGM channel! */
	for (int i=0; i<bmsdata.size(); i++) {
		int nc = bmsdata[i]->getChannel();
		boolean b = false;
		for (auto c = channels.begin(); c != channels.end(); ++c) {	// TODO c++? ++c?
			if (nc == *c) {
				b = true;
				break;
			}
		}
		if (b) {
			BMSKeyData *bkd = bmsdata[i];
			bmsdata.erase(bmsdata.begin()+i);
			bkd->setChannel(1);	//BGM
			bgmdata.push_back(bkd);
			i--;
			notecnt--;
		}
	}
	// sort again
	std::sort(bgmdata.begin(), bgmdata.end(), CompareKeyData());
}
	
bool BMSData::is5Key() {
	// check channel
	for (auto bkd = bmsdata.begin(); bkd != bmsdata.end(); ++bkd) {
		if ((*bkd)->getKeyNum() == 6 || (*bkd)->getKeyNum() == 7) {
			if ((*bkd)->is1PChannel() || (*bkd)->is1PLNChannel())
				return false;
		}
	}
		
	return true;
}
	
bool BMSData::is10Key() {
	// check channel
	for (auto bkd = bmsdata.begin(); bkd != bmsdata.end(); ++bkd) {
		if ((*bkd)->getKeyNum() == 6 || (*bkd)->getKeyNum() == 7) {
			return false;
		}
	}
		
	return true;
}
	
int BMSData::checkKey() {
	if (player == 1) {
		if (is5Key())
			key = 5;
		else
			key = 7;
	} else {
		if (is10Key())
			key = 10;
		else
			key = 14;
	}
		
	return key;
}
	
int BMSData::getTotal() {
	total = (int) (notecnt*0.16f + 160);
	return total;
}
	
void BMSData::addNote(BMSKeyData *bkd) {
	bmsdata.push_back(bkd);
}
	
void BMSData::dispose() {
	// memory release
	int i;
	for (i=0; i<bmsdata.size(); i++) {
		delete bmsdata[i];
	}
	for (i=0; i<bgadata.size(); i++) {
		delete bgadata[i];
	}
	for (i=0; i<bgmdata.size(); i++) {
		delete bgmdata[i];
	}
	bmsdata.clear();
	bgadata.clear();
	bgmdata.clear();
}
	
void BMSData::convertLNOBJ() {
	// for playing, this command is necessary
	// before sort this command, you must sort bmsdata array. (default status)
	// check LNOBJ command
	BMSKeyData *lnPrevObj[50];
		
	for (auto b=bmsdata.begin(); b!=bmsdata.end(); ++b) {
		if ((*b)->isSTOPChannel() || (*b)->isBPMChannel() || (*b)->isBPMExtChannel())
			continue;
			
		int o = (*b)->getKeyNum() + ((*b)->is1PChannel()?0:8);
			
		if (LNObj[(int)(*b)->getValue()]) {
			if (lnPrevObj[o] == 0)
				continue;	// ignores
				
			if ((*b)->is1PChannel()) {
				(*b)->set1PLNKey((*b)->getKeyNum());
				lnPrevObj[o]->set1PLNKey((*b)->getKeyNum());
			} else if ((*b)->is2PChannel()) {
				(*b)->set2PLNKey((*b)->getKeyNum());
				lnPrevObj[o]->set2PLNKey((*b)->getKeyNum());
			}
			lnPrevObj[o]->isLNfirst = true;
			notecnt--;	// LN needs 2 key data, so 1 discount to correct note number.
				
			lnPrevObj[o] = 0;
		} else {
			lnPrevObj[o] = *b;
		}
	}
}

// MUST USE AFTER PARSING & SORTING!
void BMSData::setTimemark() {
	double _bpm = BPM;		// BPM for parsing
	double _time = 0;		// time for parsing
	double _beat = 0;		// beat for parsing
		
	for (int i=0; i<bmsdata.size(); i++) {
		BMSKeyData *d = bmsdata[i];
			
		// check midi length
		while (d->getBeat() >= (int)_beat+1) {
			_time += ((int)_beat+1-_beat) * (1.0f/_bpm*60*4) * getBeatLength((int)_beat);
			_beat = (int)_beat+1;
		}
			
		_time += (d->getBeat() - _beat) * (1.0f / _bpm * 60 * 4) * getBeatLength((int)_beat);
		d->setTime(_time*1000);	// millisecond
			
		if (d->getKey() == 3 || d->getKey() == 8 )	// BPM
			_bpm = d->getValue();
		if (d->getKey() == 9)
			_time += d->getValue();
			
		_beat = d->getBeat();
	}
		
	// set current bms time
	time = _time;
		
		
	// LN note time..?
}