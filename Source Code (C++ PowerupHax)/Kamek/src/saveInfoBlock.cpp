#include "saveInfoBlock.hpp"

/*const char *SaveInfoDataFileList[] = {0};
const SpriteData SaveInfoDataSpriteData = {ProfileId::SaveInfoData, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile SaveInfoDataProfile(&dSaveInfoData_c::build, SpriteId::SaveInfoData, &SaveInfoDataSpriteData, ProfileId::AC_4SWICHAND, ProfileId::SaveInfoData, "SaveInfoData", SaveInfoDataFileList);

dSaveInfoData_c *dSaveInfoData_c::instance = 0;

dActor_c *dSaveInfoData_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dSaveInfoData_c));
	dSaveInfoData_c *c = new(buffer) dSaveInfoData_c;

	instance = c;
	return c;
}

int dSaveInfoData_c::onCreate() {
	if (!layoutLoaded) {
		if (!layout.loadArc("saveInfoData.arc", false))
			return false;

		layout.build("saveInfoData.brlyt");
		layout.drawOrder = 21;

		layoutLoaded = true;
	}
	visible = false;

	return true;
}

int dSaveInfoData_c::onExecute() {
	layout.update();

	return true;
}

int dSaveInfoData_c::onDraw() {
	if (visible) {
		layout.scheduleForDrawing();
	}

	return true;
}

int dSaveInfoData_c::onDelete() {
	return layout.free();
}

void dSaveInfoData_c::show() {
	visible = true;
}*/

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// BEGIN MANAGER SPRITE: this sprite controls the base of the layout

const char *SaveInfoManagerFileList[] = {0};
const SpriteData SaveInfoManagerSpriteData = {ProfileId::SaveInfoManager, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile SaveInfoManagerProfile(&dSaveInfoManager_c::build, SpriteId::SaveInfoManager, &SaveInfoManagerSpriteData, ProfileId::AC_BG_LAVA, ProfileId::SaveInfoManager, "SaveInfoManager", SaveInfoManagerFileList);

dSaveInfoManager_c *dSaveInfoManager_c::instance = 0;

dActor_c *dSaveInfoManager_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dSaveInfoManager_c));
	dSaveInfoManager_c *c = new(buffer) dSaveInfoManager_c;

	instance = c;
	return c;
}

CREATE_STATE(dSaveInfoManager_c, Hidden);
CREATE_STATE(dSaveInfoManager_c, OpenWait);
CREATE_STATE(dSaveInfoManager_c, Wait);
CREATE_STATE(dSaveInfoManager_c, HideWait);

int dSaveInfoManager_c::onCreate() {
	if (!layoutLoaded) {
		if (!layout.loadArc("saveInfoBase.arc", false))
			return false;
		
		layout.build("saveInfoBase.brlyt");

		static const char *brlanNames[] = {
			"saveInfoBase_inWindow.brlan",
			"saveInfoBase_outWindow.brlan",
			// todo: add before/next page anims
		};

		static const char *groupNames[] = {
			"A00_window", "A00_window",
		};
		static const int groupIDs[] = {0, 1};

		layout.loadAnimations(brlanNames, 2);
		layout.loadGroups(groupNames, groupIDs, 2);
		layout.disableAllAnimations();

		layout.drawOrder = 20;

		// get panes
		static const char *picNames[] = {
			"P_rankStarS_00", "P_rankStarS_01",
			"P_rankStarS_02", "P_rankStarS_03",
			"P_rankStarS_04", "P_rankStar_00",
			"P_rankStar_01",  "P_rankStar_02",
			"P_rankStar_03",  "P_rankStar_04",
		};
		layout.getPictures(picNames, &P_rankStarS_0[0], 10);

		T_totalCompS_00 = layout.findTextBoxByName("T_totalCompS_00");
		T_totalComp_00 = layout.findTextBoxByName("T_totalComp_00");

		layoutLoaded = true;
	}
	
	visible = false;

	return true;
}

int dSaveInfoManager_c::onExecute() {
	state.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int dSaveInfoManager_c::onDraw() {
	if (visible) {
		layout.scheduleForDrawing();
	}

	return true;
}

int dSaveInfoManager_c::onDelete() {
	instance = 0;

	if (PauseManager_c::instance)
		PauseManager_c::instance->disablePause = false; // Enable pausing

	return layout.free();
}

void dSaveInfoManager_c::showMenu() {
	state.setState(&StateID_OpenWait);
}

// Colors the max values orange:
// textBoxName->colour1 = (GXColor){247,91,0,255};
// textBoxName->colour2 = (GXColor){255,219,112,255};

// Begin Data functions

void dSaveInfoManager_c::setupCompletionStars() {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);

	// Hide them all first
	for (int i = 0; i < 5; i++) {
		P_rankStar_0[i]->SetVisible(false);
	}

	// Now use the completion bitfield to show the stars

	if (save->bitfield & SAVE_BIT_GAME_COMPLETE)
		P_rankStar_0[0]->SetVisible(true);
    if (save->bitfield & SAVE_BIT_ALL_EXITS)
		P_rankStar_0[1]->SetVisible(true);
    if (save->bitfield & SAVE_BIT_ALL_STAR_COINS)
		P_rankStar_0[2]->SetVisible(true);
    if (save->bitfield & SAVE_BIT_ALL_STAR_COINS_W9)
		P_rankStar_0[3]->SetVisible(true);
    if (save->bitfield & SAVE_BIT_EVERYTHING_TRULY_DONE)
		P_rankStar_0[4]->SetVisible(true);

	// Star sparkle effect was removed, so this is no longer needed (oh btw it doesn't work)
	/*if ((save->bitfield & 0x40) != 0) { // No Super Guide blocks have appeared throughout the game
		for (int i = 0; i < 5; i++) {
			VEC3 efPos = {P_rankStar_0[i]->effectiveMtx[0][3], P_rankStar_0[i]->effectiveMtx[1][3], 0.0f};
			VEC3 efScale = {1.0f, 1.0f, 1.0f};
			SpawnEffect("Wm_2d_titlestar01", 0, &efPos, 0, &efScale);
			SpawnEffect("Wm_2d_titlestar02", 0, &efPos, 0, &efScale);
		}
	}*/
}

void dSaveInfoManager_c::calculateCompletionPercentage() {
	/* how to go about this:
	have a maximum value that equals an amount
	there will be a value that will add the values for each thing in the save file that's 100% comp related
	[X] do math to turn that value to a percentage (current value / maximum value * 100)
	[X] then write it to the text box
	*/

	// do percentage calculation
	int result = calculatePercentage(91, 396, true);
	WriteNumberToTextBox(&result, T_totalCompS_00, false);
	WriteNumberToTextBox(&result, T_totalComp_00, false);
}


// Menu is not open
void dSaveInfoManager_c::beginState_Hidden() { }
void dSaveInfoManager_c::executeState_Hidden() { }
void dSaveInfoManager_c::endState_Hidden() { }


// Menu is opening
void dSaveInfoManager_c::beginState_OpenWait() {
	visible = true;
	//data.visible = true;
	SndSceneMgr::instance->openWindow(8);
	PauseManager_c::instance->disablePause = true; // Disable pausing
	WLClass::instance->demoControlAllPlayers(); // Disable player control

	calculateCompletionPercentage();

	layout.enableNonLoopAnim(IN_WINDOW);
	// Looping Checker Anim played here

	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_DIALOGUE_IN, 0);
}
void dSaveInfoManager_c::executeState_OpenWait() {
	if (!layout.isAnimOn(IN_WINDOW))
		state.setState(&StateID_Wait);
}
void dSaveInfoManager_c::endState_OpenWait() { }


// Menu is fully opened
void dSaveInfoManager_c::beginState_Wait() { }
void dSaveInfoManager_c::executeState_Wait() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	if (nowPressed & WPAD_LEFT) { }

	if (nowPressed & WPAD_RIGHT) { }

	if (nowPressed & WPAD_ONE) {
		state.setState(&StateID_HideWait);
	}
}
void dSaveInfoManager_c::endState_Wait() { }


// Menu is closing
void dSaveInfoManager_c::beginState_HideWait() {
	layout.enableNonLoopAnim(OUT_WINDOW);

	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_BACK, 0);
}
void dSaveInfoManager_c::executeState_HideWait() {
	if (!layout.isAnimOn(OUT_WINDOW))
		state.setState(&StateID_Hidden);
}
void dSaveInfoManager_c::endState_HideWait() {
	visible = false;
	//data.visible = false;
	PauseManager_c::instance->disablePause = true; // Enable pausing
	WLClass::instance->disableDemoControl(false); // Restore player control
	SndSceneMgr::instance->closeWindow();
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// BEGIN THE BLOCK SPRITE: this sprite is the block that triggers the menu

const char *SaveInfoBlockFileList[] = {"block_saveInfo", 0};
const SpriteData SaveInfoBlockSpriteData = {ProfileId::SaveInfoBlock, 8, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile SaveInfoBlockProfile(&dSaveInfoBlock_c::build, SpriteId::SaveInfoBlock, &SaveInfoBlockSpriteData, ProfileId::AC_BG_WATER, ProfileId::SaveInfoBlock, "SaveInfoBlock", SaveInfoBlockFileList);

dSaveInfoBlock_c *dSaveInfoBlock_c::instance = 0;

dActor_c *dSaveInfoBlock_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dSaveInfoBlock_c));
	dSaveInfoBlock_c *c = new(buffer) dSaveInfoBlock_c;

	instance = c;
	return c;
}

CREATE_STATE(dSaveInfoBlock_c, Wait);

int dSaveInfoBlock_c::onCreate() {
    mAllocator.link(-1, GameHeaps[0], 0, 0x20);

    // Setup the model
    nw4r::g3d::ResFile resFile(getResource("block_saveInfo", "g3d/block_saveInfo.brres"));
	nw4r::g3d::ResMdl resMdl = resFile.GetResMdl("block_saveInfo");
	model.setup(resMdl, &mAllocator, 0x224, 1, 0);
    SetupTextures_Enemy(&model, 0);

	// Setup shine animation
	nw4r::g3d::ResAnmTexSrt anmRes = resFile.GetResAnmTexSrt("block_saveInfo");
	anmTexSRT.setup(resMdl, anmRes, &mAllocator, 0, 1);
	anmTexSRT.bindEntry(&model, anmRes, 0, 0);
	model.bindAnim(&anmTexSRT, 0.0);

	mAllocator.unlink();

    blockInit(pos.y);

	// Setup physics
	physicsInfo.x1 = -16;
	physicsInfo.y1 = 16;
	physicsInfo.x2 = 16;
	physicsInfo.y2 = -16;

	physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.flagsMaybe = 0x260;
	physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
	physics.addToList();

    this->scale.x = 1.0;
	this->scale.y = 1.0;
	this->scale.z = 1.0;
	
	doStateChange(&dSaveInfoBlock_c::StateID_Wait);

    return true;
}

void dSaveInfoBlock_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);

	model.setDrawMatrix(matrix);
	model.setScale(&scale);
	model.calcWorld(false);
}

int dSaveInfoBlock_c::onExecute() {	
	acState.execute();
	physics.update();
	blockUpdate();

	updateModelMatrices();
	anmTexSRT.process();

	if (acState.getCurrentState()->isEqual(&StateID_Wait)) {
		checkZoneBoundaries(0);
	}


	return true;
}

int dSaveInfoBlock_c::onDraw() {
	model.scheduleForDrawing();
	return true;
}

int dSaveInfoBlock_c::onDelete() {
	physics.removeFromList();
	return true;
}

void dSaveInfoBlock_c::blockWasHit(bool isDown) {
	pos.y = initialY;

	if (dSaveInfoManager_c::instance)
		dSaveInfoManager_c::instance->showMenu();
	else
		Delete(false);

	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.addToList();

	doStateChange(&StateID_Wait);
}



void dSaveInfoBlock_c::calledWhenUpMoveExecutes() {
	if (initialY >= pos.y)
		blockWasHit(false);
}

void dSaveInfoBlock_c::calledWhenDownMoveExecutes() {
	if (initialY <= pos.y)
		blockWasHit(true);
}



void dSaveInfoBlock_c::beginState_Wait() { }

void dSaveInfoBlock_c::executeState_Wait() {
	int result = blockResult();

	if (result == 0)
		return;

	if (result == 1) {
		doStateChange(&daEnBlockMain_c::StateID_UpMove);
		anotherFlag = 2;
		isGroundPound = false;
	} else {
		doStateChange(&daEnBlockMain_c::StateID_DownMove);
		anotherFlag = 1;
		isGroundPound = true;
	}
}

void dSaveInfoBlock_c::endState_Wait() { }
