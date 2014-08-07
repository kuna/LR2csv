#include "CSVData.h"
#include "Scene.h"
#include "DXGame.h"
#include "DXTexture.h"
#include "DXFont.h"

class GameManager {
private:
	static DXGame *dxGame;

public:
	static int GameMode;
public:
	static void InitGame(DXGame *dxGame);
	static bool setGameMode(int mode);
	static void startScene();
	static bool loadScene(int mode);

	static void LoadSkinResource(DXGame *dxGame);
	static void ReleaseSkinResource();

	static CSVData* getCSVData();
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