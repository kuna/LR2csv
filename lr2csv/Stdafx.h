#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <tchar.h>
#include <Windows.h>

// autoencoder (for sqlite3)
#include "..\AutoEncoder\AutoEncoder.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\AutoEncoder.lib")
#else
#pragma comment(lib, "..\\Release\\AutoEncoder.lib")
#endif

// uses CSVParser
#include "..\CSVParser\CSVBargraph.h"
#include "..\CSVParser\CSVButton.h"
#include "..\CSVParser\CSVConst.h"
#include "..\CSVParser\CSVData.h"
#include "..\CSVParser\CSVdrawarg.h"
#include "..\CSVParser\CSVElement.h"
#include "..\CSVParser\CSVFile.h"
#include "..\CSVParser\CSVFont.h"
#include "..\CSVParser\CSVNumber.h"
#include "..\CSVParser\CSVOption.h"
#include "..\CSVParser\CSVReader.h"
#include "..\CSVParser\CSVRenderer.h"
#include "..\CSVParser\CSVSelectList.h"
#include "..\CSVParser\CSVSettings.h"
#include "..\CSVParser\CSVSlider.h"
#include "..\CSVParser\CSVText.h"
#include "..\CSVParser\CSVTimer.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\CSVParser.lib")
#else
#pragma comment(lib, "..\\Release\\CSVParser.lib")
#endif

// bmsparser
#include "..\BMSParser\BMSData.h"
#include "..\BMSParser\BMSKeyData.h"
#include "..\BMSParser\BMSParser.h"
#include "..\BMSParser\BMSUtil.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\BMSParser.lib")
#else
#pragma comment(lib, "..\\Release\\BMSParser.lib")
#endif

// directx
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "winmm.lib")
#include <d3d9.h>
#include <d3dx9.h>

// uses sqlite3
#include "..\sqlite3\sqlite3.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\sqlite3.lib")
#else
#pragma comment(lib, "..\\Release\\sqlite3.lib")
#endif

// for using FMOD
#pragma comment(lib, "fmod_vc.lib")
#include <fmod.hpp>

// xml markup
#include "..\Markup\Markup.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\Markup.lib")
#else
#pragma comment(lib, "..\\Release\\Markup.lib")
#endif

// directshow
#define USEDSHOW
#include <DShow.h>
#include <vmr9.h>
#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")
#ifdef USEDSHOW
#pragma comment(lib, "strmiids.lib")
#endif