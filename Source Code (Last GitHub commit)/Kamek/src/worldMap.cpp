// general world map hacks go here
#include <game.hpp>

extern char CurrentWorld;

bool FUN_809087A0();
void hideToadBalloons();

// THIS IS COMPLETELY INCORRECT!!!!
// part of dWmPlayer_c or whatever the fuck it's called, not the map model
u32 daWmMap_c::handleButtonPresses() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());
	dCsSeqMng_c *seqMgr = dCsSeqMng_c::instance;
	dCourseSelectManager_c *csMgr = dCourseSelectManager_c::instance;
	int dynamicButton;

	int randomButton[7];
	OSReport("DIFFICULTY: %i\n", difficultySetting);

	// choose our buttons
	if (difficultySetting > DIFFICULTY_GOODLY_LUCK) {
		for (int i = 0; i < 7; i++) {
			rollAgain:
			randomButton[i] = selectRandomButton(false, true);
			
			// don't choose D-Pad buttons or 2
			if ((randomButton[i] < WPAD_PLUS) || (randomButton[i] == WPAD_TWO))
				goto rollAgain;
		}

		OSReport("doing the thing\n");
	} else {
		// default value
		randomButton[0] = WPAD_C;
		randomButton[1] = WPAD_A;
		randomButton[2] = WPAD_PLUS;
		randomButton[3] = WPAD_B;
		randomButton[4] = WPAD_ONE;
		randomButton[5] = WPAD_ONE;
		randomButton[6] = WPAD_MINUS;
	}

	// checks something related to player visibility and if ambush actors exist
	bool flag = FUN_809087A0();
	if (!flag)
		return false;

	bool scriptCheck = seqMgr->areAnyScriptsQueued();
	bool check2 = seqMgr->FUN_80915600();

	if ((!scriptCheck) && (!check2)) {
		// VIEW MAP
		bool isYoko = isControllerWiimote(0);

		dynamicButton = randomButton[0]; // originally WPAD_C
		if (isYoko)
			dynamicButton = randomButton[1]; // originally WPAD_A
		
		if (nowPressed & dynamicButton) {
			if ((csMgr->endedSomeMsgThing | csMgr->messageWindowPtr->display) != 0)
				csMgr->startedSomeMsgThing = true;
			hideToadBalloons();

			return seqMgr->addScriptToQueue(SMC_DEMO_VIEW_WORLD, this, dWorldCamera_c::m_instance, 0x80);
		}

		// PAUSE MENU
		if (nowPressed & randomButton[2]) { // WPAD_PLUS
			return seqMgr->addScriptToQueue(SMC_DEMO_PAUSE_MENU, 0, 0, 0x80);
		}

		// STOCK ITEM
		isYoko = isControllerWiimote(0);

		dynamicButton = (randomButton[3] + randomButton[4]); // WPAD_B and WPAD_ONE
		if (isYoko)
			dynamicButton = randomButton[5]; // WPAD_ONE

		if (nowPressed & dynamicButton) {
			return seqMgr->addScriptToQueue(SMC_DEMO_STOCK_MENU, 0, 0, 0x80);
		}

		// WORLD SELECT
		if (nowPressed & randomButton[6]) { // WPAD_MINUS
			return seqMgr->addScriptToQueue(SMC_DEMO_WORLDSELECT_MENU, 0, 0, 0x80);
		}
	}

	// now handle movement directions
	int unk = map_models[0].direction;
	map_models[0].field_EC = 4;
	map_models[0].direction = 4;

	int direction = 4; // none?
	if (nowPressed & WPAD_UP) {
		direction = 0;
	} else if (nowPressed & WPAD_DOWN) {
		direction = 1;
	} else if (nowPressed & WPAD_LEFT) {
		direction = 2;
	} else if (nowPressed & WPAD_RIGHT) {
		direction = 3;
	}
	map_models[0].direction = direction;

	// try to move that way
	if ((map_models[0].field_B9 & 0x10) == 0) {
		if (map_models[0].direction != 4) {
			map_models[0].field_160 = 0;
			navigatePath();
		}
	}

	// start level
	if (nowPressed & WPAD_TWO) {
		enterCourse();
	}

	return true;
}
