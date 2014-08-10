#include "Stdafx.h"
#include "BMSKeyData.h"
#include "BMSData.h"

BMSKeyData::BMSKeyData() {
	isLNfirst = false;
}
	
void BMSKeyData::setBeat(double beat, BMSData* bd) {
	// we need to reset numerator,
	// so BMSData is necessary
	this->beat =beat;
	numerator = (int) (192 * bd->getBeatLength((int)beat) * fmod(beat, 1));
}
	
void BMSKeyData::setBeat(double beat) {
	this->beat =beat;
}