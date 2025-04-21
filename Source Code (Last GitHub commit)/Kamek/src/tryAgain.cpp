#include "tryAgain.hpp"

CREATE_STATE(dScTryAgain_c, InitWait);
CREATE_STATE(dScTryAgain_c, OpenAnimeEndWait);
CREATE_STATE(dScTryAgain_c, KeyWait);
CREATE_STATE(dScTryAgain_c, OutScreenEndWait);

const char *TryAgainFileList[] = {0};
const SpriteData TryAgainSpriteData = {ProfileId::TryAgain, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile TryAgainProfile(&dScTryAgain_c::build, NULL, &TryAgainSpriteData, ProfileId::AC_DOKAN_CANNON, ProfileId::TryAgain, "Try Again!", TryAgainFileList);

dScTryAgain_c *dScTryAgain_c::instance = 0;

dActor_c *dScTryAgain_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dScTryAgain_c));
	dScTryAgain_c *c = new(buffer) dScTryAgain_c;

	instance = c;
	return c;
}


void displayTryAgainScreen() {
	// try again is disabled
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	if (!tryAgainEnabled) {
		ExitStage(WORLD_MAP, 0, LOSE_LEVEL, WIPE_BOWSER);
		return;
	}

	dScTryAgain_c *tryAgain = dScTryAgain_c::instance;
	tryAgain->displayScreen();
}

// really simple when you don't need 3D
void dScTryAgainDrawFunc() {
	m2d::draw();
}


void dScTryAgain_c::displayScreen() {
	state.setState(&StateID_OpenAnimeEndWait);
}

int dScTryAgain_c::onCreate() {
	//*CurrentDrawFunc = dScTryAgainDrawFunc;

	//GXColor screenColor = {0, 0, 0, 0xFF};
	//setScreenColorForScene(&screenColor);

	//SetScreenFader(WIPE_FADE);

	bool setupLyt = loadLayout();
	if (!setupLyt)
		return false;

	currentButton = 0;

	return true;
}

bool dScTryAgain_c::loadLayout() {
	if (!layoutLoaded) {
		bool gotARC = layout.loadArc("tryAgain.arc", false);
		if (!gotARC)
			return false;

		layout.build("tryAgain.brlyt");

		static const char *brlanNames[] = {
			"tryAgain_inWindow.brlan",
			"tryAgain_onButton.brlan",
			"tryAgain_offButton.brlan"
		};
		layout.loadAnimations(brlanNames, 3);

		static const char *groupNames[] = {
			"A00_Window", "B00_Button", "B01_Button",
			"B00_Button", "B01_Button",
			"B00_Button", "B01_Button"
		};
		static const int groupIDs[] = {
			0, 0, 0,
			1, 1,
			2, 2
		};
		layout.loadGroups(groupNames, groupIDs, 7);

		// get shadow panes
		P_buttonOff_00 = layout.findPictureByName("P_buttonOff_00");
		P_buttonOff_01 = layout.findPictureByName("P_buttonOff_01");
	}

	layoutLoaded = true;

	return true;
}

int dScTryAgain_c::onExecute() {
	state.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int dScTryAgain_c::onDraw() {
	if (visible)
		layout.scheduleForDrawing();

	return true;
}

int dScTryAgain_c::onDelete() {
	return layout.free();
}



void dScTryAgain_c::beginState_InitWait() { }
void dScTryAgain_c::executeState_InitWait() { }
void dScTryAgain_c::endState_InitWait() { }


void dScTryAgain_c::beginState_OpenAnimeEndWait() {
	visible = true;
	layout.enableNonLoopAnim(0);
	MapSoundPlayer(SoundRelatedClass, SE_MG_IH_WIN_BR_OPEN, 1);
}
void dScTryAgain_c::executeState_OpenAnimeEndWait() {
	if (!layout.isAnyAnimOn())
		state.setState(&StateID_KeyWait);
}
void dScTryAgain_c::endState_OpenAnimeEndWait() { }


void dScTryAgain_c::beginState_KeyWait() {
	P_buttonOff_00->SetVisible(false);
}
void dScTryAgain_c::executeState_KeyWait() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	bool playCursorSound = false;

	if (currentButton != 0) {
		if (nowPressed & WPAD_LEFT) {
			P_buttonOff_00->SetVisible(false);
			P_buttonOff_01->SetVisible(true);
			
			playCursorSound = true;
			currentButton--;
		}
	}

	if (currentButton != 1) {
		if (nowPressed & WPAD_RIGHT) {
			P_buttonOff_00->SetVisible(true);
			P_buttonOff_01->SetVisible(false);

			playCursorSound = true;
			currentButton++;
		}
	}

	// button was pressed
	if (nowPressed & WPAD_TWO) {
		MapSoundPlayer(SoundRelatedClass, SE_SYS_DECIDE, 1);
		state.setState(&StateID_OutScreenEndWait);
		return;
	}

	if (playCursorSound)
		MapSoundPlayer(SoundRelatedClass, SE_SYS_CURSOR, 1);
}
void dScTryAgain_c::endState_KeyWait() { }


void dScTryAgain_c::beginState_OutScreenEndWait() {
	P_buttonOff_00->SetVisible(true);
	P_buttonOff_01->SetVisible(true);
}
void dScTryAgain_c::executeState_OutScreenEndWait() {
	state.setState(&StateID_InitWait);
}
void dScTryAgain_c::endState_OutScreenEndWait() {
	if (currentButton == 0)
		ExitStage(CRSIN, 0, LOSE_LEVEL, WIPE_FADE);
	else
		ExitStage(WORLD_MAP, 0, EXIT_LEVEL, WIPE_FADE);
}