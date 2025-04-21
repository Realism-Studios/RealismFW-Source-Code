#include <game.hpp>
#include "levelinfo.hpp"
#include <profile.hpp>
#include "fileload.hpp"
#include "pregame.hpp"
#include <sfx.hpp>

class PregameLytHandler {
	public:
		m2d::EmbedLayout_c layout;

		nw4r::lyt::Pane *rootPane;

		nw4r::lyt::TextBox
			*T_minus_00, *T_world_00, *T_worldNum_00,
			*T_pictureFont_00, *T_corseNum_00,
			*T_remainder_00, *T_remainder_01, *T_remainder_02, *T_remainder_03,
			*T_remainder_10, *T_remainder_11, *T_remainder_12, *T_remainder_13,
			*T_x_XX[10],
			*T_otasukePlay_00, *T_otasukePlay_01,
			*T_recommend_00, *T_remainder_00_o, *T_remainder_10_o;

		nw4r::lyt::Picture
			*P_Wx_00[9], *P_coin_00, *P_free_00,
			*P_batB_0x[4], *P_bat_00,
			*P_batB_1x[4], *P_bat_01,
			*P_batB_2x[4], *P_bat_02,
			*P_batB_3x[4], *P_bat_03,
			*P_luijiIcon_00_o, *P_luijiIcon_10_o, *P_coinStage_00;

		nw4r::lyt::Pane
			*N_mario_00, *N_luiji_00, *N_kinoB_01, *N_kinoY_00,
			*N_zankiPos_x[4], *N_zanki_00,
			*Null_battPosxP[4], *N_batt_x[4],
			*N_batt, *N_otasukePlay_00;

		u8 layoutLoaded, somethingHasBeenDone, isVisible, hasShownLuigiThing_orSomething;

		u32 currentStateID;

		u32 _2E8;

		u32 countdownToEndabilityCopy, activePlayerCountMultBy4_maybe;
		u32 batteryLevels[4];
		u32 pgCountdown; 

		void setup(); //80b6c040
		void loadLives(); //80b6bcc0
		void loadLevelNumber(); //80B6BDD0

		//replaced funcs
		int newSetup(); //replaces 80b6c040
		void newLoadLevelNumber(); // replaces 80B6BDD0
		void loadLevelNumberW5(); // special case for W5

		//all new funcs
		void loadLevelName(); 
		void insertSampleImage();
		void fixSuperGuidePlayerIcon();
};

extern "C" void *CurrentWarningManager;
extern "C" void WarningMgr_AllWarningEnd(void *param_1, bool param_2);
extern char CurrentWorld;
extern char CurrentLevel;


int PregameLytHandler::newSetup() {
	dInfo_c *info = dInfo_c::instance;
	bool state;
	dScript::Res_c *msgRes = GetBMG();
	u32 dVar7;

	if (!layoutLoaded) {
		// get our layout archive name
		const char *filename;

		// assume we're in story mode, other modes don't have unique preGames
		char buffer[32];
		sprintf(buffer, "preGameW%X.arc", m_startGameInfo.world1+1);
		filename = buffer;

		// level types
		switch (m_startGameInfo.level1) {
			case 19: filename = "preGameCoin.arc"; break; // Coin Battle exclusive stages
			case 20: filename = "preGameGhost.arc"; break;
			case 22: case 21: filename = "preGameTower.arc"; break;
			case 24: case 23: filename = "preGameCastle.arc"; break;
			case 37: filename = "preGameAirship.arc"; break;
			case 40: filename = "preGamePeach.arc"; break;
		};

		// Toad Houses, Ambushes, and Cannons
		if ((m_startGameInfo.level1 >= 25) && (m_startGameInfo.level1 <= 35))
			filename = "preGameOther.arc";

		// special cases
		if (((m_startGameInfo.world1 == 3) && (m_startGameInfo.level1 == 5 || m_startGameInfo.level1 == 6)) || ((m_startGameInfo.world1 == 0) && (m_startGameInfo.level1 == 8)))
			filename = "preGameEVIL.arc";

		// now finally load our layout
		bool gotFile = layout.loadArc(filename, false);
		if (!gotFile) {
			bool gotFallback = layout.loadArc("preGameError.arc", false);
			if (!gotFallback)
				return false;
		}

		// load brlyt
		bool output = layout.build("preGame_19.brlyt");

		//handle panes & anims
		layout.loadAnimations(preGameBrlanNames, 8);
		layout.loadGroups(preGameGroupNames, mappings, 0xF);

		rootPane = layout.getRootPane();

		layout.setLangStrings(preGameLangBoxes, langIDs, 200, 6);
		layout.getTextBoxes(preGameTextBoxes, &T_minus_00, 0x1C);
		
		//broken, writes "x" to all the "T_x_XX" panes
		for (int i = 0; i < 11; i++) {
			WriteBMGToTextBox(T_x_XX[i], msgRes, 0x66, 0x2a, 0);
		}

		layout.getPictures(preGamePictures, &P_Wx_00[0], 0x22);
		layout.getPanes(preGamePanes, &N_mario_00, 0x13);

		//load lives
		loadLives();

		//handle pane visibility
		this->rootPane->SetVisible(true);

		nw4r::lyt::Picture *P_levelSample_00 = layout.findPictureByName("P_levelSample_00");
		nw4r::lyt::Picture *Spotlight = layout.findPictureByName("Spotlight");
		//super guide
		if (m_startGameInfo.screenType == 1) {
			N_otasukePlay_00->SetVisible(true);
			P_levelSample_00->SetVisible(false);
			Spotlight->SetVisible(false);

			layout.enableNonLoopAnim(0, true);

			fixSuperGuidePlayerIcon();
		} else {
			N_otasukePlay_00->SetVisible(false);
			P_levelSample_00->SetVisible(true);
		}

		//handle some byte thing
		dVar7 = info->_3E8;
		if (dVar7 > 9) {
       		dVar7 = dVar7 - 10;
     	}

		//handle level bar based on game flags
		T_minus_00->SetVisible(true);
		T_world_00->SetVisible(true);
		T_worldNum_00->SetVisible(true);
		T_pictureFont_00->SetVisible(true);
		T_corseNum_00->SetVisible(true);

		if ((((GameFlag & CoinBattle) == 0) || (info->_3E4 != 0)) || ((int)dVar7 > 4)) {
			newLoadLevelNumber(); //originally loadLevelNumber();
		} else {
			T_worldNum_00->SetVisible(false);
			T_pictureFont_00->SetVisible(false);
			WriteBMGToTextBox(T_world_00, msgRes, 0x66, 0x1f, 0);
			info->layoutWorldNum = dVar7 + 1;
			WriteBMGToTextBox(T_corseNum_00, msgRes, 0x66, 0x21, 0);
			if ((GameFlag & GameOver) == 0) {
				P_coin_00->SetVisible(true);
			} else {
				P_free_00->SetVisible(true);
			}
		}

		loadLevelName(); //NEW

		layoutLoaded = 1;
		isVisible = 1;
		currentStateID = 0;

		if (!DontShowPreGame) {
			isVisible = 1;
		} else {
			WarningMgr_AllWarningEnd(CurrentWarningManager, true);
			isVisible = 0;
		} 

		hasShownLuigiThing_orSomething = 0;
		layout.disableAllAnimations();
		layout.enableNonLoopAnim(0xE);
		pgCountdown = 0x17C;
		state = true;
	} else {
		state = true;
	}
	return state;
}



// make it show the current P1 before switching to Super Guide Luigi
void PregameLytHandler::fixSuperGuidePlayerIcon() {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	char currentPlayer = save->lastP1Character;

	// fuckers, these are the shadows but we don't need the real ones
	nw4r::lyt::Picture *P_marioFucker_10 = layout.findPictureByName("P_marioFucker_10");
	nw4r::lyt::Picture *P_luigiFucker_10 = layout.findPictureByName("P_luigiFucker_10");
	nw4r::lyt::Picture *P_yoadFucker_10 = layout.findPictureByName("P_yoadFucker_10");
	nw4r::lyt::Picture *P_boadFucker_10 = layout.findPictureByName("P_boadFucker_10");

	nw4r::lyt::TextBox *T_remainder_10_o = layout.findTextBoxByName("T_remainder_10_o");
	nw4r::lyt::TextBox *T_remainder_00_o = layout.findTextBoxByName("T_remainder_00_o");

	// hide them all
	P_marioFucker_10->SetVisible(false);
	P_luigiFucker_10->SetVisible(false);
	P_yoadFucker_10->SetVisible(false);
	P_boadFucker_10->SetVisible(false);

	switch (currentPlayer) {
        default: //mario
            P_marioFucker_10->SetVisible(true);
            break;
        case 1: //luigi
            P_luigiFucker_10->SetVisible(true);
            break;
        case 2: //blue toad
            P_boadFucker_10->SetVisible(true);
            break;
        case 3: //yellow toad
            P_yoadFucker_10->SetVisible(true);
            break;
    }

	int lives = Player_Lives[currentPlayer];
	int length = 2;
    WriteNumberToTextBox(&lives, &length, T_remainder_10_o, 1);
	WriteNumberToTextBox(&lives, &length, T_remainder_00_o, 1);
}


void PregameLytHandler::insertSampleImage() {
	// sample image
	nw4r::lyt::Picture *P_levelSample_00 = layout.findPictureByName("P_levelSample_00");

	char tplName[64];
	sprintf(tplName, "/LevelSamples/%02d-%02d.tpl", m_startGameInfo.world1+1, m_startGameInfo.level1+1);
	static File tpl;
	if (tpl.open(tplName))
		P_levelSample_00->material->texMaps[0].ReplaceImage((TPLPalette*)tpl.ptr(), 0);
}

// handles level number
void PregameLytHandler::newLoadLevelNumber() {
	const wchar_t *convWorldName;
	const wchar_t *convLevelName;

	// get panes
	nw4r::lyt::TextBox *T_minus_01, *T_world_01, *T_worldNum_01, *T_pictureFont_01, *T_corseNum_01;
	T_minus_01 = layout.findTextBoxByName("T_-_01");
	T_world_01 = layout.findTextBoxByName("T_world_01");
	T_worldNum_01 = layout.findTextBoxByName("T_worldNum_01");
	T_pictureFont_01 = layout.findTextBoxByName("T_pictureFont_01");
	T_corseNum_01 = layout.findTextBoxByName("T_corseNum_01");

	//set BMG strings
	if (m_startGameInfo.world1 != 4) {
		dScript::Res_c *msgRes = GetBMG();
		WriteBMGToTextBox(T_minus_00, msgRes, 0x66, 0x29, 0);
		WriteBMGToTextBox(T_minus_01, msgRes, 0x66, 0x29, 0);
	}

	// note: setting the visibility of the shadow (01) panes will show/hide the non-shadow panes (00) too

	//set world and level icons
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(m_startGameInfo.world1, m_startGameInfo.level1);
	if (level) {
		convWorldName = getWorldNumber(level->displayWorld);
		convLevelName = getLevelNumber((int)m_startGameInfo.world1, level->displayLevel);

		T_worldNum_01->SetString(convWorldName);
		T_worldNum_00->SetString(convWorldName);

		if (level->displayLevel > 19) {
			T_pictureFont_01->SetVisible(true);			
			T_corseNum_01->SetVisible(false);

			T_pictureFont_00->SetString(convLevelName);
			T_pictureFont_01->SetString(convLevelName);
		} else {
			T_pictureFont_01->SetVisible(false);
			T_corseNum_01->SetVisible(true);

			T_corseNum_00->SetString(convLevelName);
			T_corseNum_01->SetString(convLevelName);
		}
	} else {
		T_pictureFont_01->SetVisible(false);
		T_corseNum_01->SetVisible(true);

		T_worldNum_00->SetString(L"?");
		T_worldNum_01->SetString(L"?");
		T_corseNum_00->SetString(L"?");
		T_corseNum_01->SetString(L"?");
	}

	// only insert the world name if it's not W5
	if (m_startGameInfo.world1 != 4) {
		dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(m_startGameInfo.world1+1, 100);
		const char *worldname;

		if (world) {
			worldname = dLevelInfo_c::s_info.getNameForLevel(world);
		} else {
			char worldNumber[32];
			sprintf(worldNumber, "World %d (UNNAMED)", convWorldName);
			worldname = worldNumber;
		}

		wchar_t wbuffer[0x40];
		for (int i = 0; i < 0x40; i++) {
			wbuffer[i] = (unsigned short)worldname[i];
		}

		T_world_00->SetString(wbuffer);
		T_world_01->SetString(wbuffer);
	}

	insertSampleImage();
}


void PregameLytHandler::loadLevelName() {
	const char *levelname;

	nw4r::lyt::TextBox *T_levelNameS_00 = layout.findTextBoxByName("T_levelNameS_00");
	nw4r::lyt::TextBox *T_levelName_00 = layout.findTextBoxByName("T_levelName_00");

	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(m_startGameInfo.world1, m_startGameInfo.level1);
	if (level) {
		if (m_startGameInfo.level1 == 38) {
			SaveFile *file = GetSaveFile();
			SaveBlock *block = file->GetBlock(file->header.current_file);
			switch(block->toad_level_idx[m_startGameInfo.world1]) {
				case 4: //yellow
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(m_startGameInfo.world1, 27));
					break;
				case 5: //red
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(m_startGameInfo.world1, 26));
					break;
				default: //green
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(m_startGameInfo.world1, 25));
					break;
			}
		} else {
			levelname = dLevelInfo_c::s_info.getNameForLevel(level);
		}
	} else {
		char levelNumber[15];
		sprintf(levelNumber, "%d-%d (UNNAMED)", m_startGameInfo.world1+1, m_startGameInfo.level1+1);
		levelname = levelNumber;
	}

	wchar_t lbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		lbuffer[i] = (unsigned short)levelname[i];
	}

	T_levelNameS_00->SetString(lbuffer);
	T_levelName_00->SetString(lbuffer);
}



// pregame SCENE stuff
class dScCrsin_c : public dScene_c {
	public:
		dStateWrapper_c<dScCrsin_c> state;
		PregameLytHandler preGame;
		u32 countdownToEndability;
		u8 chain[8]; // sPhase_c
		bool hasReplayFlags;
		u8 unk[3];

		void adjustPreGameDuration();
};

void dScCrsin_c::adjustPreGameDuration() {
	countdownToEndability = 120; // default

	// newer world
	if (m_startGameInfo.world1 == 4) {
		countdownToEndability = 180;
	}
}