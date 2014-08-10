// from https://github.com/kuna/BMSJava/blob/master/BMSData.java
// by kuna
#pragma once
#include "Stdafx.h"

class BMSData;	// redeclaration; at cpp

class BMSKeyData {
	double beat;
	int numerator;
	
	double value;
	double time;
	
	double posy;
	int attr;

	int convertPlayerKey(int val) {
		if (val >= 1 && val <= 5) {
			return val;
		} else if (val == 6) {
			return 8;
		} else if (val == 7) {
			return 9;
		} else if (val == 8) {
			return 6;
		} else {
			return -1;
		}
	}
public:
	int key;
	int layernum;	// used such as BGM
	bool isLNfirst;

	BMSKeyData();	// isLNfirst: 0
	void setBeat(double beat, BMSData *bd);
	void setBeat(double beat);
	
	bool isLNFirst() {
		return isLNfirst;
	}
	
	double getBeat() {
		return beat;
	}

	int getChannel() {
		return key;
	}
	
	void setChannel(int val) {
		key = val;
	}

	double getValue() {
		return value;
	}
	
	void setValue(double val) {
		value = val;
	}
	
	void setValue(int val) {
		value = val;
	}
	
	int getAttr() {
		return attr;
	}
	
	void setAttr(int val) {
		attr = val;
	}
	
	bool isProcessed() {
		return (attr != 0);
	}
	
	void setProcessed() {
		attr = 1;
	}
	
	void setPosY(double val) {
		posy = val;
	}
	
	double getPosY()  {
		return posy;
	}
	
	int getPosY(double precision)  {
		return (int)(posy*precision);
	}
	
	void set1PKey(int val) { // 1~7: normal key, 8:SC
		int r = convertPlayerKey(val);
		if (r > 0) {
			key = 16+r;
		}
	}
	
	void set2PKey(int val) { // 1~7: normal key, 8:SC
		int r = convertPlayerKey(val);
		if (r > 0) {
			key = 32+r;
		}
	}
	
	void set1PTransKey(int val) { // 1~7: normal key, 8:SC
		int r = convertPlayerKey(val);
		if (r > 0) {
			key = 48+r;
		}
	}
	
	void set2PTransKey(int val) { // 1~7: normal key, 8:SC
		int r = convertPlayerKey(val);
		if (r > 0) {
			key = 64+r;
		}
	}
	
	void set1PLNKey(int val) { // 1~7: normal key, 8:SC
		int r = convertPlayerKey(val);
		if (r > 0) {
			key = 80+r;
		}
	}
	
	void set2PLNKey(int val) { // 1~7: normal key, 8:SC
		int r = convertPlayerKey(val);
		if (r > 0) {
			key = 96+r;
		}
	}
	
	int getKey() {
		return key;
	}
	
	int getNumerator() {
		return numerator;
	}
	
	void setNumerator(int val) {
		numerator = val;
	}
	
	double getTime() {
		return time;
	}
	
	int getLayerNum() {
		return layernum;
	}

	bool isBGMChannel() {
		return (key == 1);
	}

	void setBGMChannel(int layer) {
		key = 1;
		layernum = layer;
	}
		
	bool isBPMChannel() {
		return (key == 3);
	}

	void setBPMChannel() {
		key = 3;
	}
	
	bool isBPMExtChannel() {
		return (key == 8);
	}

	void setBPMExtChannel() {
		key = 8;
	}
	
	bool isBGAChannel() {
		return (key == 4);
	}

	void setBGAChannel() {
		key = 4;
	}
	
	bool isPoorChannel() {
		return (key == 6);
	}

	void setPoorChannel() {
		key = 6;
	}
	
	bool isBGALayerChannel() {
		return (key == 7);
	}

	void setBGALayerChannel() {
		key = 7;
	}
	
	bool isSTOPChannel() {
		return (key == 9);
	}

	void setSTOPChannel() {
		key = 9;
	}
	
	bool is1PChannel() {
		return (key > 16 && key < 32);
	}

	bool is2PChannel() {
		return (key > 32 && key < 48);
	}
	
	bool is1PTransChannel() {
		return (key > 48 && key < 64);
	}

	bool is2PTransChannel() {
		return (key > 64 && key < 80);
	}
	
	bool is1PLNChannel() {
		return (key > 80 && key < 96);
	}
	
	bool is2PLNChannel() {
		return (key > 96 && key < 112);
	}

	void setTime(int time) {
		this->time = time;
	}
	
	int getKeyNum() {
		if (!is1PChannel() && !is2PChannel() && !is1PLNChannel() && !is2PLNChannel() && !is1PTransChannel() && !is2PTransChannel())
			return -1;
		
		int rk = key % 16;
		if (rk == 7)
			return -1;	// we won't support this
		else if (rk == 8)
			return 6;
		else if (rk == 9)
			return 7;
		else if (rk == 6)
			return 8;	// scratch
		else
			return rk;
	}
};


// compare
class CompareKeyData : std::unary_function< BMSKeyData*, bool >
{
public:
    bool operator()(BMSKeyData* lhs, BMSKeyData* rhs) const
    {
		return lhs->getBeat() < rhs->getBeat();
    }
};