#pragma once
#include <string>
#include <vector>
#include <Windows.h>

// for encode auto detect
#include "..\AutoEncoder\AutoEncoder.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\AutoEncoder.lib")
#else
#pragma comment(lib, "..\\Release\\AutoEncoder.lib")
#endif

// for md5 hash
#include "..\md5\md5.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\md5.lib")
#else
#pragma comment(lib, "..\\Release\\md5.lib")
#endif