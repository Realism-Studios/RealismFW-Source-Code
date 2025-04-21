// general world map hacks go here
#include <game.hpp>
#include "worldMapArrays.hpp"

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

void GetMapModelName(int world, char *buffer);

int LoadFilesForWorldMap() {
	// map model
	GetMapModelName(dScWMap_c::instance->settings & 0xF, dScWMap_c::mWmArcName);
	SpammyReport("Loading map model\n");
	DVD_LoadFile(DVDClass, "WorldMap", dScWMap_c::mWmArcName, 0);

	// course objects, RouteInfo, shadow model
	for (int i = 0; i < 0x10; i++) {
		SpammyReport("Loading common models (%i)\n", i);
		DVD_LoadFile(DVDClass, "WorldMap", commonRes[i], 0);
	}
	
	// stock item resources (and the ghost house boo)
	for (int i = 0; i < STOCK_POWERUP_COUNT; i++) {
		SpammyReport("Loading StockItem models (%i)\n", i);
		DVD_LoadFile(DVDClass, "Object", stockItemModels[i], 0);
		SpammyReport("Loading StockItem anims (%i)\n", i);
		DVD_LoadFile(DVDClass, "WorldMap", stockItemAnims[i], 0);
	}

	// map-specific res, like sand geysers or the w9 gates
	int world = dScWMap_c::mWorldNo;
	for (int i = 0; i < mapResCounts[world]; i++) {
		if (mapRes[world][i] == 0)
			break;

		SpammyReport("Loading map-specific model (%i, %s)\n", i, mapRes[world][i]);
		DVD_LoadFile(DVDClass, "WorldMap", mapRes[world][i], 0);
	}

	// koopaling for the current world, ambush enemy, bowser jr if needed
	for (int i = 0; i < enemyResCounts[world]; i++) {
		if (enemyRes[world][i] == 0)
			break;

		SpammyReport("Loading enemy model (%i, %s)\n", i, enemyRes[world][i]);
		DVD_LoadFile(DVDClass, "Object", enemyRes[world][i], 0);
	}

	// world select islands
	for (int i = 0; i < 9; i++) {
		SpammyReport("Loading World Select islands (%i)\n", i);
		char buffer[20];
		snprintf(buffer, 6, "WS_W%X", i+1);
		DVD_LoadFile(DVDClass, "WorldMap", buffer, 0);
	}

	SpammyReport("All done!\n");
	return true;
}

int UnlinkWorld();
void FreeWMScene(void *classDoesntMatter, int id);

int dScWMap_c::onDelete() {
	if (!UnlinkWorld())
		return false;
	
	CleanUpEffectThings();
	FreeEffects(1);
	FreeBreff(1);
	FreeBreft(1);

	FreeWMScene(this, 0);
	FreeWMScene(this, 1);

	// delete all of our resources
	DVD_FreeFile(GetDVDClass2(), mWmArcName);

	for (int i = 0; i < 0x10; i++)
		DVD_FreeFile(GetDVDClass2(), commonRes[i]);

	for (int i = 0; i < STOCK_POWERUP_COUNT; i++) {
		DVD_FreeFile(GetDVDClass2(), stockItemModels[i]);
		DVD_FreeFile(GetDVDClass2(), stockItemAnims[i]);
	}

	int world = dScWMap_c::mWorldNo;
	for (int i = 0; i < mapResCounts[world]; i++) {
		if (mapRes[world][i] == 0)
			break;

		DVD_FreeFile(GetDVDClass2(), mapRes[world][i]);
	}
	
	for (int i = 0; i < enemyResCounts[world]; i++) {
		if (enemyRes[world][i] == 0)
			break;

		DVD_FreeFile(GetDVDClass2(), enemyRes[world][i]);
	}
	
	// routeinfos
	for (int i = 0; i < 9; i++) {
		csvDatas[0][i] = 0;
		csvDatas[1][i] = 0;
		csvDatas[2][i] = 0;
	}

	for (int i = 0; i < 9; i++) {
		char buffer[20];
		snprintf(buffer, 6, "WS_W%X", i+1);
		DVD_FreeFile(GetDVDClass2(), buffer);
	}

	dWmEffectManager_c::instance->destroy();
	dWmSeManager_c::instance->destroy();
	CleanUpEffectThings();

	return true;
}