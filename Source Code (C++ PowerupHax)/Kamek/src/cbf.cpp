#include <game.hpp>
#include <stage.hpp>
#include <profile.hpp>

extern "C" void *MakeMarioEnterDemoMode(); // Die.
extern "C" void *MakeMarioExitDemoMode(); // Alive.

bool showCBFCalled = false; // give this guy some privacy will you

class dCBF_c : public dEn_c {
public:
    m2d::EmbedLayout_c layout;

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();
	void deactivate();

    void showCBF();
    //void timerMgr(); // E
    bool layoutLoaded;
    bool visible;
    u64 eventFlag;
    u16 stupidTimer; // rarted timer
    u16 timerFrames; // how many frames timer will run (why am i documenting this)
    bool endless;
    // stuff I don't understand
    static dCBF_c *instance;
    static dActor_c *build();
};

// more stuff i don't understand
const char *cbfFileList[] = {0};
const SpriteData cbfSpriteData = {ProfileId::cbf, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}; // flag: 0x2 to make it always spawn
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile cbfProfile(&dCBF_c::build, SpriteId::cbf, &cbfSpriteData, ProfileId::AC_DOKAN_CANNON, ProfileId::cbf, "cbf", cbfFileList);

dCBF_c *dCBF_c::instance = 0;


dActor_c* dCBF_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dCBF_c));
	return new(buffer) dCBF_c;
}


int dCBF_c::onCreate() {
    if (!layoutLoaded) { // copied from fakeCrash because I'm lazy
        if (!layout.loadArc("cbf.arc", false))
            return false;

        layout.build("cbf.brlyt");

        layout.drawOrder = 25;
        
        // UGHHHHHHHHHHHHHHHHHHH
        if (IsWideScreen()) {
		  	  layout.layout.rootPane->scale.x = 0.735f;
	      } 
        else {
			    layout.clippingEnabled = true;
			    layout.clipX = 0;
			    layout.clipY = 52;
			    layout.clipWidth = 640;
			    layout.clipHeight = 352;
			    layout.layout.rootPane->scale.x = 0.731f;
			    layout.layout.rootPane->scale.y = 0.7711f;
		    }

        layoutLoaded = true;
    }

    // Horrid Spritedata's
    char eventNum = (settings >> 24) & 0xFF; // nybbles 5-6
	eventFlag = (u64)1 << (eventNum - 1);
	endless = ((settings & 0x1) != 0); // nybble 12,
	u8 timerSecs;
	timerSecs = (settings & 0xF0) >> 4; // nybble 11
    timerFrames = timerSecs*60; // 60 frames = 1 second
    if (timerSecs == 0) {
        SaveGame(0, false); // try to be nice :)
    }


    visible = false;

	return true;
}

int dCBF_c::onExecute() {
  if (dFlagMgr_c::instance->flags & this->eventFlag) {
    // Don't call showCBF here anymore
    if (visible) {
      stupidTimer++;
      if (stupidTimer == timerFrames) {
        this->Delete(1);
      }
    }
  }
  return true;
}



int dCBF_c::onDelete() { // NOTE does the same thing as end CBF i think. shouldn't be run i think.
    if (PauseManager_c::instance)
		PauseManager_c::instance->disablePause = false; // enable pausing
    MakeMarioExitDemoMode(); // The

    return layout.free();
}

void dCBF_c::deactivate() {
    if (PauseManager_c::instance)
		PauseManager_c::instance->disablePause = false; // enable pausing
    MakeMarioExitDemoMode(); // The
    visible = false;
    stupidTimer = 0;
}

int dCBF_c::onDraw() {
    if (visible)
    layout.scheduleForDrawing();

    return true;
}

void dCBF_c::showCBF() {
  if (!showCBFCalled) {
    visible = true;
    PlaySoundWithFunctionB4(SoundRelatedClass, &menuHandle, SE_SYS_DIALOGUE_IN, 0);
    PauseManager_c::instance->disablePause = true; // disable pausing
    MakeMarioEnterDemoMode();
    showCBFCalled = true;
  }
}


