#include <common.hpp>
#include <game.hpp>
#include "levelinfo.hpp"

// Text color values
GXColor fullTopColor = {255,197,0,255};
GXColor fullBottomColor = {252,144,0,255};
GXColor whiteColor = {255,255,255,255};

void dDateFile_c::newLoadInfoForSaveBlock(SaveBlock *block) {
    //init variables
    dScript::Res_c *msgRes = GetBMG();
    int world = 8;
    int livesCount;
    int worldNum = 9;

    //do worldnumber
    do {
        if (1 & block->worlds_available[world]) break;
        world--;
        worldNum--;
    } while (world > -1);
    WriteNumberToTextBox(&worldNum, T_worldNumber_01, 1);
    WriteBMGToTextBox(T_worldNumber_00, msgRes, 0x32, 0xF, 0);

    //lives number
    livesCount = block->player_lives[0];
    livesCount = livesCount & 0xFF;
    const int length = 2;
    WriteNumberToTextBox(&livesCount, &length, T_zanki_00, 1);
    WriteBMGToTextBox(T_x_00, msgRes, 0x32, 0x12, 0);

    // Color lives
    if (block->player_lives[0] == 99) {
        T_x_00->colour1 = fullTopColor;
        T_x_00->colour2 = fullBottomColor;
        T_zanki_00->colour1 = fullTopColor;
        T_zanki_00->colour2 = fullBottomColor;
    } else {
        T_x_00->colour1 = whiteColor;
        T_x_00->colour2 = whiteColor;
        T_zanki_00->colour1 = whiteColor;
        T_zanki_00->colour2 = whiteColor;
    }

    //handle stars
    P_Star_00->SetVisible(false);
    P_Star_01->SetVisible(false);
    P_Star_02->SetVisible(false);
    P_Star_03->SetVisible(false);
    P_Star_04->SetVisible(false);
    if (block->bitfield & SAVE_BIT_GAME_COMPLETE) P_Star_00->SetVisible(true);
    if (block->bitfield & SAVE_BIT_ALL_EXITS) P_Star_01->SetVisible(true);
    if (block->bitfield & SAVE_BIT_ALL_STAR_COINS) P_Star_02->SetVisible(true);
    if (block->bitfield & SAVE_BIT_ALL_STAR_COINS_W9) P_Star_03->SetVisible(true);
    if (block->bitfield & SAVE_BIT_EVERYTHING_TRULY_DONE) P_Star_04->SetVisible(true);
    
    // new functions
    setupClearedExitCount(block);
    setupWorldClearFlags(block);
    setIcons(block);

    return;
}

void dDateFile_c::setupClearedExitCount(SaveBlock *block) {
    int exitCount = 0;
    // Count all of the cleared exits
    for (int i = 0; i < dLevelInfo_c::s_info.sectionCount(); i++) {
		dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(i);

		for (int j = 0; j < section->levelCount; j++) {
			dLevelInfo_c::entry_s *l = &section->levels[j];
			if (l->flags & 2) {
				u32 cond = block->GetLevelCondition(l->worldSlot, l->levelSlot);
				if ((l->flags & 0x10) && (cond & COND_NORMAL))
					exitCount++;
				if ((l->flags & 0x20) && (cond & COND_SECRET))
					exitCount++;
			}
		}
	}
    OSReport("Completed exit count is: %i\n", exitCount);

    T_clear_00 = layout.findTextBoxByName("T_clear_00");
    const int length = 2;
    WriteNumberToTextBox(&exitCount, &length, T_clear_00, 0);

    // Color the text if it's the maximum value
    if (exitCount >= 63) {
        T_clear_00->colour1 = fullTopColor;
        T_clear_00->colour2 = fullBottomColor;
    } else {
        T_clear_00->colour1 = whiteColor;
        T_clear_00->colour2 = whiteColor;
    }
}

// TODO: Clean this up whenever i get around to it
void dDateFile_c::setupWorldClearFlags(SaveBlock *block) {
    P_flag_01 = layout.findPictureByName("P_flag_01");
    P_flag_02 = layout.findPictureByName("P_flag_02");
    P_flag_03 = layout.findPictureByName("P_flag_03");
    P_flag_04 = layout.findPictureByName("P_flag_04");
    P_flag_05 = layout.findPictureByName("P_flag_05");
    P_flag_06 = layout.findPictureByName("P_flag_06");
    P_flag_07 = layout.findPictureByName("P_flag_07");
    P_flag_08 = layout.findPictureByName("P_flag_08");
    P_flag_09 = layout.findPictureByName("P_flag_09");

    // Hide them all first
    P_flag_01->SetVisible(false);
    P_flag_02->SetVisible(false);
    P_flag_03->SetVisible(false);
    P_flag_04->SetVisible(false);
    P_flag_05->SetVisible(false);
    P_flag_06->SetVisible(false);
    P_flag_07->SetVisible(false);
    P_flag_08->SetVisible(false);
    P_flag_09->SetVisible(false);

    SaveBlock *save = GetSaveFile()->GetBlock(-1);
	//dLevelInfo_c *linfo = &dLevelInfo_c::s_info;

    // might work idk
    // instead of doing this batshit insane thing, just use the last level list in newer.cpp
    if ((save->GetLevelCondition(1, 24)) & COND_NORMAL)
        P_flag_01->SetVisible(true);
    if ((save->GetLevelCondition(2, 24)) & COND_NORMAL)
        P_flag_02->SetVisible(true);
    if ((save->GetLevelCondition(3, 24)) & COND_NORMAL)
        P_flag_03->SetVisible(true);
    if ((save->GetLevelCondition(4, 38)) & COND_NORMAL)
        P_flag_04->SetVisible(true);
    if ((save->GetLevelCondition(5, 24)) & COND_NORMAL)
        P_flag_05->SetVisible(true);
    if ((save->GetLevelCondition(6, 38)) & COND_NORMAL)
        P_flag_06->SetVisible(true);
    if ((save->GetLevelCondition(7, 24)) & COND_NORMAL)
        P_flag_07->SetVisible(true);
    if ((save->GetLevelCondition(8, 24)) & COND_NORMAL)
        P_flag_08->SetVisible(true);
    if ((save->GetLevelCondition(9, 8)) & COND_NORMAL)
        P_flag_09->SetVisible(true);
}

// Written by Ryguy, adapted to dateFile code (+ additons) by me
void dDateFile_c::setIcons(SaveBlock *block) {   
    static const char *picNames[] = {
        "P_charMario_00", "P_charLuigi_00",
        "P_charKinoY_00", "P_charKinoB_00",
        "P_flagMario_00", "P_flagLuigi_00",
        "P_flagKinoY_00", "P_flagKinoB_00",
    };
    layout.getPictures(picNames, &P_charMario_00, 8);

    // Hide them all
    P_charMario_00->SetVisible(false);
    P_flagMario_00->SetVisible(false);
    P_charLuigi_00->SetVisible(false);
    P_flagLuigi_00->SetVisible(false);
    P_charKinoY_00->SetVisible(false);
    P_flagKinoY_00->SetVisible(false);
    P_charKinoB_00->SetVisible(false);
    P_flagKinoB_00->SetVisible(false);

    //handle data
    switch (block->lastP1Character) {
        default: //mario
            P_charMario_00->SetVisible(true);
            P_flagMario_00->SetVisible(true);
            break;
        case 1: //luigi
            P_charLuigi_00->SetVisible(true);
            P_flagLuigi_00->SetVisible(true);
            break;
        case 2: //boad
            P_charKinoB_00->SetVisible(true);
            P_flagKinoB_00->SetVisible(true);
            break;
        case 3: //yoad
            P_charKinoY_00->SetVisible(true);
            P_flagKinoY_00->SetVisible(true);
            break;
    }
}