#include <game.hpp>
#include <profile.hpp>
#include <sfx.hpp>

class dScVideoMenu_c : public dScene_c {
public:
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dScVideoMenu_c> state;

    dScVideoMenu_c() : state(this, &StateID_InitWait) { }

    int onCreate();
    int onExecute();
    int onDraw();
    int onDelete();

    bool loadLayout();

    dYesNoWindow_c *yesNoWindow;
    dSelectCursor_c *selectCursor;

    dSettingMenu_c *settingMenu;
    
    bool layoutLoaded;

    static dScVideoMenu_c *instance;
    static dActor_c *build();

    USING_STATES(dScVideoMenu_c);
    DECLARE_STATE(InitWait);
    DECLARE_STATE(OpenAnimeEndWait);
    DECLARE_STATE(KeyWait);
    DECLARE_STATE(YesNoWindow);
    DECLARE_STATE(OutScreenEndWait);
};

CREATE_STATE(dScVideoMenu_c, InitWait);
CREATE_STATE(dScVideoMenu_c, OpenAnimeEndWait);
CREATE_STATE(dScVideoMenu_c, KeyWait);
CREATE_STATE(dScVideoMenu_c, YesNoWindow);
CREATE_STATE(dScVideoMenu_c, OutScreenEndWait);

const SpriteData VideoMenuSpriteData = {ProfileId::VideoMenu, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile VideoMenuProfile(&dScVideoMenu_c::build, NULL, &VideoMenuSpriteData, ProfileId::AC_DOKAN_CANNON, ProfileId::VideoMenu, "VideoMenu", 0);

dScVideoMenu_c *dScVideoMenu_c::instance = 0;

dActor_c *dScVideoMenu_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dScVideoMenu_c));
	dScVideoMenu_c *c = new(buffer) dScVideoMenu_c;

	instance = c;
	return (dActor_c*)c;
}


// really simple when you don't need 3D
void dScVideoMenuDrawFunc() {
    m2d::draw();
}

int dScVideoMenu_c::onCreate() {
    *CurrentDrawFunc = dScVideoMenuDrawFunc;

    GXColor screenColor = {0, 0, 0, 0xFF};
    setScreenColorForScene(&screenColor);

    // create some stuff
    yesNoWindow = (dYesNoWindow_c*)CreateParentedObject(YES_NO_WINDOW, this, 0, 0);
    selectCursor = (dSelectCursor_c*)CreateParentedObject(SELECT_CURSOR, this, 0, 0);
    settingMenu = (dSettingMenu_c*)CreateParentedObject(SettingMenu, this, 0, 0);
    settingMenu->visible = true;

    SetScreenFader(WIPE_FADE);

    // begin setting up layouts
    bool setupBG = loadLayout();
    if (!setupBG)
        return false;

    state.setState(&StateID_OpenAnimeEndWait);

    return true;
}

// videoSelectBG
bool dScVideoMenu_c::loadLayout() {
    if (!layoutLoaded) {
        bool gotARC = layout.loadArc("videoSelectBG.arc", false);
        if (!gotARC)
            return false;

        layout.build("videoSelectBG.brlyt");

        layout.drawOrder = 0;

        static const char *brlanName[] = {"videoSelectBG_loopBG.brlan"};
        layout.loadAnimations(brlanName, 1);

        static const char *groupName[] = {"A00_BG"};
        static const int groupID[] = {0};
        layout.loadGroups(groupName, groupID, 1);

        layout.enableLoopAnim(0);
    }

    layoutLoaded = true;

    return true;
}

int dScVideoMenu_c::onExecute() {
    state.execute();

    layout.execAnimations();
    layout.update();

    return true;
}

int dScVideoMenu_c::onDraw() {
    layout.scheduleForDrawing();

    return true;
}

int dScVideoMenu_c::onDelete() {
    return layout.free();
}



void dScVideoMenu_c::beginState_InitWait() { }
void dScVideoMenu_c::executeState_InitWait() { }
void dScVideoMenu_c::endState_InitWait() { }


void dScVideoMenu_c::beginState_OpenAnimeEndWait() {}
void dScVideoMenu_c::executeState_OpenAnimeEndWait() {
    state.setState(&StateID_KeyWait);
}
void dScVideoMenu_c::endState_OpenAnimeEndWait() { }


void dScVideoMenu_c::beginState_KeyWait() {}
void dScVideoMenu_c::executeState_KeyWait() {
    int nowPressed = Remocon_GetPressed(GetActiveRemocon());

    // exiting
    if (nowPressed & WPAD_ONE) {
        state.setState(&StateID_YesNoWindow);
        return;
    }
}
void dScVideoMenu_c::endState_KeyWait() { }


// Exiting or whatever now
void dScVideoMenu_c::beginState_YesNoWindow() {
    yesNoWindow->type = EXIT_VIDEO_MENU;
	yesNoWindow->visible = true;
}
void dScVideoMenu_c::executeState_YesNoWindow() {
    int nowPressed = Remocon_GetPressed(GetActiveRemocon());
    if (nowPressed & WPAD_ONE) {
        yesNoWindow->cancelled = true;
        return;
    }

	if (Wiimote_TestButtons(GetActiveWiimote(), WPAD_A | WPAD_TWO)) {
		yesNoWindow->close = true;

        if (yesNoWindow->current == 1) // OK
            state.setState(&StateID_OutScreenEndWait);
        else // cancel
            state.setState(&StateID_KeyWait);
	}

	// deny primary menu control until this window is closed
	if (!yesNoWindow->visible)
		state.setState(&StateID_OutScreenEndWait);
}
void dScVideoMenu_c::endState_YesNoWindow() { }


void dScVideoMenu_c::beginState_OutScreenEndWait() {
    SetScreenFader(WIPE_FADE);
    PlayOutFader(0x1E);
}
void dScVideoMenu_c::executeState_OutScreenEndWait() {
    if (finishedFadeOut(mFaderPtr))
        state.setState(&StateID_InitWait);
}
void dScVideoMenu_c::endState_OutScreenEndWait() {
    DoSceneChange(GAME_SETUP, 0, false);
}