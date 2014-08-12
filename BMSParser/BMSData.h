// from https://github.com/kuna/BMSJava/blob/master/BMSData.java
// by kuna
#pragma once
#include "Stdafx.h"
#include "BMSKeyData.h"

#define GENERAL_BPM 130.0

class BMSData {
private:

public:
	~BMSData();

	int player;
	std::wstring title;
	std::wstring subtitle;
	std::wstring genre;
	std::wstring artist;
	double BPM;
	int playlevel;
	int difficulty;
	int rank;
	int total;
	int volwav;
	std::wstring stagefile;
	std::wstring str_wav[1322];
	std::wstring str_bg[1322];
	double str_bpm[1322];
	double str_stop[1322];
	boolean LNObj[1322];
	
	int key;										// custom; 5-7-10-14

	int beat_numerator[1600];		// MAXIMUM_BEAT
	int beat_denominator[1600];		// MAXIMUM_BEAT
	
	std::vector<BMSKeyData*> bmsdata;	// MAXIMUM_OBJECT (Trans object+hit object+STOP+BPM)
	std::vector<BMSKeyData*> bgadata;	// BGA
	std::vector<BMSKeyData*> bgmdata;	// BGM
	int notecnt;
	double time;
	
	// bms file specific data
	std::wstring hash;
	std::wstring path;
	std::wstring dir;
	std::wstring preprocessCommand;


public:
	double getBeatFromTime(int millisec);
	double getTimeFromBeat(std::vector<BMSKeyData> &bpmarr, double beat);
	double getBPMFromBeat(double beat);
	int getBeatNumerator(int beat);
	int getBeatDenominator(int beat);
	void setNumeratorFit(BMSKeyData bkd, int fit);
	double getBeatLength(int beat);
	double getNotePosition(int beatHeight, int beat, int numerator);
	double getNotePosition(int beatHeight, int beat, double decimal);
	double getNotePositionWithBPM(int beatHeight, const std::vector<BMSKeyData*> &bpmarr, double b);

	
	double getBPM(int val) {
		return str_bpm[val];
	}
	double getSTOP(int val) {
		return str_stop[val];
	}
	std::wstring getBGA(int val) {
		if (str_bg[val].length() == 0)
			return L"";
		return str_bg[val];
	}
	std::wstring getWAV(int val) {
		if (str_wav[val].length() == 0)
			return L"";
		return str_wav[val];
	}
	void setBGA(int val, std::wstring s) {
		str_bg[val] = s;
	}
	void setWAV(int val, std::wstring s) {
		str_wav[val] = s;
	}

	void fillNotePosition(std::vector<BMSKeyData*> &arr, int beatHeight, boolean considerBPM);
	BMSKeyData* getBeatFromPosition(int beatHeight, int sy);
	BMSKeyData* getPairLN(BMSKeyData *lnData);
	bool isNoteAlreadyExists(int beat, int numerator, int channel, int layer);
	BMSKeyData* getNote(int beat, int numerator, int channel, int layer);
	bool removeNote(int beat, int numerator, int channel, int layer);
	bool removeNote(BMSKeyData *bkd);
	void removeChannel(std::vector<int> &channels);
	bool is5Key();
	bool is10Key();
	int checkKey();
	int getTotal();
	void addNote(BMSKeyData *bkd);
	void dispose();
	void convertLNOBJ();

	void setTimemark();
};