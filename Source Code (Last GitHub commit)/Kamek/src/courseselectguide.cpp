#include <game.hpp>
#include <levelnumber.hpp>
#include "levelinfo.hpp"

// handles updating the level data displays
void dCourseSelectGuide_c::handleLevelData(LevelNumber param_2, int param_3) {
	if (currentLevelNum != param_2) {
		currentLevelGroup = GetLevelGroup(param_2);
		currentLevelNum = param_2;

		updateLevelDisplay(param_3);

		nw4r::lyt::Pane *N_completion_00 = layout.findPaneByName("N_completion_00");
		N_completion_00->SetVisible(true);

		dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(currentWorldNum, currentLevelNum);
		if (level) {
			if (param_3 != 1) {
				collectionCoinSet();
			}

			SaveBlock *save = GetSaveFile()->GetBlock(-1);
			u32 conds = save->GetLevelCondition(currentWorldNum, currentLevelNum);

			nw4r::lyt::Picture *P_flagSkull_01 = layout.findPictureByName("P_flagSkull_01");
			P_flagSkull_00->SetVisible(false);
			P_flagSkull_01->SetVisible(false);

			if (dInfo_c::instance->cyuukan.isCyuukanStart(0, currentWorldNum, currentLevelNum)) {
				P_flagSkull_00->SetVisible(true);
				P_flagSkull_00->alpha = 0x46;
			}

			if ((conds & COND_NORMAL) && (level->flags & 0x10))
				P_flagSkull_00->SetVisible(true);
			if ((conds & COND_SECRET) && (level->flags & 0x20))
				P_flagSkull_01->SetVisible(true);
		} else {
			N_completion_00->SetVisible(false);
		}
	}
}

void dCourseSelectGuide_c::updateLevelDisplay(u32 param) {
    if (param == 1) {
		currentLevelNum = 0xfffffffe;
	} else {
        nw4r::lyt::TextBox *T_levelName_00 = layout.findTextBoxByName("T_levelName_00");
		nw4r::lyt::Pane *N_zanki_00 = layout.findPaneByName("N_zanki_00");
		N_zanki_00->trans.y = 14.99f; // normal position

		T_cSelect_pic->SetVisible(false);
		T_cSelect_00->SetVisible(true);
        T_levelName_00->SetVisible(true);

		const wchar_t *convWorldName;
		const wchar_t *convLevelName;
        const char *levelname;

        dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(currentWorldNum, currentLevelNum);
		if (level) {
			convWorldName = getWorldNumber(level->displayWorld);
			convLevelName = getLevelNumber(currentWorldNum, level->displayLevel);

            levelname = dLevelInfo_c::s_info.getNameForLevel(level);
			WriteAsciiToTextBox(T_levelName_00, levelname);

			T_worldNum_00->SetString(convWorldName);

			if (level->displayLevel > 19) {
				T_cSelect_pic->SetVisible(true);
				T_cSelect_00->SetVisible(false);
				T_cSelect_pic->SetString(convLevelName);
			} else {
				T_cSelect_pic->SetVisible(false);
				T_cSelect_00->SetVisible(true);
				T_cSelect_00->SetString(convLevelName);
			}

            // airship anchor icon fix
            if (level->displayLevel == 38) {
                SaveBlock *save = GetSaveFile()->GetBlock(-1);
                u32 conds = save->GetLevelCondition(currentWorldNum, currentLevelNum);

                if (conds & COND_NORMAL)
                    T_cSelect_pic->SetString(L"?"); // anchor icon
            }
		} else {
			if (currentLevelNum > 254) { //get a dot
				T_cSelect_pic->SetVisible(true);
				T_cSelect_00->SetVisible(false);
				T_cSelect_pic->SetString(L"6");

                T_levelName_00->SetVisible(false);
                N_zanki_00->trans.y = 41.99f; // retail position

				dLevelInfo_c::entry_s *liWorld = dLevelInfo_c::s_info.searchBySlot(currentWorldNum, 38);
                if (liWorld) {
                    const wchar_t *worldNum;
                    worldNum = getWorldNumber(liWorld->displayWorld);
                    T_worldNum_00->SetString(worldNum);
                } else {
                    T_worldNum_00->SetString(L"?");
                }
			} else { // no fucking clue what this level is
				T_cSelect_pic->SetVisible(false);
				T_cSelect_00->SetVisible(true);
				T_cSelect_00->SetString(L"?");
				T_worldNum_00->SetString(L"?");

                char levelNumber[15];
				sprintf(levelNumber, "%d-%d (UNNAMED)", currentWorldNum+1, currentLevelNum+1);
				levelname = levelNumber;
				WriteAsciiToTextBox(T_levelName_00, levelname);
			}
		}
	}

	setupFooterInfo();
	setupHeaderInfo();
	//setupOtherStuff();

    return;
}

void dCourseSelectGuide_c::setupFooterInfo() {
    // insert world name
    nw4r::lyt::TextBox *T_worldName_00 = layout.findTextBoxByName("T_worldName_00");
    dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(currentWorldNum, currentLevelNum);
    dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(currentWorldNum + 1, 100);
    const char *worldname;

	if (world) {
		worldname = dLevelInfo_c::s_info.getNameForLevel(world);
	} else {
		char worldNumber[32];
		sprintf(worldNumber, "World %d (UNNAMED)", getWorldNumber(level->displayWorld));
		worldname = worldNumber;
	}

	wchar_t wbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		wbuffer[i] = (unsigned short)worldname[i];
	}

	T_worldName_00->SetString(wbuffer);


	// write star coin count
	nw4r::lyt::TextBox *T_worldCoin_00 = layout.findTextBoxByName("T_worldCoin_00");

	int length = 2;
    int starCoins = getStarCoinCountForWorld(currentWorldNum);
	WriteNumberToTextBox(&starCoins, &length, T_worldCoin_00, false);
}

void dCourseSelectGuide_c::setupHeaderInfo() {

}

//void dCourseSelectGuide_c::setupOtherStuff() {}