#include <dInfo_c.hpp>
#include <game.hpp>
#include <newer.hpp>
#include <levelinfo.hpp>
#include <sfx.hpp>
#include <stage.hpp>

extern char CurrentWorld;
extern char CurrentLevel;

class Pausewindow_c : public dBase_c {
	public:
		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		Pausewindow_c();
		~Pausewindow_c();

		m2d::EmbedLayout_c layout;
		dStateWrapper_c<Pausewindow_c> state;

		nw4r::lyt::Picture
			*P_SBBase_00, *P_SBBase_02, *P_shadowBlack, *P_coin;

		nw4r::lyt::Pane *W_N_pauseMenu_00; 
		
		nw4r::lyt::TextBox 
			*T_tuzukeru_00, *T_tuzukeru_01, *T_modoru_00, *T_modoru_01,
			*T_worldNum_00, *T_corseNum_00, *T_corsePic_00, *T_osusumeText_00;
		
		nw4r::lyt::Pane
			*N_worldText_00, *N_osusumeText_00, *N_multiText_00;
		
		u32 nextButton, *currentButton, windowColourID;
		u8 layoutLoaded;
		bool isVisible;
		u8 _292, _293, animationIsPlaying, needsToSetButtonStrings;
		u8 unk[2];

		// new crap
		int pauseCounter;

		//new funcs
		void newLoadLevelName();
		void insertProTips();
		void setupCounters();
		void setupRandomCrap();

		//actually replaces parts of states but shhh
		void onWindowCreation();
		void onWindowOpen();
		void onWindowClose();

		static Pausewindow_c *instance; // 0x8042A61C
};

void Pausewindow_c::newLoadLevelName() {
	const wchar_t *convWorldName;
	const wchar_t *convLevelName;
	const char *levelname;

	dInfo_c *info = dInfo_c::instance;
	u32 unk;

	unk = info->_3E8;

	if ((int)unk > 9) {
    	unk = unk - 10;
	}

	int wnum = (int)CurrentWorld;
	int lnum = (int)CurrentLevel;

	nw4r::lyt::TextBox *T_levelName_00;
	T_levelName_00 = layout.findTextBoxByName("T_levelName_00");

	if ((((GameFlag & 0x40) == 0) || (info->_3E4 != 0)) || ((int)unk > 4)) {
		//P_coin_00->SetVisible(false);
		T_worldNum_00->SetVisible(true);
	} else {
		//P_coin_00->SetVisible(true);
		T_worldNum_00->SetVisible(false);
	}
	
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(CurrentWorld, CurrentLevel);
	if (level) {
		convWorldName = getWorldNumber(level->displayWorld);
		convLevelName = getLevelNumber((int)CurrentWorld, level->displayLevel);

		T_worldNum_00->SetString(convWorldName);

		if (level->displayLevel > 19) {
			T_corsePic_00->SetVisible(true);
			T_corseNum_00->SetVisible(false);
			T_corsePic_00->SetString(convLevelName);
		} else {
			T_corsePic_00->SetVisible(false);
			T_corseNum_00->SetVisible(true);
			T_corseNum_00->SetString(convLevelName);
		}

		if (CurrentLevel == 38) {
			SaveFile *file = GetSaveFile();
			SaveBlock *block = file->GetBlock(file->header.current_file);
			switch (block->toad_level_idx[CurrentWorld]) {
				case 4: //yellow
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(CurrentWorld, 27));
					break;
				case 5: //red
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(CurrentWorld, 26));
					break;
				default: //green
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(CurrentWorld, 25));
					break;
			}
		} else {
			levelname = dLevelInfo_c::s_info.getNameForLevel(level);
		}
	} else {
		T_corsePic_00->SetVisible(false);
		T_corseNum_00->SetVisible(true);
		T_worldNum_00->SetString(L"?");
		T_corseNum_00->SetString(L"?");
		char levelNumber[15];
		sprintf(levelNumber, "%d-%d (UNNAMED)", wnum+1, lnum+1);
		levelname = levelNumber;
	}

	wchar_t lbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		lbuffer[i] = (unsigned short)levelname[i];
	}

	T_levelName_00->SetString(lbuffer);
}


void Pausewindow_c::setupCounters() {
	// player lives
	static const int LogicalPlayerIDs[] = {0,1,3,2};
	nw4r::lyt::TextBox *T_life_0[4];

	for (int i = 0; i < 4; i++) {
		int playerID = LogicalPlayerIDs[i];
		int slotID = SearchForIndexOfPlayerID(playerID);
		int lives = Player_Lives[slotID];
		int length = 2;

		// get textboxes
		static const char *names[] = {"T_life_00", "T_life_01", "T_life_02", "T_life_03"};
		layout.getTextBoxes(names, &T_life_0[0], 4);

		WriteNumberToTextBox(&lives, &length, T_life_0[i], true);
	}

	// star coin counts
	int coins = getStarCoinCount();
	int spent = getStarCoinCount() - getUnspentStarCoinCount();

	nw4r::lyt::TextBox *T_counter_00, *T_counter_01;
	T_counter_00 = layout.findTextBoxByName("T_counter_00");
	T_counter_01 = layout.findTextBoxByName("T_counter_01");

	int length = 8;
	WriteNumberToTextBox(&coins, &length, T_counter_00, false);
	WriteNumberToTextBox(&spent, &length, T_counter_01, false);


	// update the star coin display
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(CurrentWorld, CurrentLevel);

	if (level) {
		if (level->flags & 2)
			layout.findPaneByName("N_collection_00")->SetVisible(true);
		else {
			layout.findPaneByName("N_collection_00")->SetVisible(false);
			return;
		}

		// update coin display
		dInfo_c *info = dInfo_c::instance;
		nw4r::lyt::Pane *P_coinOff_0[3];
		nw4r::lyt::Pane *P_coinOn_0[3];

		for (int coinIdx = 0; coinIdx < 3; coinIdx++) {
			// get panes
			char onName[16];
			sprintf(onName, "P_coinOn_0%i", coinIdx);
			P_coinOn_0[coinIdx] = layout.findPictureByName(onName);

			char offName[16];
			sprintf(offName, "P_coinOff_0%i", coinIdx);
			P_coinOff_0[coinIdx] = layout.findPictureByName(offName);

			// is the coin collected? if not, do it
			u32 isCollect = save->CheckIfCoinCollected(CurrentWorld, CurrentLevel, coinIdx);
			if ((isCollect & 0xFF) == 0) {
				// make sure the coin is not uncollected
				if ((info->cyuukan.star_coin_status[coinIdx] != 4) || (StarCoinStatusArray[coinIdx] != 4)) {
					// we have the coin, show it
					P_coinOff_0[coinIdx]->alpha = 0;
					P_coinOn_0[coinIdx]->SetVisible(true);
				} else {
					// we don't have the coin, hide it
					P_coinOff_0[coinIdx]->alpha = 255;
					P_coinOn_0[coinIdx]->SetVisible(false);
				}
			} else {
				// coin is already saved to file, show it
				P_coinOn_0[coinIdx]->SetVisible(true);
			}
		}
	}
}

// Stuff that isn't significant enough to have it's own function
void Pausewindow_c::setupRandomCrap() {
	// Random number generator
	nw4r::lyt::TextBox *T_result_00;
	T_result_00 = layout.findTextBoxByName("T_result_00");

	int rand = GenerateRandomNumber(6);
	WriteNumberToTextBox(&rand, T_result_00, true);

	// the layout's fucked for some reason, so i can't fix this the normal way
	T_result_00->trans.z = 0.00f;

	// Warning Manager
	nw4r::lyt::TextBox *T_warning_00;
	T_warning_00 = layout.findTextBoxByName("T_warning_00");
	T_warning_00->SetVisible(false);

	if (pauseCounter >= 5) {
		T_warning_00->SetVisible(true);
		T_warning_00->SetString(L"WARNING: Why do you keep pausing so much? Play the game.");
		if (pauseCounter == 5) {
			MapSoundPlayer(SoundRelatedClass, SE_SYS_KO_DIALOGUE_IN, 1);
		}
	}

	if (pauseCounter >= 20) {
		MapSoundPlayer(SoundRelatedClass, SE_SYS_CHANGE_RANK, 1);
		T_warning_00->SetString(L"WARNING: If you pause 25 times, you'll regret it. You'll be FROZEN with... regret of pausing so much?");
	}
	
	if (pauseCounter >= 25) {
		while (true) { }
	}


	// control-scheme button, this just prints the relevant controller icon to a textbox
	nw4r::lyt::TextBox *T_control_00 = layout.findTextBoxByName("T_control_00");
	int cntType = RemoconMng->controllers[windowColourID]->controllerType;

	if (cntType == 0) { // wiimote
		T_control_00->SetString(L"\x0B\x012C\xBEEF");
	} else { // nunchuk
		T_control_00->SetString(L"\x0B\x012D\xBEEF");
	}


	// really rare easter egg
	int result = GenerateRandomNumber(64018);

	nw4r::lyt::Pane *N_error_00;
	nw4r::lyt::TextBox *T_errorDesc_00;

	N_error_00 = layout.findPaneByName("N_error_00");
	T_errorDesc_00 = layout.findTextBoxByName("T_errorDesc_00");
    
    // Alpha is set to 0 by default so it doesn't cover up the rest of the layout in a gui editor
	if (result == 907) {
		N_error_00->SetVisible(true);
		N_error_00->alpha = 255;
	} else {
		N_error_00->SetVisible(false);
		N_error_00->alpha = 0;
	}
}


void Pausewindow_c::insertProTips() {
    int tipChooser = GenerateRandomNumber(64);
    nw4r::lyt::TextBox *T_tipText_00, *T_tipText_01;

    T_tipText_00 = layout.findTextBoxByName("T_tipText_00");
    T_tipText_01 = layout.findTextBoxByName("T_tipText_01");

	// Never show message 9, i wish i could do it the same way minecraft does it
	if (tipChooser == 9) {
		tipChooser++;
    }

	if (tipChooser == 29) {
		MapSoundPlayer(SoundRelatedClass, SE_VOC_YS_YOSHI, 1);
	}

	// Show/hide upside down textbox
	if (tipChooser == 10) {
		T_tipText_00->SetVisible(false);
		T_tipText_01->SetVisible(true);
	} else {
		T_tipText_00->SetVisible(true);
		T_tipText_01->SetVisible(false);
	}

	int msg = tipChooser;
	WriteBMGToTextBox(T_tipText_00, GetBMG(), 0x20000, msg, 0);
	WriteBMGToTextBox(T_tipText_01, GetBMG(), 0x20000, msg, 0);
}

// todo: if there's ever another menu that needs music, maybe move the these two
// functions to another file and have them be for general-purpose menu music playing

// Used by below
static nw4r::snd::StrmSoundHandle menuHandle;
static bool isPauseMusicPlaying;

void playBGM() {
    if (isPauseMusicPlaying) // Just in case
        return;
    
    int trackIndex = GenerateRandomNumber(9);
    
    static const int trackIDs[9] = {
        STRM_BGM_SELECT_W1,
        STRM_BGM_SELECT_W2,
        STRM_BGM_SELECT_W3,
        STRM_BGM_SELECT_W4,
        STRM_BGM_SELECT_W5,
        STRM_BGM_SELECT_W6,
        STRM_BGM_SELECT_W7,
        STRM_BGM_SELECT_W8,
        STRM_BGM_SELECT_W9
    };
    
    PlaySoundWithFunctionB4(SoundRelatedClass, &menuHandle, trackIDs[trackIndex], 0);
    
    isPauseMusicPlaying = true;
}


void stopBGM() {
	if (!isPauseMusicPlaying)
		return;

	if (menuHandle.Exists())
		menuHandle.Stop(0);

	isPauseMusicPlaying = false;
}


// BEGIN FUNCTIONS THAT TRIGGER THE ABOVE CODE

// runs when the menu is created
void Pausewindow_c::onWindowCreation() {
	pauseCounter = 0; // Reset it
	newLoadLevelName();
}

// runs every time the menu is opened
void Pausewindow_c::onWindowOpen() {
	// fix animations
	layout.resetAnim(3, false); // button 0
	layout.resetAnim(4, false); // button 2
	layout.enableNonLoopAnim(0); // inWindow
	
	// new stuff
	pauseCounter++;
	playBGM();
	insertProTips();
	setupCounters();

	setupRandomCrap();
}

// runs every time the menu is closed
void Pausewindow_c::onWindowClose() {
	stopBGM();
}


// since the pause counter increments when the window is opened, this causes an issue when
// opening/closing the window for the exit course prompt, since it closes and reopens the pause menu.
// so if we deincrement the counter when the menu opens, the window incrementing it will return it to the former value
void fixPauseCounterBug() {
	Pausewindow_c::instance->pauseCounter--;
}