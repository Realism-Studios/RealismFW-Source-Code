#include <game.hpp>
#include <profile.hpp>
#include <sfx.hpp>

#define ANIM_HIT_BUTTON 1
#define ANIM_OFF_BUTTON 14
#define ANIM_ON_BUTTON 27

#define BUTTON_COUNT 12

class dScLanguageSelect_c : public dScene_c {
public:
	m2d::EmbedLayout_c layout;
	dStateWrapper_c<dScLanguageSelect_c> state;

	nw4r::lyt::Pane *N_langs_00, *N_langs_01, *W_blue[12];
	nw4r::lyt::Picture *P_last_00, *P_next_00;

	int currentSelection, previousSelection, hitButtonID;
	int currentPage, pagePosModifier, buttonPosModifier, timer;

	bool layoutLoaded;

	dScLanguageSelect_c() : state(this, &StateID_OpenWait) { }

	int onCreate();
	int onExecute();
	int onDraw();
	int onDelete();

	bool loadLayout();

	static dScLanguageSelect_c *instance;
	static dActor_c *build();

	USING_STATES(dScLanguageSelect_c);
	DECLARE_STATE(OpenWait);
	DECLARE_STATE(ButtonChangeWait);
	DECLARE_STATE(KeyWait);
	DECLARE_STATE(PageChangeWait);
	DECLARE_STATE(ExitWait);
};

CREATE_STATE(dScLanguageSelect_c, OpenWait);
CREATE_STATE(dScLanguageSelect_c, ButtonChangeWait);
CREATE_STATE(dScLanguageSelect_c, KeyWait);
CREATE_STATE(dScLanguageSelect_c, PageChangeWait);
CREATE_STATE(dScLanguageSelect_c, ExitWait);

const SpriteData LangSelectSpriteData = {ProfileId::LanguageSelect, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile LangSelectProfile(&dScLanguageSelect_c::build, NULL, &LangSelectSpriteData, ProfileId::CRSIN, ProfileId::LanguageSelect, "LanguageSelect", 0);

dScLanguageSelect_c *dScLanguageSelect_c::instance = 0;

dActor_c *dScLanguageSelect_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dScLanguageSelect_c));
	dScLanguageSelect_c *c = new(buffer) dScLanguageSelect_c;

	instance = c;
	return (dActor_c*)c;
}

// really simple wh- okay yeah you get it by now
void dScLanguageSelectDrawFunc() {
	m2d::draw();
}

int dScLanguageSelect_c::onCreate() {
	*CurrentDrawFunc = dScLanguageSelectDrawFunc;

	GXColor screenColor = {0x33, 0, 0, 0xFF};
	setScreenColorForScene(&screenColor);

	//SetScreenFader(WIPE_FADE);

	bool setup = loadLayout();
	if (!setup)
		return false;

	return true;
}

bool dScLanguageSelect_c::loadLayout() {
	if (!layoutLoaded) {
		bool gotFile = layout.loadArc("languageSelect.arc", false);
		if (!gotFile)
			return false;

		layout.build("languageSelect_03.brlyt");

		static const char *brlanNames[] = {
			"languageSelect_03_hitButton.brlan",
			"languageSelect_03_offButon.brlan",
			"languageSelect_03_onButton.brlan"
		};
		static const char *groupNames[] = {
			"A00_Confirm", "B00_ENG", "B01_SPA", "B02_FRA", "B03_GER", "B04_ITA", "B05_NED", "B06_JPN", "B07_KOR", "C00_TWN", "C01_CHN", "C02_RUS", "C03_AUS",
			"A00_Confirm", "B00_ENG", "B01_SPA", "B02_FRA", "B03_GER", "B04_ITA", "B05_NED", "B06_JPN", "B07_KOR", "C00_TWN", "C01_CHN", "C02_RUS", "C03_AUS",
			"A00_Confirm", "B00_ENG", "B01_SPA", "B02_FRA", "B03_GER", "B04_ITA", "B05_NED", "B06_JPN", "B07_KOR", "C00_TWN", "C01_CHN", "C02_RUS", "C03_AUS",
		};
		static const int groupIDs[] = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
		};
		layout.loadAnimations(brlanNames, 3);
		layout.loadGroups(groupNames, groupIDs, 39);

		N_langs_00 = layout.findPaneByName("N_langs_00");
		N_langs_01 = layout.findPaneByName("N_langs_01");

		for (int i = 0; i < BUTTON_COUNT; i++) {
			char name[16];
			sprintf(name, "W_blue_%02d", i);
			W_blue[i] = layout.findPaneByName(name);
			W_blue[i]->SetVisible(false);
		}

		P_last_00 = layout.findPictureByName("P_last_00");
		P_next_00 = layout.findPictureByName("P_next_00");

		// since we start on page 0
		P_last_00->trans.x = -348.0f;

		layoutLoaded = true;
	}

	return true;
}

int dScLanguageSelect_c::onExecute() {
	state.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int dScLanguageSelect_c::onDraw() {
	layout.scheduleForDrawing();
	return true;
}

int dScLanguageSelect_c::onDelete() {
	return layout.free();
}



void dScLanguageSelect_c::beginState_OpenWait() { }
void dScLanguageSelect_c::executeState_OpenWait() {
	state.setState(&StateID_ButtonChangeWait);
}
void dScLanguageSelect_c::endState_OpenWait() {
	currentSelection = 0;
	previousSelection = -2; // -1 is confirm button
	currentPage = 0;
}


void dScLanguageSelect_c::beginState_ButtonChangeWait() {
	if (previousSelection > -2) {
		MapSoundPlayer(SoundRelatedClass, SE_SYS_CURSOR, 1);
		layout.enableNonLoopAnim(ANIM_OFF_BUTTON + previousSelection);
	}
	hitButtonID = -2;
	previousSelection = currentSelection;
	layout.enableNonLoopAnim(ANIM_ON_BUTTON + currentSelection);
}
void dScLanguageSelect_c::executeState_ButtonChangeWait() {
	if (!layout.isAnimOn())
		state.setState(&StateID_KeyWait);
}
void dScLanguageSelect_c::endState_ButtonChangeWait() { }


void dScLanguageSelect_c::beginState_KeyWait() { }
void dScLanguageSelect_c::executeState_KeyWait() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	if (nowPressed & (WPAD_TWO | WPAD_A)) {
		if (hitButtonID == currentSelection)
			return;
		
		MapSoundPlayer(SoundRelatedClass, SE_SYS_DECIDE, 1);
		if (currentSelection == -1) {
			state.setState(&StateID_ExitWait);
			return;
		} else {
			hitButtonID = currentSelection;
			for (int i = 0; i < BUTTON_COUNT; i++)
				W_blue[i]->SetVisible(false);
			W_blue[hitButtonID]->SetVisible(true);

			// AUS
			if (hitButtonID == 11) {
				layout.layout.rootPane->rotate.z = 180.0f;
			} else {
				layout.layout.rootPane->rotate.z = 0.0f;
			}
			return;
		}
	}
	
	// confirm button only
	if (currentSelection == -1) {
		if (nowPressed & WPAD_UP) {
			currentSelection = (currentPage == 0) ? 6 : 10;
			state.setState(&StateID_ButtonChangeWait);
		}
		return;
	}

	// in left column, pressing right
	if ((nowPressed & WPAD_RIGHT) && (currentSelection % 2 == 0)) {
		currentSelection++;
		state.setState(&StateID_ButtonChangeWait);
		return;
	
	// in right column, pressing left
	} else if ((nowPressed & WPAD_LEFT) && (currentSelection % 2 != 0)) {
		currentSelection--;
		state.setState(&StateID_ButtonChangeWait);
		return;
	}

	// on any row but the top, pressing up
	if (nowPressed & WPAD_UP) {
		int topRowStart = (currentPage == 0) ? 1 : 9;
		
		if (currentSelection > topRowStart) {
			currentSelection -= 2;
			state.setState(&StateID_ButtonChangeWait);
			return;
		}
	}
	
	// on any row but the bottom, pressing down
	if (nowPressed & WPAD_DOWN) {
		int bottomRowStart = (currentPage == 0) ? 6 : 10;
		
		if (currentSelection < bottomRowStart) {
			currentSelection += 2;
			state.setState(&StateID_ButtonChangeWait);
			return;
		} else {
			currentSelection = -1; // confirm button
			state.setState(&StateID_ButtonChangeWait);
			return;
		}
	}


	if (nowPressed & WPAD_MINUS) {
		if (currentPage == 1) {
			state.setState(&StateID_PageChangeWait);
			return;
		}
	}

	if (nowPressed & WPAD_PLUS) {
		if (currentPage == 0) {
			state.setState(&StateID_PageChangeWait);
			return;
		}
	}
	
}
void dScLanguageSelect_c::endState_KeyWait() { }



void dScLanguageSelect_c::beginState_PageChangeWait() {
	MapSoundPlayer(SoundRelatedClass, SE_SYS_PAGE, 1);
	pagePosModifier = (currentPage == 0) ? -25 : 25;
	buttonPosModifier = (currentPage == 0) ? 3.33f : -3.33f;
	timer = 24; // 600 / 25 = 24
}
void dScLanguageSelect_c::executeState_PageChangeWait() {
	currentPage = !currentPage;
	currentSelection = (currentPage == 0) ? 0 : 8;
	
	if (timer != 0) {
		P_last_00->trans.x += buttonPosModifier;
		P_next_00->trans.x += buttonPosModifier;
		
		N_langs_00->trans.x += pagePosModifier;
		N_langs_01->trans.x += pagePosModifier;
		timer--;
	} else {
		state.setState(&StateID_ButtonChangeWait);
	}
}
void dScLanguageSelect_c::endState_PageChangeWait() { }



// pretty much just dScBoot_c::StateID_ProcEnd
void dScLanguageSelect_c::beginState_ExitWait() { }
void dScLanguageSelect_c::executeState_ExitWait() {
	StartTitleScreenStage(false, false);
}
void dScLanguageSelect_c::endState_ExitWait() { }