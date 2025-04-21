#include <game.hpp>
#include <profile.hpp>

class dFakeCrash_c : public dStageActor_c {
public:
	m2d::EmbedLayout_c layout;

	int onCreate();
	int onExecute();
	int onDraw();
	int onDelete();

	u64 eventFlag;

	void displayLayout();
	bool layoutLoaded;
	bool visible;

	nw4r::lyt::Picture *P_dolphinBG_00;

	static dFakeCrash_c *instance;
	static dActor_c *build();
};

extern int MessageBoxIsShowing;

const char *FakeCrashFileList[] = {0};
const SpriteData FakeCrashSpriteData = {ProfileId::FakeCrash, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}; // flag: 0x2 to make it always spawn
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile FakeCrashProfile(&dFakeCrash_c::build, SpriteId::FakeCrash, &FakeCrashSpriteData, ProfileId::AC_DOKAN_CANNON, ProfileId::FakeCrash, "FakeCrash", FakeCrashFileList);

dFakeCrash_c *dFakeCrash_c::instance = 0;

dActor_c *dFakeCrash_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dFakeCrash_c));
	dFakeCrash_c *c = new(buffer) dFakeCrash_c;

	instance = c;
	return c;
}

int dFakeCrash_c::onCreate() {
	if (!layoutLoaded) {
		if (!layout.loadArc("fakeCrash.arc", false))
			return false;

		layout.build("fakeCrash.brlyt");
		layout.drawOrder = 160;

		// get pane
		P_dolphinBG_00 = layout.findPictureByName("P_dolphinBG_00");

		// show the magenta background if it's dolphin
		if (isDolphin())
			P_dolphinBG_00->SetVisible(true);
		else
			P_dolphinBG_00->SetVisible(false);

		layoutLoaded = true;
	}

	
	

	// spritedata
	char eventNum = (this->settings >> 24) & 0xFF; // nybbles 5-6
	eventFlag = (u64)1 << (eventNum - 1);

	visible = false;

	return true;
}

int dFakeCrash_c::onExecute() {
	// only trigger if the triggering event ID is active
	if (dFlagMgr_c::instance->flags & this->eventFlag) {
		displayLayout();

		// since this sprite freezes the game, we should save the progress
		// gotta at least TRY to be nice to the players...
		SaveGame(0, false);
	}

	return true;
}

int dFakeCrash_c::onDraw() {
	if (visible)
		layout.scheduleForDrawing();

	return true;
}

int dFakeCrash_c::onDelete() {
	if (PauseManager_c::instance)
		PauseManager_c::instance->disablePause = false; // enable pausing
	MessageBoxIsShowing = false; // unfreeze game

	return layout.free();
}


void dFakeCrash_c::displayLayout() {
	PauseManager_c::instance->disablePause = true; // disable pausing
	MessageBoxIsShowing = true; // freeze game

	// pause bgm
	SndSceneMgr *sndScnMgr = SndSceneMgr::createInstance();
	sndScnMgr->pauseGame();
	visible = true;
}