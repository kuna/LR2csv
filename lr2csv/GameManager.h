class GameManager {
public:
	static int GameMode;
public:
	static void startGame();
	static void setPlayMode();
	static void setSelectMode();
};

enum GAMEMODE {
	NONE = 0,
	SELECT = 1,
	DECIDE = 2,
	PLAY = 3,
	RESULT = 4
};