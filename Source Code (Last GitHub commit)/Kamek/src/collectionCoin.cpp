#include <newer.hpp>
#include <sfx.hpp>
#include "levelinfo.hpp"
#include <levelnumber.hpp>
#include <stage.hpp>

CREATE_STATE(dCollectionCoin_c, MenuJump);


extern "C" bool IsCourseClear(int World, int Level);

void dCollectionCoinDate_c::loadLevelData() {
	if (courseNum == Stage_Invalid) {
		rootPane->SetVisible(false);
		return;
	}
	rootPane->SetVisible(true);
	
	// insert levelinfo
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldNum, courseNum);
    if (level) {
        const wchar_t *convWorldName = getWorldNumber(level->displayWorld);
        const wchar_t *convLevelName = getLevelNumber(worldNum, level->displayLevel);

        T_worldNum_00->SetString(convWorldName);

		if (level->displayLevel > 19) {
			T_pictureFont_00->SetVisible(true);
			T_corseNum_00->SetVisible(false);
			T_pictureFont_00->SetString(convLevelName);
		} else {
			T_pictureFont_00->SetVisible(false);
			T_corseNum_00->SetVisible(true);
			T_corseNum_00->SetString(convLevelName);
		}
    } else {
        T_pictureFont_00->SetVisible(false);
		T_corseNum_00->SetVisible(true);
		T_worldNum_00->SetString(L"?");
		T_corseNum_00->SetString(L"?");
    }

	// update fileters
	nw4r::lyt::Picture *P_coinFileter_00 = layout.findPictureByName("P_coinFileter_00");
	nw4r::lyt::Picture *P_coinFileter_01 = layout.findPictureByName("P_coinFileter_01");
	nw4r::lyt::Picture *P_coinFileter_02 = layout.findPictureByName("P_coinFileter_02");

	if (IsCourseClear(worldNum, courseNum)) {
		P_coinFileter_00->SetVisible(false);
		P_coinFileter_01->SetVisible(false);
		P_coinFileter_02->SetVisible(false);
	} else {
		P_coinFileter_00->SetVisible(true);
		P_coinFileter_01->SetVisible(true);
		P_coinFileter_02->SetVisible(true);
	}


	// handle coin visibility
	if (courseConditions < 0) {
		P_coinBase1_00->SetVisible(false);
		P_coin_1_00->SetVisible(false);
		P_coinBase2_00->SetVisible(false);
		P_coin_2_00->SetVisible(false);
		P_coinBase3_00->SetVisible(false);
		P_coin_3_00->SetVisible(false);
	} else {
		if (courseConditions & COND_COIN1) {
			P_coinBase1_00->SetVisible(false);
			P_coin_1_00->SetVisible(true);
		} else {
			P_coinBase1_00->SetVisible(true);
			P_coin_1_00->SetVisible(false);
		}

		if (courseConditions & COND_COIN2) {
			P_coinBase2_00->SetVisible(false);
			P_coin_2_00->SetVisible(true);
		} else {
			P_coinBase2_00->SetVisible(true);
			P_coin_2_00->SetVisible(false);
		}

		if (courseConditions & COND_COIN3) {
			P_coinBase3_00->SetVisible(false);
			P_coin_3_00->SetVisible(true);
		} else {
			P_coinBase3_00->SetVisible(true);
			P_coin_3_00->SetVisible(false);
		}
	}

	// update course fileters
	if (IsCourseClear(worldNum, courseNum)) {
		W_fileter_00->SetVisible(false);
		P_topFileter->SetVisible(false);
	} else {
		W_fileter_00->SetVisible(true);
		P_topFileter->SetVisible(true);
	}
}



static bool doneRandomization = false;

int dCollectionCoinBase_c::onExecute() {
	if ((!layoutLoaded) || (!visible))
		rootPane->SetVisible(false);
	else {
		rootPane->SetVisible(true);
		layout.update();

		if (difficultySetting != DIFFICULTY_GOODLY_LUCK)
			positionCoinBars(); // retail behavior
		else
			randomCoinBarPosition();
	}

	return true;
}

void dCollectionCoinBase_c::randomCoinBarPosition() {
	u8 coinBarSlots[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int result = 0;

	for (int i = 0; i < 12; i++) {
		nw4r::lyt::Pane *dateRootPane = datePtr[i]->rootPane;
		nw4r::lyt::Pane *coinBarDate = datePtr[i]->N_coinBarDate_00;

		rollAgain:
		result = GenerateRandomNumber(12);

		if (coinBarSlots[result] == 1)
			goto rollAgain;

		dateRootPane->scale.x = N_coinBarPos[result]->effectiveMtx[0][0];
		dateRootPane->scale.y = N_coinBarPos[result]->effectiveMtx[1][1];
		dateRootPane->trans.x = N_coinBarPos[result]->effectiveMtx[0][3];
		dateRootPane->trans.y = N_coinBarPos[result]->effectiveMtx[1][3];
		coinBarSlots[result] = 1; // prevent it from being chosen again

		coinBarDate->alpha = N_coinDateAll_00->alpha;
	}
	
	doneRandomization = true;
}



void dCollectionCoin_c::updateWorldNames() {
	nw4r::lyt::TextBox *T_world_00 = layout.findTextBoxByName("T_world_00");
    nw4r::lyt::TextBox *T_world_01 = layout.findTextBoxByName("T_world_01");

    const char *worldname;

    dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(worldNum+1, 100);
    if (world) {
		worldname = dLevelInfo_c::s_info.getNameForLevel(world);
	} else {
		worldname = "World ?";
	}

    wchar_t wbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		wbuffer[i] = (unsigned short)worldname[i];
	}

	T_world_00->SetString(wbuffer);
    T_world_01->SetString(wbuffer);
    return;
}

void dCollectionCoin_c::updateSpecifiedWorldName(int textBoxNum) {
    const char *name;
	name = (textBoxNum == 0) ? "T_world_00" : "T_world_01";
	nw4r::lyt::TextBox *TextBox = layout.findTextBoxByName(name);

    const char *worldname;

    dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(worldNum+1, 100);
    if (world) {
		worldname = dLevelInfo_c::s_info.getNameForLevel(world);
	} else {
		worldname = "World ?";
	}

    wchar_t wbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		wbuffer[i] = (unsigned short)worldname[i];
	}

	TextBox->SetString(wbuffer);
    return;
}



static int minusCount = 0;
void dCollectionCoin_c::executeState_KeyWait() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	// secret stage
	if (nowPressed & WPAD_Z) {
		minusCount++;
		if (minusCount >= 16) {
			minusCount = 0;

			MapSoundPlayer(SoundRelatedClass, SE_OBJ_GET_COIN_OLD, 1);
		}
	}

	// make the menu jump
	if (nowPressed & WPAD_TWO) {
		state.setState(&StateID_MenuJump);
	}

	// normal button controls
	if (nowPressed & WPAD_LEFT) {
		toPreviousPage();
	} else if (nowPressed & WPAD_RIGHT) {
		toNextPage();
	} else if (nowPressed & WPAD_ONE) {
		state.setState(&StateID_ExitAnimeEndWait);
	}
}

static int jumpCountdown, fallCountdown;
static bool hasPlayedSound;

void dCollectionCoin_c::beginState_MenuJump() {
	jumpCountdown = 15;
	fallCountdown = 15;
	hasPlayedSound = false;
	MapSoundPlayer(SoundRelatedClass, SE_PLY_JUMP, 1);
}
void dCollectionCoin_c::executeState_MenuJump() {
	OSReport("yeah");
	
	if (jumpCountdown != 0) {
		jumpCountdown--;
		rootPane->trans.y += 1.5f;
		return;
	}

	if (!hasPlayedSound) {
		MapSoundPlayer(SoundRelatedClass, SE_SYS_NICE_S, 1);
		hasPlayedSound = true;
	}

	if (fallCountdown != 0) {
		fallCountdown--;
		rootPane->trans.y -= 1.5f;
		return;
	}

	state.setState(&StateID_KeyWait);
}
void dCollectionCoin_c::endState_MenuJump() { }