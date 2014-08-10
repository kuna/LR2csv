#include "Stdafx.h"
#include "BMSParser.h"
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>


int BMSParser::LNType;
int BMSParser::LNprevVal[120];
BMSKeyData* BMSParser::LNKey[120];
int BMSParser::BGALayerCount[1322];

int BMSParser::BMSParseMode;

int BMSParser::BMSKeyCount[14];
int BMSParser::randomStackCnt;
int BMSParser::randomVal[256];		// Maximum stack: 256
int BMSParser::condition[256];

bool BMSParser::LoadBMSFile(std::wstring path, BMSData bd) {
	BMSUtil::Log(L"BMSParser", L"Loading BMS File ... " + path);

	BYTE *data = 0;
	if (!ReadBMSFile(path.c_str(), &data)) {
		if (data)
			free(data);
		return false;
	}
	bool r = LoadBMSFile(data, bd);
	free(data);
	return r;
}

bool BMSParser::LoadBMSFile(BYTE *data, BMSData bd) {
	BMSUtil::Log(L"BMSParser", L"checking locale...");
		
	// just process data
	bd.hash = BMSUtil::GetHash(data, strlen((char*)data));
		
	// check locale
	std::wstring locale = BMSUtil::CheckEncoding(data);
	std::wstring wstr;
	    
	if (locale.compare(L"ANSI") == 0) {
		try {
		    // attempt SHIFT_JIS first
		    // we also can use https://github.com/hnakamur/sjis-check,
		    // but it may be too much slow in mobile device.
			wstr = boost::locale::conv::to_utf<wchar_t>((char*)data, "CP932");
			int examineLen = (wstr.length()>1000)?1000:wstr.length();
			for (int i=0; i< examineLen ; i++) {
				if (wstr[i] >= 44032 && wstr[i] <= 55203) {
					// EUC-KR encoding
					wstr = boost::locale::conv::to_utf<wchar_t>((char*)data, "CP949");
					break;
				}
			}
		} catch (...) {
		    BMSUtil::Log(L"BMSParser", L"Unsupported Encoding Exception");
		    return false;
		}
	} else {
		// utf8?
	    try {
			wstr = boost::locale::conv::to_utf<wchar_t>((char*)data, "UTF-8");
		} catch (...) {
		    BMSUtil::Log(L"BMSParser", L"Unsupported Encoding Exception");
		    return false;
		}
	}
	
	// init before parsing
	for (int i=0; i<14; i++) {
		BMSKeyCount[i] = 0;
	}
		
	return ParseBMSData(wstr, bd);
}

bool BMSParser::ReadBMSFile(const TCHAR *path, BYTE **out) {
	FILE *fp;
	fp = _wfopen(path, L"rb");
	if (!fp) return false;

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	// just read file
	*out = (BYTE*)malloc(fileSize + 10);
	fread(*out, 1, fileSize, fp);

	fclose(fp);
	return true;
}

bool BMSParser::ParseBMSData(std::wstring &data, BMSData bd) {
	// init
	memset(BGALayerCount, 0, sizeof(BGALayerCount));
		
	bd.notecnt = 0;
	bd.total = 0;
	bd.rank = 3;	// EASY is default;
	bd.title = L"";
	bd.subtitle = L"";
	bd.genre = L"";
	bd.artist = L"";
	bd.stagefile = L"";
	LNType = 1;	// 1 is default
	for (int i=0; i<1322; i++)
		bd.LNObj[i] = false;
	// clear bmskeydata
	bd.dispose();

	boost::algorithm::replace_all(data, L"\r\n", L"\n");
	std::vector<std::wstring> lines;
	boost::split(lines, data, boost::is_any_of(L"\n"));

	// do basic process (trim)
	for (int i=0; i<lines.size(); i++) {
		boost::trim(lines[i]);
	}

	// preprocess line ...
	for (int i=0; i<lines.size(); i++) {
		PreProcessBMSLine(lines[i], bd);
	}
		
	// process line...
	for (int i=0; i<lines.size(); i++) {
		ProcessBMSLine(lines[i], bd);
	}
		
	// sort data
	std::sort(bd.bmsdata.begin(), bd.bmsdata.end(), CompareKeyData());
	std::sort(bd.bgadata.begin(), bd.bgadata.end(), CompareKeyData());
	std::sort(bd.bgmdata.begin(), bd.bgmdata.end(), CompareKeyData());
		
	// when difficulty is not setted,
	// process automatic difficulty set
	if (bd.difficulty == 0) {
		// basically it is 5
		bd.difficulty = 5;
			
		std::wstring _title = bd.title;
		std::wstring _path = bd.path;
		boost::to_upper(_title);
		boost::to_upper(_path);
#define FOUND(str,x) ((str).find(x)!=std::wstring::npos)
		if (FOUND(_title, L"BEGINNER") || FOUND(_path, L"BEGINNER") ||
				FOUND(_title, L"LIGHT") || FOUND(_path, L"LIGHT") ||
				FOUND(_title, L"EASY") || FOUND(_path, L"EASY")) {
			bd.difficulty = 1;
		}
		if (FOUND(_title, L"NORMAL") || FOUND(_path, L"NORMAL") ||
				FOUND(_title, L"STANDARD") || FOUND(_path, L"STANDARD")) {
			bd.difficulty = 2;
		}
		if (FOUND(_title, L"HARD") || FOUND(_path, L"HARD") ||
				FOUND(_title, L"HYPER") || FOUND(_path, L"HYPER")) {
			bd.difficulty = 3;
		}
		if (FOUND(_title, L"ANOTHER") || FOUND(_path, L"ANOTHER") ||
				FOUND(_title, L"EX") || FOUND(_path, L"EX")) {
			bd.difficulty = 4;
		}
		if (FOUND(_title, L"BLACK") || FOUND(_path, L"BLACK") ||
				FOUND(_title, L"KUSO") || FOUND(_path, L"KUSO") ||
				FOUND(_title, L"INSANE") || FOUND(_path, L"INSANE")) {
			bd.difficulty = 5;
		}
#undef FOUND
	}
		
	// total : 160+(note)*0.16
	if (bd.total == 0) {
		bd.getTotal();
	}

	BMSUtil::Log(L"BMSParser", L"Parse finished");
	return true;
}

void BMSParser::ExecutePreProcessor(BMSData bd) {
	// this will modify BMSData
	// TODO check this thing may executed later
		
	std::wstring data = bd.preprocessCommand;
	if (!data.size())
		return;

	std::vector<std::wstring> lines, args;
	boost::split(lines, data, boost::is_any_of(L"\n"));
	for (int i=0; i<lines.size(); i++) {
		// preprocessor
		std::wstring &line = lines[i];
		boost::split(args, line, boost::is_any_of(L" "));
		boost::to_upper(line);
		if (boost::starts_with(line, L"#RANDOM") || boost::starts_with(line, L"#SETRANDOM")) {
			int val = _wtoi(args[1].c_str());
			randomVal[randomStackCnt++] = rand()%val;
			return;
		} else if (boost::starts_with(line, L"#IF")) {
			int val = _wtoi(args[1].c_str());
			if (val == randomVal[randomStackCnt-1])
				condition[randomStackCnt-1] = 2;
			else
				condition[randomStackCnt-1] = 0;
			return;
		} else if (boost::starts_with(line, L"#ELSEIF")) {
			if (condition[randomStackCnt-1] == 2) {
				condition[randomStackCnt-1] = 3;
				return;
			}
			
			int val = _wtoi(args[1].c_str());
			if (val == randomVal[randomStackCnt-1])
				condition[randomStackCnt-1] = 2;
			else
				condition[randomStackCnt-1] = 0;
			return;
		} else if (line.compare(L"#ENDIF") == 0) {
			condition[--randomStackCnt] = 0;
			return;
		}
		if (randomStackCnt > 0) {
			if (condition[randomStackCnt-1] == 1 || condition[randomStackCnt-1] == 3)
				return;
		}
		// preprocessor end
	}
}

std::wstring BMSParser::getBMSCommand(std::wstring &line, std::wstring spliter) {
	int p = line.find(spliter);
	if (p == std::wstring::npos) {
		return L"";
	} else {
		return line.substr(0, p);
	}
}

std::wstring BMSParser::getBMSData(std::wstring &line, std::wstring spliter) {
	int p = line.find(spliter);
	if (p == std::wstring::npos) {
		return L"";
	} else {
		return line.substr(p+1);
	}
}

int BMSParser::checkBMSCommand(std::wstring &line) {
	if (line.find(L" ") == std::wstring::npos) {
		if (line.find(L":") == std::wstring::npos) {
			return 0;	// invalid
		} else {
			return 2;	// :
		}
	} else {
		return 1;		// space
	}
}

void BMSParser::PreProcessBMSLine(std::wstring &line, BMSData bd) {
	// in this function, Metadata & midi length will parsed
	// and preprocessor will be parsed
	std::wstring line_up = line;
	boost::to_upper(line_up);
	if (boost::starts_with(line_up, L"#RANDOM")
			|| boost::starts_with(line_up, L"#SETRANDOM")
			|| boost::starts_with(line_up, L"#IF")
			|| boost::starts_with(line_up, L"#ELSEIF")
			|| boost::starts_with(line_up, L"#ENDIF")) {
		bd.preprocessCommand += line + L"\n";
	} else {
		// seperate command
		int check = checkBMSCommand(line);
		if (check == 1) {
			std::wstring cmd = getBMSCommand(line, L" ");
			std::wstring data = getBMSData(line, L" ");
			boost::to_upper(cmd);

			if (cmd.compare(L"#TITLE") == 0) {
				bd.title = data;
			} else
			if (cmd.compare(L"#SUBTITLE") == 0) {
				bd.subtitle = data;
			} else
			if (cmd.compare(L"#PLAYER") == 0) {
				bd.player = _wtoi(data.c_str());
			} else
			if (cmd.compare(L"#GENRE") == 0) {
				bd.genre = data;
			} else
			if (cmd.compare(L"#ARTIST") == 0) {
				bd.artist = data;
			} else
			if (cmd.compare(L"#BPM") == 0) {
				bd.BPM = _wtof(data.c_str());
			} else
			if (cmd.compare(L"#DIFFICULTY") == 0) {
				bd.difficulty = _wtoi(data.c_str());
			} else
			if (cmd.compare(L"#PLAYLEVEL") == 0) {
				bd.playlevel = _wtoi(data.c_str());
			} else
			if (cmd.compare(L"#RANK") == 0) {
				bd.rank = _wtoi(data.c_str());
			} else
			if (cmd.compare(L"#TOTAL") == 0) {
				bd.total = (int) _wtof(data.c_str());
			} else
			if (cmd.compare(L"#VOLWAV") == 0) {
				bd.volwav = (int) _wtof(data.c_str());
			} else
			if (cmd.compare(L"#STAGEFILE") == 0) {
				bd.stagefile = data;
			} else
			if (cmd.compare(L"#LNTYPE") == 0) {
				LNType = _wtoi(data.c_str());
			} else if (boost::starts_with(cmd, L"#STP")) {
					std::vector<std::wstring> pt;
					boost::split(pt, cmd.substr(4), boost::is_any_of(L"."));
					
					BMSKeyData *nData = (BMSKeyData*)malloc(sizeof(BMSKeyData));
					memset(nData, 0, sizeof(BMSKeyData));
					nData->setValue(_wtof(data.c_str())/1000);
					nData->key = 9;		// STOP Channel
					nData->setBeat(_wtoi(pt[0].c_str()) + _wtoi(pt[1].c_str())/1000.0);
					bd.bmsdata.push_back(nData);
			} else if (boost::starts_with(cmd, L"#LNOBJ")) {
				bd.LNObj[BMSUtil::ExtHexToInt(data)] = true;
			} else
			if (boost::starts_with(cmd, L"#BMP")) {
				int index = BMSUtil::ExtHexToInt(cmd.substr(4, 6));
				bd.str_bg[index] = data;
			} else
			if (boost::starts_with(cmd, L"#WAV")) {
				int index = BMSUtil::ExtHexToInt(cmd.substr(4, 6));
				bd.str_wav[index] = data;
			} else
			if (boost::starts_with(cmd, L"#BPM")) {
				int index = BMSUtil::ExtHexToInt(cmd.substr(4, 6));
				bd.str_bpm[index] = _wtof(data.c_str());
			} else
			if (boost::starts_with(cmd, L"#STOP")) {
				int index = BMSUtil::ExtHexToInt(cmd.substr(4, 6));
				bd.str_stop[index] = _wtof(data.c_str());
			}
		}

		if (check == 2) {
			std::wstring cmd = getBMSCommand(line, L":");
			std::wstring data = getBMSData(line, L":");

			if (!BMSUtil::IsInteger(cmd.substr(1, 6))) return;
			int beat = _wtoi(cmd.substr(1, 4).c_str());
			int channel = BMSUtil::HexToInt(cmd.substr(4, 6));	// channel is heximedical
				
			if (channel == 2) {
				double length_beat = _wtof(data.c_str());
				if (length_beat == 0) {
					BMSUtil::Log(L"BMSParser", L"length_beat cannot be Zero, ignored.");
				}
				// TODO fix!
				if (fmod(length_beat * 4, 1) == 0) {
					bd.beat_numerator[beat] = (int)(length_beat * 4);
					bd.beat_denominator[beat] = 4;
				} else if (fmod(length_beat * 8, 1) == 0) {
					bd.beat_numerator[beat] = (int)(length_beat * 8);
					bd.beat_denominator[beat] = 8;
				} else if (fmod(length_beat * 16, 1) == 0) {
					bd.beat_numerator[beat] = (int)(length_beat * 16);
					bd.beat_denominator[beat] = 16;
				} else if (fmod(length_beat * 32, 1) == 0) {
					bd.beat_numerator[beat] = (int)(length_beat * 32);
					bd.beat_denominator[beat] = 32;
				} else if (fmod(length_beat * 64, 1) == 0) {
					bd.beat_numerator[beat] = (int)(length_beat * 64);
					bd.beat_denominator[beat] = 64;
				}
			}
		}
	}
}

void BMSParser::ProcessBMSLine(std::wstring &line, BMSData bd) {
	/*
		* many BMS has not follow this rule,
		* so we're going to ignore it.
		* 
	if (line.compareTo("*---------------------- HEADER FIELD") == 0) {
		BMSParseMode = BMS_PARSER_HEADER;
		return;
	}
	if (line.compareTo("*---------------------- MAIN DATA FIELD") == 0) {
		BMSParseMode = BMS_PARSER_MAINDATA;
		return;
	}
	if (line.compareTo("*---------------------- BGA FIELD") == 0) {
		BMSParseMode = BMS_PARSER_BGA;
		return;
	}
	*/

	// in this function, we'll only parse note data.
	
	std::wstring cmd = getBMSCommand(line, L":");
	std::wstring data = getBMSData(line, L":");
	if (cmd.size() > 0) {
		if (!BMSUtil::IsInteger(cmd.substr(1, 6))) return;
		int beat = _wtoi(cmd.substr(1, 4).c_str());
		int channel = BMSUtil::HexToInt(cmd.substr(4, 6));	// channel is heximedical
			
		if (channel == 2) {
			// ignore! we already did it in PreProcessBMSLine
		} else {
			if (channel == 1) {
				// BGM
				BGALayerCount[beat]++;
			}
				
			int ncb = data.length();
			for (int i=0; i<ncb/2; i++) {
				std::wstring val_str = data.substr(i*2, i*2+2);
				int val = BMSUtil::ExtHexToInt(val_str);
				if (val == 0) {
					// ignore data 00
					LNprevVal[channel] = 0;
					continue;		
				}

				double nb = beat + (double)i/(double)ncb*2;
				BMSKeyData *nData = (BMSKeyData*)malloc(sizeof(BMSKeyData));
				memset(nData, 0, sizeof(BMSKeyData));
				nData->setValue(val);
				nData->key = channel;
				nData->setBeat(nb);
				nData->setNumerator(i*((192*bd.getBeatNumerator(beat)/bd.getBeatDenominator(beat))/(ncb/2)));
				// beat numerator must proceed first!!
					
				if (nData->is1PChannel() || nData->is2PChannel() || nData->is1PLNChannel() || nData->is2PLNChannel())
					bd.notecnt ++;
					
					
				if (nData->isBGMChannel()) {
					// BGM
					nData->layernum = BGALayerCount[beat];
					bd.bgmdata.push_back(nData);
				} else if (nData->isBPMExtChannel()) {
					nData->setValue(bd.getBPM(val));
					nData->setBPMChannel();  // for ease
					bd.bmsdata.push_back(nData);
				} else if (nData->isSTOPChannel()) {
					//nData->setValue(bd.getSTOP(val));	// TODO: what is it... bug??
					nData->setValue(val);
					bd.bmsdata.push_back(nData);
				} else if (nData->isBPMChannel()) {
					nData->setValue(BMSUtil::HexToInt(val_str));
					bd.bmsdata.push_back(nData);
				} else if (nData->isBGAChannel() || nData->isBGALayerChannel() || nData->isPoorChannel()) {
					// BGA
					bd.bgadata.push_back(nData);
				} else if (nData->is1PChannel() || nData->is2PChannel()) {
					// BMS key
					// if you need data for playing, use convertLNOBJ() in BMSData class.
					bd.bmsdata.push_back(nData);
				} else if (nData->is1PTransChannel() || nData->is2PTransChannel()) {
					// transparent key sound
					bd.bmsdata.push_back(nData);
				} else if (nData->is1PLNChannel() || nData->is2PLNChannel()) {
					// long note (LNTYPE)
					// find previous LN obj and set etime.
					// if no previous LN obj found, then insert new one.
					bool foundObj = false;
					for (int _i=bd.bmsdata.size()-1; _i>=0 ;_i--)
					{
						if (LNType == 2 && nData->getKey() != LNprevVal[channel])
							break;	// LNTYPE 2: create new keydata when not continuous
							
						BMSKeyData *oldData = bd.bmsdata[_i];
						if (nData->getKey() == oldData->getKey()) {
							if (LNType == 1) {
								// LNTYPE 1: only uses clean one
								//oldData.ebeat = nData->ebeat;
								//oldData.evalue = nData->value;
								bd.bmsdata.push_back(nData);
								foundObj = true;
								break;
							} else if (LNType == 2) {
								// LNTYPE 2: able to use dirty one when continuous.
								// TODO it may not have same key. have to be fixed.
								//oldData.ebeat = nData->ebeat;
								//oldData.evalue = nData->value;
								if (LNKey[channel] != oldData)
									bd.bmsdata.erase(bd.bmsdata.begin()+_i);
								bd.bmsdata.push_back(nData);
								foundObj = true;
								break;
							}
						}
					}
						
					if (!foundObj) {
						nData->isLNfirst = true;
						bd.bmsdata.push_back(nData);
						LNKey[channel] = nData;
					} else {
						bd.notecnt--;	// LN needs 2 key data, so 1 discount to correct note number.
					}
				}
					
					
				// save prev val for LNTYPE 2
				if (nData->is1PLNChannel() || nData->is2PLNChannel()) {
					LNprevVal[channel] = val;
				} else {
					LNprevVal[channel] = 0;
				}
			}
		}
	}
}