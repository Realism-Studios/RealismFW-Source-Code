#include <game.hpp>
#include <dInfo_c.hpp>
#include <sfx.hpp>

extern u8 CurrentWorldNumForWorldMap;

struct YesNoType {
	int bmgMessageID;
	bool yesButtonVisible;
	bool noButtonVisible;
	bool centerButtonVisible;
	int initialSelection; // 0 = no, 1 = yes, 2 = center
	int bmgYesID;
	int bmgNoID;
	int bmgCenterID;
};

const YesNoType yesNoTypeList[] = {
	{0x32, false, false, true,  2, 0x28, 0x29, 0x28}, // SAVE_DATA_CREATED
	{0x33, true,  true,  false, 0, 0x2A, 0x2B, 0x28}, // SAVE_PROMPT
	{0x34, false, false, true,  2, 0x28, 0x29, 0x28}, // SAVE_COMPLETE
	{0x35, true,  true,  false, 1, 0x28, 0x29, 0x28}, // EXIT_FREE_MODE_PROMPT
	{0x36, true,  true,  false, 1, 0x28, 0x29, 0x2C}, // PLAY_HINT_MOVIE_PROMPT
	{0x37, false, false, true,  2, 0x28, 0x29, 0x28}, // SKIP_COURSE
	{0x36, true,  true,  false, 1, 0x28, 0x29, 0x2C}, // PURCHASE_HINT_MOVIE_PROMPT
	{0x3A, false, false, true,  2, 0x28, 0x29, 0x28}, // GOT_STAR_COINS_IN_WORLD
	{0x3B, false, false, true,  2, 0x28, 0x29, 0x28}, // GOT_STAR_COINS_UNLOCK_W9_STAGE
	{0x3C, false, false, true,  2, 0x28, 0x29, 0x28}, // BOWSER_JR_LETTER
	{0x3D, true,  true,  false, 1, 0x28, 0x29, 0x28}, // RETURN_TO_TITLE_PROMPT
	{0x3E, false, false, true,  2, 0x28, 0x29, 0x28}, // BOWSER_JR_LETTER_MULTIPLAYER
	{0x3F, true,  true,  false, 1, 0x28, 0x29, 0x28}, // RETURN_TO_MAP_PROMPT
	{0x40, true,  true,  false, 1, 0x28, 0x29, 0x28}, // RETURN_MP_COURSE_SELECT_PROMPT
	{0x41, false, false, true,  2, 0x28, 0x29, 0x28}, // GOT_STAR_COINS_ALL_WORLDS
	{0x42, true,  true,  false, 0, 0x2D, 0x2B, 0x28}, // QUICK_SAVE_PROMPT
	{0x43, false, false, true,  2, 0x28, 0x29, 0x28}, // QUICK_SAVE_COMPLETE
	{0x30, false, false, true,  2, 0x28, 0x29, 0x28}, // ALL_TOAD_HOUSES_REAPPEARED
	{0x31, false, false, true,  2, 0x28, 0x29, 0x28}, // ITEMS_HINT
	{0x2E, true,  true,  false, 0, 0x26, 0x27, 0x28}, // TRY_WITHOUT_SUPER_GUIDE_PROMPT
	{0x37, true,  true,  false, 1, 0x24, 0x25, 0x28}, // SKIP_COURSE_PROMPT
	{0x44, false, false, true,  2, 0x28, 0x29, 0x28}, // EVERYTHING_CLEAR
	{0x2F, false, false, true,  2, 0x28, 0x29, 0x28}, // SAVE_AFTER_BOWSER_CLEAR
	{0x50, false, false, true,  2, 0x28, 0x29, 0x28}, // ACTIVATE_W3_SWITCH
	{0x51, false, false, true,  2, 0x28, 0x29, 0x28}, // DEACTIVATE_W3_SWITCH
	{0x39, true,  true,  false, 1, 0x28, 0x29, 0x28}, // EXIT_COIN_BATTLE_PROMPT
	{0x45, true,  true,  false, 1, 0x28, 0x29, 0x28}, // RETURN_TO_MAP_PROMPT_2
	{0x42, false, false, true,  2, 0x28, 0x29, 0x28}, // PEACH_CASTLE_MESSAGE
	{0x53, false, false, true,  2, 0x28, 0x29, 0x28}, // PEACH_CASTLE_MESSAGE_2
	// start of custom types
	{0x54, true,  true,  false, 0, 0x28, 0x29, 0}, // exit video menu prompt
};

void dYesNoWindow_c::newLoadThings() {
	int fieldLength;
	int messageID = yesNoTypeList[type].bmgMessageID;
	int yesButtonMessageID = yesNoTypeList[type].bmgYesID;
	int noButtonMessageID = yesNoTypeList[type].bmgNoID;
	int centerButtonMessageID = yesNoTypeList[type].bmgCenterID;

	nw4r::lyt::TextBox *T_center_00 = layout.findTextBoxByName("T_center_00");
	nw4r::lyt::TextBox *T_center_01 = layout.findTextBoxByName("T_center_01");
	
	// purchasing a Hint Movie
	if (type == PURCHASE_HINT_MOVIE_PROMPT) {
		fieldLength = 2;
		WriteNumberToTextBox(&starCoinRequiredCount, &fieldLength, T_needCoin_00, 1);
		WriteBMGToTextBox(T_otehonTextS_01, GetBMG(), 0x12E, 7, 0);
		WriteBMGToTextBox(T_otehonText_01, GetBMG(), 0x12E, 7, 0);
		WriteBMGToTextBox(T_needCoinX_00, GetBMG(), 0x12E, 8, 0);
		T_questionS_00->SetVisible(false);
		N_otehonText_00->SetVisible(true);
	} else {
		T_questionS_00->SetVisible(true);
		N_otehonText_00->SetVisible(false);
		
		if (type == GOT_STAR_COINS_ALL_WORLDS)
			dInfo_c::instance->layoutWorldNum = CurrentWorldNumForWorldMap + 1;
		
		// 8-Castle fix
		if (((type == SKIP_COURSE_PROMPT) && (m_startGameInfo.world1 = 7) && (m_startGameInfo.level1 = 23))) {
			messageID = 0x38;
		}
		WriteBMGToTextBox(T_questionS_00, GetBMG(), 0, messageID, 0);
		WriteBMGToTextBox(T_question_00, GetBMG(), 0, messageID, 0);
	}

	// set button visibility
	P_yesBase_00->SetVisible(yesNoTypeList[type].yesButtonVisible);
	P_noBase_00->SetVisible(yesNoTypeList[type].noButtonVisible);
	P_centerBase_00->SetVisible(yesNoTypeList[type].centerButtonVisible);

	current = yesNoTypeList[type].initialSelection;

	// 8-Castle fix
	if (type == SKIP_COURSE_PROMPT) {
		if ((m_startGameInfo.world1 = 7) && (m_startGameInfo.level1 = 23))
			yesButtonMessageID = 0x23;
	}

	// write button strings
	WriteBMGToTextBox(T_yes_00, GetBMG(), 0, yesButtonMessageID, 0);
	WriteBMGToTextBox(T_yes_01, GetBMG(), 0, yesButtonMessageID, 0);
	WriteBMGToTextBox(T_no_00, GetBMG(), 0, noButtonMessageID, 0);
	WriteBMGToTextBox(T_no_01, GetBMG(), 0, noButtonMessageID, 0);
	WriteBMGToTextBox(T_center_00, GetBMG(), 0, centerButtonMessageID, 0);
	WriteBMGToTextBox(T_center_01, GetBMG(), 0, centerButtonMessageID, 0);

	if (type == QUICK_SAVE_COMPLETE)
		N_saveIcon_00->SetVisible(true);
	else
		N_saveIcon_00->SetVisible(false);
}