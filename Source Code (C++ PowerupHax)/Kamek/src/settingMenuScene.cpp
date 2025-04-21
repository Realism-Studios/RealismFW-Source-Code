#include <game.hpp>
#include <profile.hpp>
#include <sfx.hpp>

class dScSettingMenu_c : public dScene_c {
public:
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dScSettingMenu_c> state;

    dScSettingMenu_c() : state(this, &StateID_Wait) { }

    int onCreate();
    int onExecute();

    dSelectCursor_c *selectCursor;
    dSettingMenu_c *settingMenu;
    dInfoWindow_c *infoWindow;

    static dScSettingMenu_c *instance;
    static dActor_c *build();

    USING_STATES(dScSettingMenu_c);
    DECLARE_STATE(Wait);
    DECLARE_STATE(ExitWait);
};

CREATE_STATE(dScSettingMenu_c, Wait);
CREATE_STATE(dScSettingMenu_c, ExitWait);

const SpriteData SettingSceneSpriteData = {ProfileId::SettingMenuScene, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile SettingSceneProfile(&dScSettingMenu_c::build, NULL, &SettingSceneSpriteData, ProfileId::AC_DOKAN_CANNON, ProfileId::SettingMenuScene, "SettingMenuScene", 0);

dScSettingMenu_c *dScSettingMenu_c::instance = 0;

dActor_c *dScSettingMenu_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dScSettingMenu_c));
	dScSettingMenu_c *c = new(buffer) dScSettingMenu_c;

	instance = c;
	return (dActor_c*)c;
}


int dScSettingMenu_c::onCreate() {
    *CurrentDrawFunc = DrawFuncFor2dScenes;

    GXColor screenColor = {0, 0, 0, 0xFF};
    setScreenColorForScene(&screenColor);

    // create some stuff
    //infoWindow = (dInfoWindow_c*)CreateParentedObject(INFO_WINDOW, this, 0, 0);
    selectCursor = (dSelectCursor_c*)CreateParentedObject(SELECT_CURSOR, this, 0, 0);
    settingMenu = (dSettingMenu_c*)CreateParentedObject(SettingMenu, this, 0, 0);
    settingMenu->visible = true;

    SetScreenFader(WIPE_FADE);

    return true;
}

int dScSettingMenu_c::onExecute() {
    state.execute();

    return true;
}


void dScSettingMenu_c::beginState_Wait() {}
void dScSettingMenu_c::executeState_Wait() {}
void dScSettingMenu_c::endState_Wait() {}


void dScSettingMenu_c::beginState_ExitWait() {
    DoSceneChange(WORLD_MAP, 0, false);
}
void dScSettingMenu_c::executeState_ExitWait() {}
void dScSettingMenu_c::endState_ExitWait() {}