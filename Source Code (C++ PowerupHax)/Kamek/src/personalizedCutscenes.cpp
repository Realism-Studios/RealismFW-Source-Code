#include <game.hpp>
#include <profile.hpp>

class cutscene1 : public dStageActor_c { // if this ends up being good i'll make a base class for these kinds of things but it's too specific rn
public:
	m2d::EmbedLayout_c layout;

	int onCreate();
	int onExecute();
	int onDraw();
    int onDelete();   
    void freezeGame();

    bool loaded;
    u8 screenNum;
    int timer; // this one's a real workhorse
    nw4r::lyt::Picture *im_base_00; // my lazy name for the fake TV static that occurs at the end of the "cutscene"
    nw4r::lyt::TextBox
		*txt0, *txt1, *txt2, *txt3, *txt4;

    static cutscene1 *instance;
	static dActor_c *build();
};

extern int MessageBoxIsShowing;

const char *cutscene1filelist[] = {0};
const SpriteData cutscene1SpriteData = {ProfileId::Personalization1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}; // flag: 0x2 to make it always spawn
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile cutscene1Profile(&cutscene1::build, NULL, &cutscene1SpriteData, ProfileId::BOOT, ProfileId::Personalization1, "Personalization1", cutscene1filelist); // CreepyPasta is the most important thing. It has runs FIRST

cutscene1 *cutscene1::instance = 0;

dActor_c *cutscene1::build() {
	void *buffer = AllocFromGameHeap1(sizeof(cutscene1));
	cutscene1 *c = new(buffer) cutscene1;

	instance = c;
	return c;
}

int cutscene1::onCreate() {
	if (!loaded) {
		if (!layout.loadArc("personalized_1.arc", false))
			return false;

		layout.build("personalized_1.brlyt");
		layout.drawOrder = 160;

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

		

		loaded = true;
	}

    static const char *textBoxNames[] = {
	"txt0", "txt1",
	"txt2", "txt3", "txt4"
	};
	layout.getTextBoxes(textBoxNames, &txt0, 5);
    im_base_00 = layout.findPictureByName("im_static");
    // Firstly we show the first text pane
    txt0->SetVisible(true);
    txt1->SetVisible(false);
    txt2->SetVisible(false);
    txt3->SetVisible(false);
    txt4->SetVisible(false);
    im_base_00->SetVisible(false);
    // also this is amazing forgiveness
    SaveGame(0, false);
    freezeGame();

	return true;
}

// this is totally required even though it'll never do anything
int cutscene1::onDelete() {
	if (PauseManager_c::instance)
		PauseManager_c::instance->disablePause = false; // enable pausing
	MessageBoxIsShowing = false; // unfreeze game

	return layout.free();
}

int cutscene1::onDraw() {
    layout.scheduleForDrawing();

    return true;
}

int cutscene1::onExecute() {
    ++timer;

    if (timer == 300) { // 5 seconds later
        txt0->SetVisible(false);
        txt1->SetVisible(true);
    }
    if (timer == 600) {
        txt1->SetVisible(false);
        txt2->SetVisible(true);
    }
    if (timer == 960) {
        txt2->SetVisible(false);
        txt3->SetVisible(true);
    }
    if (timer == 1320) {
        txt3->SetVisible(false);
        txt4->SetVisible(true);
    }
    // FINALLY END THIS
    if (timer == 2000) {
        txt4->SetVisible(false);
        im_base_00->SetVisible(true); // END THIS
    }
	return true;
}

void cutscene1::freezeGame() {
	PauseManager_c::instance->disablePause = true; // disable pausing
	//MessageBoxIsShowing = true; // freeze game
    // sadly with the above the cutscene also freezes. I need fixes. I'm going to sleep - surrealism 1/7/2025 2:30 AM

	// pause bgm
	SndSceneMgr *sndScnMgr = SndSceneMgr::createInstance();
	sndScnMgr->pauseGame();
}
