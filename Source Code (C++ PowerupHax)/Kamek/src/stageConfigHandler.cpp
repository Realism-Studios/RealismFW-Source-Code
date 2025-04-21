#include <game.hpp>

// Handles various configuration settings for stages

class dStageConfigHandler_c : public dActor_c {
public:
    int onCreate();
    int onDelete();

    // settings
    bool hideClock;

    static dStageConfigHandler_c *instance;
    static dActor_c *build();
};

const SpriteData StageConfigSpriteData = {ProfileId::StageConfig, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}; // flag: 0x2 to make it always spawn
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile StageConfigProfile(&dStageConfigHandler_c::build, SpriteId::StageConfig, &StageConfigSpriteData, ProfileId::AC_DOKAN_CANNON, ProfileId::StageConfig, "StageConfig", NULL);

dStageConfigHandler_c *dStageConfigHandler_c::instance = 0;

dActor_c *dStageConfigHandler_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dStageConfigHandler_c));
	dStageConfigHandler_c *c = new(buffer) dStageConfigHandler_c;

	instance = c;
	return c;
}

// externs
extern bool shouldHideGameSceneClock;

// set all values
int dStageConfigHandler_c::onCreate() {
    hideClock = (settings & 0x1);
    shouldHideGameSceneClock = hideClock;
    
    return true;
}

// reset all values so they don't carry over to other stages
int dStageConfigHandler_c::onDelete() {
    shouldHideGameSceneClock = false;

    return true;
}