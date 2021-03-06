#include "..\CSVParser\CSVData.h"
#include "Scene.h"
#include "DXGame.h"
#include "DXTexture.h"
#include "DXFont.h"

class GameManager {
private:

public:
	static DXGame *dxGame;
	static int GameMode;
public:
	static void InitGame(DXGame *dxGame);
	static bool setGameMode(int mode);
	static void startScene();
	static bool loadScene(int mode);
	static double getFadeAlpha();
	static void Invalidate();	// should be called every frame

	static void LoadSkinResource(DXGame *dxGame);
	static void ReleaseSkinResource();
	static void LoadSounds();
	static void ReleaseSounds();
	static void PlaySound(int num);
	static void StopSound(int num=-1);
	static void Release();	// when program ends

	// check list
	static void InitSelectList();
	static void SelectList();

	static CSVData* getCSVData();
	static CSVEventHandler* getCSVHandler();
	static DXTexture* getTexture(int num);
	static DXFont* getFont(int num);
};

enum GAMEMODE {
	NONE = 0,
	SELECT = 1,
	DECIDE = 2,
	PLAY = 3,
	RESULT = 4
};

namespace SOUND {
	enum SOUND {
		SELECT =0,
		DECIDE = 1,
		EXSELECT = 2,
		EXDECIDE = 3,
		FOLDER_OPEN = 4,
		FOLDER_CLOSE = 5,
		PANEL_OPEN = 6,
		PANEL_CLOSE = 7,
		OPTION_CHANGE = 8,
		DIFFICULTY = 9,
		SCREENSHOT = 10,
		CLEAR = 11,
		FAIL = 12,
		STOP = 13,
		MINE = 14,
		SCRATCH = 15,
		COURSECLEAR = 16,
		COURSEFAIL = 17
	};
};