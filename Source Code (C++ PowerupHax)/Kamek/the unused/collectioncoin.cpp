#include <newer.hpp>
#include <sfx.hpp>
#include "levelinfo.hpp"
#include <levelnumber.hpp>
#include <stage.hpp>

void SCMWorldName(int param_1) {
    nw4r::lyt::TextBox *TextBox1 = *(nw4r::lyt::TextBox **)(param_1 + 0x474);
    nw4r::lyt::TextBox *TextBox2 = *(nw4r::lyt::TextBox **)(param_1 + 0x478);

    const char *worldname;
    int CurrentWorld = *(int *)(param_1 + 0xb44);

    dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(CurrentWorld+1, 100);
    if (world) {
		worldname = dLevelInfo_c::s_info.getNameForLevel(world);
	} else {
		worldname = "World ?";
	}

    wchar_t wbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		wbuffer[i] = (unsigned short)worldname[i];
	}

	TextBox1->SetString(wbuffer);
    TextBox2->SetString(wbuffer);
    return;
}

void SCMWorldName2(int param_1, int param_2) {
    nw4r::lyt::TextBox *TextBox = *(nw4r::lyt::TextBox **)(param_1 + param_2 * 4 + 0x474);

    const char *worldname;
    int CurrentWorld = *(int *)(param_1 + 0xb44);

    dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(CurrentWorld+1, 100);
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

extern "C" bool IsCourseClear(int World, int Level);

void handleFileters(int worldNumber, int levelNumber, m2d::EmbedLayout_c *layout) {
	nw4r::lyt::Picture *P_coinFileter_00 = layout->findPictureByName("P_coinFileter_00");
	nw4r::lyt::Picture *P_coinFileter_01 = layout->findPictureByName("P_coinFileter_01");
	nw4r::lyt::Picture *P_coinFileter_02 = layout->findPictureByName("P_coinFileter_02");

	if (IsCourseClear(worldNumber, levelNumber)) {
		P_coinFileter_00->SetVisible(false);
		P_coinFileter_01->SetVisible(false);
		P_coinFileter_02->SetVisible(false);
	} else {
		P_coinFileter_00->SetVisible(true);
		P_coinFileter_01->SetVisible(true);
		P_coinFileter_02->SetVisible(true);
	}
}

void SCMWorldAndLevelNumbers(nw4r::lyt::TextBox *T_worldNum_00, nw4r::lyt::TextBox *T_corseNum_00, nw4r::lyt::TextBox *T_pictureFont_00, nw4r::lyt::Pane *somePane, int worldNumber, int levelNumber) {
	if (levelNumber == Stage_Invalid)
		somePane->SetVisible(false);
	else
		somePane->SetVisible(true);

    dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldNumber, levelNumber);
    if (level) {
        const wchar_t *convWorldName = getWorldNumber(level->displayWorld);
        const wchar_t *convLevelName = getLevelNumber(worldNumber, level->displayLevel);

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
	return;
}


// adds support for button combinations
extern "C" void CollectionCoinNextWorld(int dCoinCollection);
extern "C" void CollectionCoinPreviousWorld(int dCoinCollection);

static const int secretCode[] = {
	WPAD_UP,WPAD_UP,WPAD_DOWN,WPAD_DOWN,
	WPAD_LEFT,WPAD_RIGHT,WPAD_LEFT,WPAD_RIGHT,
	WPAD_ONE,WPAD_TWO,0
};
static const int secretCodeButtons = WPAD_UP|WPAD_DOWN|WPAD_LEFT|WPAD_RIGHT|WPAD_ONE|WPAD_TWO;
static int secretCodeIndex = 0;
static int minusCount = 0;
extern bool enableHardMode;
extern bool enableDebugMode;
extern u8 isReplayEnabled;

void addReplayAndHardMode(dCollectionCoin_c* self) {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	// hard mode
	if ((GetActiveRemocon()->heldButtons == 0x810) && (nowPressed & 0x810)) {
		if (!enableHardMode) {
			enableHardMode = true;
			OSReport("Hard Mode enabled!\n");
			MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_CS_COURSE_IN_HARD, 1);
		} else {
			enableHardMode = false;
			OSReport("Hard Mode disabled!\n");
		}
		return;
	}

	// replay
	if (nowPressed & secretCodeButtons) {
		int nextKey = secretCode[secretCodeIndex];
		if (nowPressed & nextKey) {
			secretCodeIndex++;
			if (secretCode[secretCodeIndex] == 0) {
				secretCodeIndex = 0;
				if (isReplayEnabled != 100) {
					isReplayEnabled = 100;
					MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_CS_COURSE_IN, 1);
					OSReport("Replay Recording enabled!\n");
				} else {
					isReplayEnabled = 0;
					OSReport("Replay Recording disabled!\n");
				}
			}
			return;
		} else {
			secretCodeIndex = 0;
		}
	}

	// debug collision viewer
	if (nowPressed & WPAD_MINUS) {
		minusCount++;
		if (minusCount >= 16) {
			minusCount = 0;

			enableDebugMode = !enableDebugMode;

			if (enableDebugMode) {
				MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_GET_PRIZE, 1);
			}
		}
	}
	
	// normal button controls
	if (nowPressed & WPAD_RIGHT) {
		CollectionCoinNextWorld((int)self);
	} else if (nowPressed & WPAD_LEFT) {
		CollectionCoinPreviousWorld((int)self);
	} else if (nowPressed & WPAD_ONE) {
		self->state.setState(&dCollectionCoin_c::StateID_ExitAnimeEndWait);
	}
}