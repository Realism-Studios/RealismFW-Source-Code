#include <game.hpp>
#include "fileload.hpp"

void ChooseTitlescreenStage() {
    resetGameSettings(); // fix everything

    SaveBlock *save = GetSaveFile()->GetBlock(-1);
    int worldID = save->current_world;
    int submapID = save->current_submap + 40;

    // todo find a way to make it load 10-40 on a new boot of the game, like it did earlier until i accidentally "fixed" it

    // okay so here's my idea on how to do this: have an external bool that determines if it should use 10-40.arc
    // have it set to true in the controllerinformation code hack, then set to false at the end of this (so it's only enabled once per session)

    //if (submapID == 0) {
    //    worldID = 9; // actually 10
    //    submapID = 40;
    //}
    
    // insert level number and some replay info
    RESTART_CRSIN_LevelStartStruct.world1 = worldID;
    RESTART_CRSIN_LevelStartStruct.level1 = submapID - 1;
    RESTART_CRSIN_LevelStartStruct.screenType = 2; // titlescreen behavior
    RESTART_CRSIN_LevelStartStruct.isReplay = 2; // without this the titlescreen will behave as playable stage after a title replay ends
    return;
}
	
class dEventOpeningTitle_c : public dBase_c {
public:
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dEventOpeningTitle_c> state;

    nw4r::lyt::Pane *rootPane;
    nw4r::lyt::TextBox *T_E3verCheck;
    nw4r::lyt::Pane *N_proportionC_00;
    u8 unk[0x24];

    bool layoutLoaded;
    u8 field_275;
    u8 userCancelled_maybe;
    int whichButton; // new

	bool loadLayout();
    // new functions
	int newOnCreate();
    void chooseGameLogo();
    void chooseRandomButton();
    void buttonFunctionality();

    USING_STATES(dEventOpeningTitle_c);
    REF_NINTENDO_STATE(PressButtonExitAnimeEndWait);
};

// not exactly accurate to the original function, but it works the same
int dEventOpeningTitle_c::newOnCreate() {
	int gotLayout;

	if (!layoutLoaded) {
		gotLayout = loadLayout();
		if (!gotLayout)
			return false;
		
		rootPane->SetVisible(false);

		layout.drawOrder = 148;
		field_275 = 0;

		// new shit
        WriteBMGToTextBox(T_E3verCheck, GetBMG(), 0x3E9, 0x1, 0); // writes version number
        chooseRandomButton();
        chooseGameLogo();
		
		// not new shit
        if (IsWideScreen()) {
			layout.layout.rootPane->scale.x = 1.0f;
			layout.layout.rootPane->scale.y = 1.0f;
		} else {
			layout.layout.rootPane->scale.x = 0.85f;
			layout.layout.rootPane->scale.y = 0.85f;
		}

		layoutLoaded = true;
	}

	return true;
}

void dEventOpeningTitle_c::chooseGameLogo() {
    // i don't feel like adding things to the class
    nw4r::lyt::Picture *P_logo_00 = layout.findPictureByName("P_logo_00");

    SaveBlock *save = GetSaveFile()->GetBlock(-1); // most recently used file
    int worldID = save->current_world + 1;
    int submapID = save->current_submap + 10; // add 10 so in hex it'll be a, b, or c
    
    char tplName[64];
	sprintf(tplName, "/Layout/tecks tore/title/gameTitleLogo_W%i%x.tpl", worldID, submapID);
	static File tpl;
	if (tpl.open(tplName)) {
		P_logo_00->material->texMaps[0].ReplaceImage((TPLPalette*)tpl.ptr(), 0);
	}
    // if it can't find it, it just uses the texture inside the archive
}

void dEventOpeningTitle_c::chooseRandomButton() {
    int buttonValue = selectRandomButton();
    whichButton = buttonValue;
}

void dEventOpeningTitle_c::buttonFunctionality() {
    int nowPressed = Remocon_GetPressed(GetActiveRemocon());

    if (nowPressed & whichButton) {
        state.setState(&dEventOpeningTitle_c::StateID_PressButtonExitAnimeEndWait);
    }
}