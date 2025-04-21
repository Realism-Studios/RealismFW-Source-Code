#include <game.hpp>
#include <sfx.hpp>
#include "stockItemArrays.hpp"

int dStockItem_c::onCreate() {
	if (!layoutLoaded) {
		if (!this->loadLayout())
			return false;

		for (int i = 0; i < POWERUP_COUNT; i++) {
			newCounts[i] = dInfo_c::instance->getStockItem(i);
			newIconPictures[i]->SetVisible(false);
		}
		
		// set some stuff
		_8A8 = 0;
		layoutLoaded = true;
		show = false;
		effectIDs[0] = 4;
		effectIDs[1] = 4;
		effectIDs[2] = 4;
		effectIDs[3] = 4;
		_8D0 = 14;
		layout.drawOrder = 15;

		rootPane->SetVisible(false);

		// hide lemmy flower until a certain level is cleared
		SaveBlock *save = GetSaveFile()->GetBlock(-1);
		nw4r::lyt::TextBox *T_locked_09 = layout.findTextBoxByName("T_locked_09");
		T_locked_09->SetVisible(false); // hide it

		// set savefile flag
		u32 conds = save->GetLevelCondition(3, 2); // 4-3
		if (conds & COND_NORMAL) save->lemmySuitUnlocked = true;

		if (!save->lemmySuitUnlocked)
			T_locked_09->SetVisible(true);
	}

	return true;
}

bool dStockItem_c::loadLayout() {
	if (!layout.loadArc("stockItem.arc", false))
		return false;
	
	layout.build("stockItem_27.brlyt");

	layout.loadAnimations(stockItemBrlanNames, 6);
	layout.loadGroups(stockItemGroupNames, stockItemGroupIDs, STOCK_NUM_ANIMS);

	rootPane = layout.layout.rootPane;
	layout.setLangStrings(bmgTextBoxes, bmgIDs, 2, 2);

	// don't touch these if adding powerups!
	layout.getPanes(stockItemPaneNames, &N_forUse_PPos[0], 14);
	layout.getPictures(stockItemPictureNames, &P_icon[0], 24);

	// new stuff
	for (int i = 0; i < POWERUP_COUNT; i++) {
		char name[24];
		sprintf(name, "P_buttonBase_%02d", i);
		newButtonBase[i] = layout.findPictureByName(name);

		sprintf(name, "N_icon_%02d", i);
		newIconPanes[i] = layout.findPaneByName(name);

		sprintf(name, "P_icon_%02d", i);
		newIconPictures[i] = layout.findPictureByName(name);
	}

	if (!IsWideScreen()) {
		rootPane->scale.x = 0.85f;
		rootPane->scale.y = 0.85f;

		nw4r::lyt::Pane *N_frameStock_00 = layout.findPaneByName("N_frameStock_00");
		N_frameStock_00->scale.x = 0.85f;
		N_frameStock_00->scale.y = 0.85f;
	}

	return true;
}


void CrapUpPositions(Vec *out, const Vec *in);

void dStockItem_c::setScalesOfSomeThings() {
	nw4r::lyt::Pane *ppos = N_forUse_PPos[playerCount];

	int howManyPlayers = 0;
	for (int i = 0; i < 4; i++) {
		if (isPlayerActive[i]) {
			int picID = getIconPictureIDforPlayer(howManyPlayers);
			int charID = Player_ID[i];

			if (picID != 24) {
				nw4r::lyt::Picture *pic = P_icon[picID];
				Vec in, out;

				in.x = pic->effectiveMtx[0][3];
				in.y = pic->effectiveMtx[1][3];
				in.z = pic->effectiveMtx[2][3];

				CrapUpPositions(&out, &in);

				u8 *wmp = (u8*)player2d[charID];
				*((float*)(wmp+0xAC)) = out.x;
				*((float*)(wmp+0xB0)) = out.y;
				*((float*)(wmp+0xB4)) = out.z;
				*((float*)(wmp+0x220)) = 0.89999998f;
				*((float*)(wmp+0x224)) = 0.89999998f;
				*((float*)(wmp+0x228)) = 0.89999998f;
				*((float*)(wmp+0x25C)) = 26.0f;
			}
			howManyPlayers++;
		}
	}


	for (int i = 0; i < POWERUP_COUNT; i++) {
		daWMItem_c *item = newItemPtr[i];
		daWMItem_c *lemmy = newItemPtr[9]; // lemmy suit

		nw4r::lyt::Pane *icon = newIconPanes[i];

		Vec in, out;
		in.x = icon->effectiveMtx[0][3];
		in.y = icon->effectiveMtx[1][3];
		in.z = icon->effectiveMtx[2][3];

		CrapUpPositions(&out, &in);

		item->pos.x = out.x;
		item->pos.y = out.y;
		item->pos.z = out.z;

		// different scales depending on the aspect ratio
		if (IsWideScreen()) {
			item->itemScale.x = P_buttonBase[i]->scale.x;
			item->itemScale.y = P_buttonBase[i]->scale.y;
			item->itemScale.z = 1.0f;
		} else {
			item->itemScale.x = 0.75f;
			item->itemScale.y = 0.75f;
			item->itemScale.z = 0.75f;
		}

		// hide the lemmy flower if a certain level has not been cleared
		SaveBlock *save = GetSaveFile()->GetBlock(-1);
		if (!save->lemmySuitUnlocked) {
			lemmy->itemScale.x = 0.0f;
			lemmy->itemScale.y = 0.0f;
			lemmy->itemScale.z = 1.0f;
		}
	}


	nw4r::lyt::Pane *shdRoot = shadow->rootPane;
	shdRoot->trans.x = N_stockItem->effectiveMtx[0][3];
	shdRoot->trans.y = N_stockItem->effectiveMtx[1][3];
	shdRoot->trans.z = N_stockItem->effectiveMtx[2][3];
	shdRoot->scale.x = N_stockItem_01->effectiveMtx[0][0];
	shdRoot->scale.y = N_stockItem_01->effectiveMtx[1][1];

	for (int i = 0; i < POWERUP_COUNT; i++)
		shadow->newButtonBase[i]->scale = newButtonBase[i]->scale;
}


void dStockItem_c::updateCursor() {
	UpdateSelectCursor(newButtonBase[selectedItem], 0, false);
}

void dStockItem_c::updateSelectOrder(int *count) {
	int selectedMaybe;

	if (difficultySetting == DIFFICULTY_GOODLY_LUCK) {
		selectedMaybe = GenerateRandomNumber(POWERUP_COUNT);
		*count = selectedMaybe;
		return;
	}

	if (actionTaken == 1) { //going left
		while (*count > -1) {
			switch (*count) {
				default:
					selectedMaybe = *count - 1;
					*count = selectedMaybe;
					break;
				case 0: //mushroom
					selectedMaybe = 6;
					*count = 6; //go to star
					break;
				case 5: //mini
					selectedMaybe = 7;
					*count = 7; //go to hammer
					break;
				case 7: //hammer
					selectedMaybe = 9;
					*count = 9; //go to boomerang
					break;
				case 8: //Bubble
					selectedMaybe = 4;
					*count = 4; //go to pengi
					break;
			}
			if ((selectedMaybe > -1) && (newCounts[selectedMaybe] != 0)) {
				return;
			}
		}
	} else { //going right
		while (*count < POWERUP_COUNT) {
			switch (*count) {
				default:
					selectedMaybe = *count + 1;
					*count = selectedMaybe;
					break;
				case 4: //pengi
					selectedMaybe = 8;
					*count = 8; //go to bubble
					break;
				case 7: //hammer
					selectedMaybe = 5;
					*count = 5; //go to mini
					break;
				case 9: //boomer
					selectedMaybe = 7;
					*count = 7; //go to hammer
					break;
				case 6: //Star
					selectedMaybe = 0;
					*count = 0; //go to mushroom
					break;
			}
			if ((selectedMaybe < POWERUP_COUNT) && (newCounts[selectedMaybe] != 0)) {
				return;
			}
		}
	}
	*count = selectedItem;
	MapSoundPlayer(SoundRelatedClass, SE_SYS_INVALID, 1);
	return;
}

void playPowerupSound(int usedItem) {
	int sound = 0;

	// star (6) is handled separately, it can't be used here
	switch (usedItem) {
		default:
			sound = SE_PLY_CS_CHANGE_BIG;
			break;
		case 2:
			sound = SE_PLY_CS_CHANGE_PRPL;
			break;
		case 4:
			sound = SE_PLY_CS_CHANGE_PNGN;
			break;
		case 5:
			sound = SE_PLY_CS_CHANGE_MAME;
			break;
		case 7:
			sound = SE_OBJ_HAMMER_HIT_BOTH;
			break;
		case 8:
			sound = SE_PLY_BALLOON_SELF;
			break;
		case 9:
			sound = SE_BOSS_LEMMY_BALL_BOUND;
			break;
	}

	MapSoundPlayer(SoundRelatedClass, sound, 1);
}


void dStockItem_c::initializeState_WindowOpenAnimeEndWait() {
	layout.disableAllAnimations();

	// reset retail anims
	for (int i = 1; i < 8; i++)
		layout.resetAnim(i);
	// and new anims
	for (int i = 0; i < NEW_POWERUP_COUNT; i++) {
		int anim = i * 3 + 30;
		layout.resetAnim(anim);
	}

	layout.enableNonLoopAnim(STOCK_IN_WINDOW);
	rootPane->SetVisible(true);

	for (int i = 0; i < POWERUP_COUNT; i++) {
		newItemPtr[i]->isVisible = true;
		newItemPtr[i]->setupModelThings();
		newItemPtr[i]->animIDToSwitchTo = (newCounts[i] == 0) ? 1 : 0;
	}
	SndSceneMgr::instance->openWindow(4);

	// originally part of executeState_WindowOpenInit()
	for (int i = 0; i < POWERUP_COUNT; i++)
		storeCountToShadow(newCounts[i], i);
}

void dStockItem_c::executeState_WindowOpenAnimeEndWait() {
	if (layout.isAnimOn(-1))
		return;

	int prevItem = dInfo_c::instance->someItemID;

	// no previous button or we're out of that item
	if (prevItem < 0 || newCounts[prevItem] == 0) {
		selectedItem = -1;

		for (int i = 0; i < POWERUP_COUNT; i++) {
			if (newCounts[i] != 0) {
				selectedItem = i;
				break;
			}
		}

		if (selectedItem < 0)
			state.setState(&StateID_ItemSelectWait);
		else
			state.setState(&StateID_ButtonChangeAnimeEndWait);

	} else {
		selectedItem = prevItem;
		state.setState(&StateID_ButtonChangeAnimeEndWait);
	}
}

void dStockItem_c::initializeState_ButtonChangeAnimeEndWait() {
	if (previousItem > -1) {
		int animID = previousItem + 22;
		if (previousItem > 6)
			animID = (previousItem - 7) * 3 + 32;
		layout.enableNonLoopAnim(animID);

		MapSoundPlayer(SoundRelatedClass, SE_SYS_CURSOR, 1);
		CancelSelectCursor(dSelectCursor_c::instance, 0);
		if (previousItem == 0)
			someAnimID = 22;
	}

	int onAnim = selectedItem + 1;
	if (selectedItem > 6)
		onAnim = (selectedItem - 7) * 3 + 30;
	layout.enableNonLoopAnim(onAnim);
}

void dStockItem_c::initializeState_HitAnimeEndWait() {
	useItem();
	setupButtonEffect();

	int animID = selectedItem + 15;
	if (selectedItem > 6)
		animID = (selectedItem - 7) * 3 + 31;
	layout.enableNonLoopAnim(animID);
}

void dStockItem_c::finalizeState_WindowCloseAnimeEndWait() {
	for (int i = 0; i < 3; i++) {
		if (isPlayerActive[i])
			player2d[Player_ID[i]]->_269 = 1;
	}

	for (int i = 0; i < POWERUP_COUNT; i++) {
		newItemPtr[i]->isVisible = false;
		newItemPtr[i]->setupModelThings();
	}

	shadow->visible = false;
	SndSceneMgr::instance->closeWindow();
}



//////////////////////////////////////////////////
// Stock Item Shadow
//////////////////////////////////////////////////

bool dStockItemShadow_c::loadLayout() {
	if (!layout.loadArc("stockItemShadow.arc", false))
		return false;

	layout.build("stockItemShadow_00.brlyt");

	rootPane = layout.layout.rootPane;

	// original panes
	layout.getTextBoxes(shadowTextboxNames, &textBoxes[0], 14);
	layout.getPictures(shadowPictureNames, &buttonBases[0], 7);

	// new
	for (int i = 0; i < POWERUP_COUNT; i++) {
		char name[24];
		sprintf(name, "T_x_%02d", i);
		newX[i] = layout.findTextBoxByName(name);

		sprintf(name, "T_itemRest_%02d", i);
		newValue[i] = layout.findTextBoxByName(name);

		sprintf(name, "P_buttonBaseS_%02d", i);
		newButtonBase[i] = layout.findPictureByName(name);
	}

	if (!IsWideScreen()) {
		nw4r::lyt::Pane *N_ItemShadow_00 = layout.findPaneByName("N_ItemShadow_00");
		N_ItemShadow_00->scale.x = 0.85f;
		N_ItemShadow_00->scale.y = 0.85f;
	}

	return true;
}

void dStockItemShadow_c::update() {
	if (!needsUpdate)
		return;

	needsUpdate = false;

	GXColor normal = {0xFF, 0xFF, 0xFF, 0xFF};
	GXColor locked = {0x46, 0x46, 0x46, 0xFF};

	for (int i = 0; i < POWERUP_COUNT; i++) {
		int value = counts[i];

		newButtonBase[i]->SetVisible(value == 0);

		if (value == 0) {
			newX[i]->colour2 = locked;
			newValue[i]->colour2 = locked;
		} else {
			newX[i]->colour2 = normal;
			newValue[i]->colour2 = normal;
		}

		int fieldLength = 2;
		WriteNumberToTextBox(&value, &fieldLength, newValue[i], true);
	}
}