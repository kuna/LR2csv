#include <string>
#include <vector>
#include <Windows.h>	// typedef unsigned char BYTE / unsigned int DWORD

// uses autoencoder
#include "..\AutoEncoder\AutoEncoder.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\AutoEncoder.lib")
#else
#pragma comment(lib, "..\\Release\\AutoEncoder.lib")
#endif

// uses Markup for CSV file saving
#include "..\Markup\Markup.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\Markup.lib")
#else
#pragma comment(lib, "..\\Release\\Markup.lib")
#endif

// for md5 hash
#include "..\md5\md5.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\md5.lib")
#else
#pragma comment(lib, "..\\Release\\md5.lib")
#endif