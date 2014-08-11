#pragma once
#include <string>
#include <vector>
#include "md5.h"
#include <Windows.h>

// for encode auto detect
#include "..\AutoEncoder\AutoEncoder.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\AutoEncoder.lib")
#else
#pragma comment(lib, "..\\Release\\AutoEncoder.lib")
#endif