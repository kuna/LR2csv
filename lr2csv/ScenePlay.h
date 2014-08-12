#include "Scene.h"

namespace PlayJudgeTime {
	enum PlayJudgeTime {
		EASY = 21,
		NORMAL = 18,
		HARD = 15,
		VERYHARD = 8,
	};
};

namespace PlayJudge {
	enum PlayJudge {
		NOJUDGE = -1,
		NOPOOR = 0,
		POOR = 1,
		BAD = 2,
		GOOD = 3,
		GREAT = 4,
		PGREAT = 5
	};
};

class ScenePlay: public Scene {
private:
	BMSData *bmsData;
	int combo;
	
	BMSKeyData* getLastValidKey(int num=-1);	// TODO: insert it into BMSData
	int checkJudge(int diffTime);
	int GetJudgeTime(int judge, int judgetime);
	void processJudge(int judge);
public:
	// redeclaration
	void OnKeyDown(int key);
	void OnKeyUp(int key);
	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnRButtonDown(int x, int y);
	void OnRButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseWheel(int delta);

	// customize
	void LanePress(int num);
	void LaneUp(int num);
	void SetBMSData(BMSData *bmsData);
	void InitalizeCSV();
};