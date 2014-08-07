#include <tchar.h>

struct SceneSetting {
	TCHAR select[256];
	TCHAR play[256];
	TCHAR decide[256];
	TCHAR result[256];
};

struct ScreenSetting {
	int width;
	int height;
};

struct KeySetting {
};

struct PlaySetting {
	int speed;
	int shutter;
};

class GameSetting {
public:
	static SceneSetting scene;
	static ScreenSetting screen;
	static KeySetting key;
	static PlaySetting play;

public:
	static void SaveSetting();
	static void LoadSetting();
};
