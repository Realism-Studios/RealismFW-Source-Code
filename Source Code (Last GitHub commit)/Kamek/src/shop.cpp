#include "shop.hpp"

const char *ShopFileList[] = {0};
const SpriteData ShopSpriteData = {ProfileId::Shop, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile ShopProfile(&dWMShop_c::build, SpriteId::Shop, &ShopSpriteData, ProfileId::AC_BG_POISON, ProfileId::Shop, "Shop", ShopFileList);

dWMShop_c *dWMShop_c::instance = 0;

dActor_c *dWMShop_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMShop_c));
	dWMShop_c *c = new(buffer) dWMShop_c;

	instance = c;
	return c;
}

CREATE_STATE(dWMShop_c, Hidden);
CREATE_STATE(dWMShop_c, ShowWait);
CREATE_STATE(dWMShop_c, ButtonActivateWait);
CREATE_STATE(dWMShop_c, CoinCountdown);
CREATE_STATE(dWMShop_c, Wait);
CREATE_STATE(dWMShop_c, HideWait);


int dWMShop_c::onCreate() {
	PauseManager_c::instance->disablePause = true; // Disable pausing
	WLClass::instance->demoControlAllPlayers(); // Disable player control

	// spritedata
	itemSet = (settings & 0xF0000000) >> 28;  // nybble 5
	lakituTex = (settings & 0xF000000) >> 24; // nybble 6
	hideCursor = ((settings & 0x1) != 0); // nybble 12, value 1
	rndItems = ((settings & 0x2) != 0);   // nybble 12, value 2

	if (rndItems)
		itemSet = GenerateRandomNumber(MAX_INVENTORY_COUNT);

	// setup the layout
	if (!layoutLoaded) {
		bool gotFile = layout.loadArc("shop.arc", false);
		if (!gotFile)
			return false;

		selected = 0;
		lastTopRowChoice = 0;

		layout.build("shop.brlyt");

		if (IsWideScreen()) {
			layout.layout.rootPane->scale.x = 0.735f;
		} else {
			layout.clippingEnabled = true;
			layout.clipX = 0;
			layout.clipY = 52;
			layout.clipWidth = 640;
			layout.clipHeight = 352;
			layout.layout.rootPane->scale.x = 0.731f;
			layout.layout.rootPane->scale.y = 0.7711f;
		}

		layout.drawOrder = 18; // draws above gameScene layout

		static const char *brlanNames[] = {
			"shop_Show.brlan",
			"shop_Hide.brlan",
			"shop_ActivateButton.brlan",
			"shop_DeactivateButton.brlan",
			"shop_CountCoin.brlan"
		};
		static const char *groupNames[] = {
			"BaseGroup", "BaseGroup",
			"GBtn00", "GBtn01", "GBtn02", "GBtn03", "GBtn1", "GBtn2",
			"GBtn00", "GBtn01", "GBtn02", "GBtn03", "GBtn1", "GBtn2",
			"GCoinCount"
		};
		static const int brlanIDs[] = {
			0, 1,
			2, 2, 2, 2, 2, 2,
			3, 3, 3, 3, 3, 3,
			4
		};

		layout.loadAnimations(brlanNames, 5);
		layout.loadGroups(groupNames, brlanIDs, 15);
		layout.disableAllAnimations();

		static const char *tbNames[] = {
			"Title", "TitleShadow",
			"CoinCount", "CoinCountShadow",
			"BackText", "BuyText",
		};
		layout.getTextBoxes(tbNames, &Title, 6);

		// Warning: weird code coming up
		const char *crap = "000102031\0" "2\0";
		char name[12];
		for (int i = 0; i < 6; i++) {
			strcpy(name, "BtnLeftXX");
			name[7] = crap[i*2];
			name[8] = crap[i*2+1];
			BtnLeft[i] = layout.findPictureByName(name);

			strcpy(name, "BtnMidXX");
			name[6] = crap[i*2];
			name[7] = crap[i*2+1];
			BtnMid[i] = layout.findPictureByName(name);

			strcpy(name, "BtnRightXX");
			name[8] = crap[i*2];
			name[9] = crap[i*2+1];
			BtnRight[i] = layout.findPictureByName(name);

			strcpy(name, "BtnXX");
			name[3] = crap[i*2];
			name[4] = crap[i*2+1];
			Buttons[i] = layout.findPaneByName(name);
		}

		Btn1Base = layout.findPaneByName("Btn1_Base");
		Btn2Base = layout.findPaneByName("Btn2_Base");

		// item icon panes
		static const char *itemPanes[] = {
			"P_item_00", "P_item_01", "P_item_02", "P_item_03",
			"P_item_10", "P_item_11", "P_item_12",
			"P_item_20", "P_item_21", "P_item_22", "P_item_23", "P_item_24",
		};
		layout.getPictures(itemPanes, &P_item[0], 12);

		// lakitu
		for (int i = 0; i < 6; i++) {
			char name[16];
			sprintf(name, "P_lakitu_0%i", i);
			P_lakitu_0[i] = layout.findPictureByName(name);
		}

		layoutLoaded = true;
	}

	showLayout();

	return true;
}


int dWMShop_c::onDelete() {
	return layout.free();
}


int dWMShop_c::onExecute() {
	state.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int dWMShop_c::onDraw() {
	if (visible)
		layout.scheduleForDrawing();

	return true;
}


void dWMShop_c::showLayout() {
	state.setState(&StateID_ShowWait);
}



// Hidden
void dWMShop_c::beginState_Hidden() { }
void dWMShop_c::executeState_Hidden() { }
void dWMShop_c::endState_Hidden() { }


// ShowWait
void dWMShop_c::beginState_ShowWait() {
	MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_IN, 1);

	layout.disableAllAnimations();
	layout.enableNonLoopAnim(SHOW_ALL);
	visible = true;

	loadInfo();
	loadItemIcons();
}
void dWMShop_c::executeState_ShowWait() {
	if (!layout.isAnimOn(SHOW_ALL)) {
		selected = 0;
		layout.enableNonLoopAnim(ACTIVATE_BUTTON);
		state.setState(&StateID_ButtonActivateWait);
	}
}
void dWMShop_c::endState_ShowWait() { }


// ButtonActivateWait
void dWMShop_c::beginState_ButtonActivateWait() { }
void dWMShop_c::executeState_ButtonActivateWait() {
	if (!layout.isAnyAnimOn())
		state.setState(&StateID_Wait);
}
void dWMShop_c::endState_ButtonActivateWait() { }


// Wait
void dWMShop_c::beginState_Wait() {
	showSelectCursor();
}
void dWMShop_c::executeState_Wait() {
	if (layout.isAnyAnimOn())
		return;
	
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	int newSelection = -1;

	if (nowPressed & WPAD_ONE) {
		// Hide the thing
		state.setState(&StateID_HideWait);

	} else if (nowPressed & WPAD_UP) {
		// Move up
		if (selected == 4)
			newSelection = lastTopRowChoice;
		else if (selected == 5)
			newSelection = 4;

	} else if (nowPressed & WPAD_DOWN) {
		// Move down
		if (selected <= 3)
			newSelection = 4;
		else if (selected == 4)
			newSelection = 5;

	} else if (nowPressed & WPAD_LEFT) {
		// Just use the previous choice
		if (selected > 0)
			newSelection = selected - 1;

	} else if (nowPressed & WPAD_RIGHT) {
		// Just use the next choice
		if (selected < 5)
			newSelection = selected + 1;

	} else if (nowPressed & WPAD_TWO) {
		buyItem(selected);
	}

	if (newSelection > -1) {
		MapSoundPlayer(SoundRelatedClass, SE_SYS_CURSOR, 1);

		layout.enableNonLoopAnim(DEACTIVATE_BUTTON+selected);
		layout.enableNonLoopAnim(ACTIVATE_BUTTON+newSelection);

		selected = newSelection;
		if (newSelection <= 3)
			lastTopRowChoice = newSelection;

		showSelectCursor();
	}
}
void dWMShop_c::endState_Wait() { }


// HideWait
void dWMShop_c::beginState_HideWait() {
	MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_OUT_AUTO, 1);
	layout.enableNonLoopAnim(HIDE_ALL);
	layout.enableNonLoopAnim(DEACTIVATE_BUTTON+selected);

	HideSelectCursor(SelectCursorPointer, 0);
}
void dWMShop_c::executeState_HideWait() {
	if (!layout.isAnimOn(HIDE_ALL))
		state.setState(&StateID_Hidden);
}
void dWMShop_c::endState_HideWait() {
	visible = false;
	ExitStage(WORLD_MAP, 0, EXIT_LEVEL, FADE_OUT);
}



// setup functions
void dWMShop_c::loadInfo() {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);

	// Find out the shop name
	dLevelInfo_c::entry_s *shopNameEntry = dLevelInfo_c::s_info.searchBySlot(CurrentWorld, CurrentLevel);

	wchar_t shopName[100];
	const char *sourceName = dLevelInfo_c::s_info.getNameForLevel(shopNameEntry);
	int charCount = 0;
	
	while (*sourceName != 0 && charCount < 99) {
		shopName[charCount] = *sourceName;
		sourceName++;
		charCount++;
	}
	shopName[charCount] = 0;

	Title->SetString(shopName);
	TitleShadow->SetString(shopName);

	// Apply random colors to the Title text and buttons
	randomizeColors(false);

	// Handle lakitu
	for (int i = 0; i < 6; i++)
		P_lakitu_0[i]->SetVisible(false);
	P_lakitu_0[lakituTex]->SetVisible(true);

	// Load the coin count
	int scCount = getUnspentStarCoinCount();
	WriteNumberToTextBox(&scCount, CoinCount, false);
	WriteNumberToTextBox(&scCount, CoinCountShadow, false);

	// Button prompts
	WriteBMGToTextBox(BackText, GetBMG(), 2, 58, 0);
	BuyText->SetString(L"\x0B\x0128\xBEEF Purchase"); // prints 2 button from PictureFont
}


void dWMShop_c::randomizeColors(bool useRandomAlpha) {
	for (int i = 0; i < 8; i++) {
		randomColorValue[i] = GenerateRandomNumber(255);
	}

	if (!useRandomAlpha) {
		randomColorValue[3] = 255;
		randomColorValue[7] = 255;
	}

	GXColor textTopColor = {randomColorValue[0], randomColorValue[1], randomColorValue[2], randomColorValue[3]};
	GXColor textBottomColor = {randomColorValue[4], randomColorValue[5], randomColorValue[6], randomColorValue[7]};

	// apply our colors
	Title->colour1 = textTopColor;
	Title->colour2 = textBottomColor;
	
	
	// color the buttons
	for (int j = 0; j < 6; j++) {

		for (int i = 0; i < 8; i++) {
			randomColorValue[i] = GenerateRandomNumber(255);
		}

		if (!useRandomAlpha) {
			randomColorValue[3] = 255;
			randomColorValue[7] = 255;
		}

		GXColor btnTopColor = {randomColorValue[0], randomColorValue[1], randomColorValue[2], randomColorValue[3]};
		GXColor btnBottomColor = {randomColorValue[4], randomColorValue[5], randomColorValue[6], randomColorValue[7]};

		// put them into the pane vertex colors
		BtnLeft[j]->colours[0] = btnTopColor;
		BtnLeft[j]->colours[1] = btnTopColor;
		BtnLeft[j]->colours[2] = btnBottomColor;
		BtnLeft[j]->colours[3] = btnBottomColor;

		BtnMid[j]->colours[0] = btnTopColor;
		BtnMid[j]->colours[1] = btnTopColor;
		BtnMid[j]->colours[2] = btnBottomColor;
		BtnMid[j]->colours[3] = btnBottomColor;

		BtnRight[j]->colours[0] = btnTopColor;
		BtnRight[j]->colours[1] = btnTopColor;
		BtnRight[j]->colours[2] = btnBottomColor;
		BtnRight[j]->colours[3] = btnBottomColor;
	}
}


// Powerup Listing:
// 0 = Mushroom		- 1
	// 1 = Fireflower	- 2
	// 2 = Iceflower	- 2
		// 3 = Penguin		- 3
		// 4 = Propeller	- 3
	// 5 = MiniShroom	- 2
	// 6 = Starman		- 2
		// 7 = Hammer		- 3
	// 8 = 1-ups		- 2
//
// Format: 1coin, 1coin, 2coins, 3coins, 5coins[3] (Value 6-9), 8coins[5] (Value 10-15)
//
// Possible 5 coin combos =  2,2,2  /  1,2,3  /  2,3,2  /  3,2,3  /  3,3,3
// Possible 8 coin combos =  1,1,2,3,3  /  1,2,2,3,3  /  1,2,3,3,3  /  2,2,2,3,3  /  2,2,3,3,3  /  1,3,3,3,3  /  2,3,3,3,3  /  3,3,3,3,3

const dWMShop_c::ItemTypes dWMShop_c::Inventory[MAX_INVENTORY_COUNT][12] = {
	{ // Set 0
		MUSHROOM, FIRE_FLOWER, ICE_FLOWER, PROPELLER,
		FIRE_FLOWER, ICE_FLOWER, FIRE_FLOWER,
		MUSHROOM, MUSHROOM, ONE_UP, PROPELLER, PROPELLER
	},
	{ // Set 1
		MUSHROOM, FIRE_FLOWER, ICE_FLOWER, PROPELLER,
		FIRE_FLOWER, STARMAN, FIRE_FLOWER,
		MUSHROOM, FIRE_FLOWER, FIRE_FLOWER, PROPELLER, PROPELLER
	},
	{ // Set 2
		MUSHROOM, FIRE_FLOWER, MINI_SHROOM, PROPELLER,
		MUSHROOM, MINI_SHROOM, PROPELLER,
		MUSHROOM, MINI_SHROOM, PROPELLER, PROPELLER, HAMMER
	},
	{ // Set 3
		MUSHROOM, FIRE_FLOWER, ONE_UP, HAMMER,
		ONE_UP, ONE_UP, ONE_UP,
		PROPELLER, ICE_FLOWER, ONE_UP, FIRE_FLOWER, PROPELLER
	},
	{ // Set 4
		MUSHROOM, FIRE_FLOWER, ICE_FLOWER, BUBBLE_FLOWER,
		ICE_FLOWER, BUBBLE_FLOWER, ICE_FLOWER,
		ICE_FLOWER, BUBBLE_FLOWER, BUBBLE_FLOWER, BUBBLE_FLOWER, ICE_FLOWER
	},
	{ // Set 5
		MUSHROOM, FIRE_FLOWER, STARMAN, PROPELLER,
		MINI_SHROOM, PROPELLER, MINI_SHROOM,
		PROPELLER, PROPELLER, MUSHROOM, PROPELLER, PROPELLER
	},
	{ // Set 6
		MUSHROOM, STARMAN, ONE_UP, HAMMER,
		STARMAN, STARMAN, STARMAN,
		HAMMER, HAMMER, ONE_UP, HAMMER, HAMMER
	},
	{ // Set 7
		MUSHROOM, ONE_UP, PROPELLER, HAMMER,
		HAMMER, PROPELLER, HAMMER,
		PROPELLER, HAMMER, PROPELLER, HAMMER, PROPELLER
	},
	/*{ // Set 8
		MUSHROOM, MUSHROOM, MUSHROOM, MUSHROOM,
		MUSHROOM, MUSHROOM, MUSHROOM,
		MUSHROOM, MUSHROOM, MUSHROOM, MUSHROOM, MUSHROOM
	},*/
	{ // Set 8
		MUSHROOM, FIRE_FLOWER, ICE_FLOWER, PROPELLER,
		FIRE_FLOWER, ICE_FLOWER, MINI_SHROOM,
		STARMAN, ONE_UP, MUSHROOM, MUSHROOM, MUSHROOM
	},
	{ // Set 9
		MUSHROOM, FIRE_FLOWER, ONE_UP, BUBBLE_FLOWER,
		FIRE_FLOWER, PROPELLER, FIRE_FLOWER,
		FIRE_FLOWER, FIRE_FLOWER, STARMAN, FIRE_FLOWER, FIRE_FLOWER
	}
};


#include "fileload.hpp"
void dWMShop_c::loadItemIcons() {
	static File itemTPLs[ITEM_COUNT];

	// load all the files
	for (int i = 0; i < ITEM_COUNT; i++) {
		char filename[64];
		sprintf(filename, "/Layout/tecks tore/shop/item_%02X.tpl", (int)Inventory[itemSet][i]);

		if (itemTPLs[i].open(filename)) {
			P_item[i]->material->texMaps[0].ReplaceImage((TPLPalette*)itemTPLs[i].ptr(), 0);
		}
	}
}


void dWMShop_c::buyItem(int item) {
	static int itemDefs[6][3] = {
		// Cost, Start Index, Count
		{1, 0, 1}, {2, 1, 1}, {2, 2, 1}, {3, 3, 1},
		{5, 4, 3}, {8, 7, 5}
	};

	int cost = itemDefs[item][0], cash = getUnspentStarCoinCount();

	if (cost > cash) {
		MapSoundPlayer(SoundRelatedClass, SE_SYS_ROUTE_NG, 1);
		Title->SetString(L"No.");
		return;
	}

	MapSoundPlayer(SoundRelatedClass, SE_SYS_DECIDE, 1);

	SaveFile *file = GetSaveFile();
	SaveBlock *block = file->GetBlock(file->header.current_file);

	coinsRemaining = cost;

	// Work out what we need to apply
	int appliedItems[ITEM_TYPE_COUNT];
	for (int i = 0; i < ITEM_TYPE_COUNT; i++)
		appliedItems[i] = 0;

	int invStartIndex = itemDefs[item][1] - 1;
	int invCount = itemDefs[item][2];
	for (int i = 0; i < invCount; i++)
		appliedItems[(int)Inventory[itemSet][invStartIndex+i]]++;

	for (int i = 0; i < 10; i++) {
		block->new_powerups_available[i] += appliedItems[i];

		if (block->new_powerups_available[i] > 99)
			block->new_powerups_available[i] = 99;
	}

	// Apply lives to everyone
	for (int i = 0; i < 4; i++) {
		if (Player_Active[i]) {
			int id = Player_ID[i];
			Player_Lives[id] += appliedItems[(int)ONE_UP];
			if (Player_Lives[id] > 99)
				Player_Lives[id] = 99;
		}
	}

	if (appliedItems[(int)ONE_UP] > 0)
		MapSoundPlayer(SoundRelatedClass, SE_SYS_100COIN_ONE_UP, 1);

	state.setState(&StateID_CoinCountdown);
	HideSelectCursor(SelectCursorPointer, 0);
}


void dWMShop_c::beginState_CoinCountdown() {
	timerForCoinCountdown = 8;
}
void dWMShop_c::endState_CoinCountdown() { }

void dWMShop_c::executeState_CoinCountdown() {
	timerForCoinCountdown--;
	if (timerForCoinCountdown <= 0) {
		SaveBlock *save = GetSaveFile()->GetBlock(-1);
		save->spentStarCoins++;

		// load the coin count
		int scCount = getUnspentStarCoinCount();
		WriteNumberToTextBox(&scCount, CoinCount, false);
		WriteNumberToTextBox(&scCount, CoinCountShadow, false);

		layout.enableNonLoopAnim(COUNT_COIN);
		VEC3 efPos = {
			CoinCount->effectiveMtx[0][3],
			CoinCount->effectiveMtx[1][3],
			0.0f};

		// ARGHHHHHHHHHHHHHHHHh.
		if (IsWideScreen()) {
			float div = 5.0f;
			if (scCount < 100)
				div = 3.6f;
			if (scCount < 10)
				div = 2.7f;
			efPos.x -= (CoinCount->size.x / div);
			efPos.y -= (CoinCount->size.y / 2.0f);
		} else {
			float div = 5.8f;
			if (scCount < 100)
				div = 8.2f;
			if (scCount < 10)
				div = 14.5f;
			efPos.x += (CoinCount->size.x / div);
			efPos.y -= (CoinCount->size.y / 2.8f);
		}

		VEC3 efScale = {0.7f, 0.7f, 0.7f};
		SpawnEffect("Wm_2d_moviecoinvanish", 0, &efPos, 0, &efScale);

		coinsRemaining--;
		if (coinsRemaining <= 0) {
			MapSoundPlayer(SoundRelatedClass, SE_PLY_GET_ITEM_AGAIN, 1);
			state.setState(&StateID_Wait);
		} else {
			MapSoundPlayer(SoundRelatedClass, SE_SYS_STAR_COIN_PAY, 1);
			beginState_CoinCountdown();
		}
	}
}


void dWMShop_c::showSelectCursor() {
	if (hideCursor != 0)
		return;
	
	// use selectcursor function so pane alpha isn't applied
	dSelectCursor_c *cursor = dSelectCursor_c::instance;

	switch (selected) {
		case 4: cursor->updateCursor(Btn1Base, 0, false); break;
		case 5: cursor->updateCursor(Btn2Base, 0, false); break;
		default: cursor->updateCursor(Buttons[selected], 0, false);
	}
}

