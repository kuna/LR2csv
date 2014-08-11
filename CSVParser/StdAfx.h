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