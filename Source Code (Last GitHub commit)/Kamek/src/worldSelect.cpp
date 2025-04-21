#include <game.hpp>
#include <newer.hpp>
#include "levelinfo.hpp"

void daWMIsland_c::fuckUpIslands() {
	FUN_808D3F30();

	if (difficultySetting < DIFFICULTY_HARD)
		return;

	float rotX = RandFloat(-94.0f, 100.0f);
	float rotY = RandFloat(-80.0f, 232.0f);
	float rotZ = RandFloat(-260.0f, 492.53f);

	rot.x += rotX;
	rot.y += rotY;
	rot.z += rotZ;

	float posX = RandFloat(-323.0f, 522.0f);
	float posY = RandFloat(-864.0f, 967.3f);
	float posZ = RandFloat(-100.0f, 463.53f);

	pos.x += posX;
	pos.y += posY;
	pos.z += posZ;

	int posFix = GenerateRandomNumber(24);
	if (posFix == 5) {
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;
	}

	float scaleX = RandFloat(-1.5f, 1.2f);
	float scaleY = RandFloat(-1.25f, 1.35f);
	float scaleZ = RandFloat(-1.05f, 1.5f);

	scale.x += scaleX;
	scale.y += scaleY;
	scale.z += scaleZ;

	int scaleFix = GenerateRandomNumber(96);
	if (scaleFix == 52) {
		scale.x = 1.0f;
		scale.y = 1.0f;
		scale.z = 1.0f;
	}
}


// this function is admittedly pretty messy. if i end up adding more to the layout,
// perhaps i should rewrite the onCreate/loadLayout and clean this up? todo maybe???
void dWorldSelectGuide_c::updateWorldInfo() {
	nw4r::lyt::TextBox *T_goto_01 = layout.findTextBoxByName("T_goto_01");
	WriteBMGToTextBox(T_goto_01, GetBMG(), 3, 0x0D, 0);

	nw4r::lyt::Pane *N_cursor_00 = layout.findPaneByName("N_cursor_00");
	UpdateSelectCursor(N_cursor_00, 0, true);
	
	//if (worldNum == nextWorldNum)
	//	return;

	worldNum = nextWorldNum;
	
	const wchar_t *worldNumber = getWorldNumber(worldNum);
	T_worldNum_00->SetString(worldNumber);


	// name
	nw4r::lyt::TextBox *T_worldName_00 = layout.findTextBoxByName("T_worldName_00");

	dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(worldNum, 100);
	const char *name;

	if (world)
		name = dLevelInfo_c::s_info.getNameForLevel(world);
	else
		name = "Superfail Nonnames";

	wchar_t wbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		wbuffer[i] = (unsigned short)name[i];
	}

	nw4r::lyt::TextBox *T_titleWorld_00 = layout.findTextBoxByName("T_titleWorld_00");
	
	// obfuscate text
	if (difficultySetting == DIFFICULTY_GOODLY_LUCK) {
		T_worldName_00->SetString(obfuscateText(wbuffer));
		T_titleWorld_00->SetString(obfuscateText(T_titleWorld_00->stringBuf));
	} else {
		T_worldName_00->SetString(wbuffer);
		WriteBMGToTextBox(T_titleWorld_00, GetBMG(), 3, 0, 0);
	}


	// star coins
	int collected = getStarCoinCountForWorld(worldNum);
	int total = getTotalStarCoinCountForWorld(worldNum);

	nw4r::lyt::TextBox *T_coinCount_00 = layout.findTextBoxByName("T_coinCount_00");
	nw4r::lyt::TextBox *T_coinCount_01 = layout.findTextBoxByName("T_coinCount_01");

	WriteNumberToTextBox(&collected, T_coinCount_00, 0);
	WriteNumberToTextBox(&total, T_coinCount_01, 0);


	// exits
	nw4r::lyt::TextBox *T_exitCount_00 = layout.findTextBoxByName("T_exitCount_00");

	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int exitCount = 0;

	dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(worldNum + 1);
	for (int i = 0; i < section->levelCount; i++) {
		dLevelInfo_c::entry_s *level = &section->levels[i];
		u32 conds = save->GetLevelCondition(level->worldSlot, level->levelSlot);

		if ((level->flags & 0x10) && (conds & COND_NORMAL))
			exitCount++;
		if ((level->flags & 0x20) && (conds & COND_SECRET))
			exitCount++;
	}
	WriteNumberToTextBox(&exitCount, T_exitCount_00, 0);
}

void dWorldSelect_c::finalizeState_ExitAnimeEndWait() {
	for (int i = 0; i < _294 + 1; i++) {
		islandPtr[i]->_26C = 0;
	}

	for (int i = 0; i < 9; i++) {
		islandPtr[i]->unk3[7] = 0;
	}

	CancelSelectCursor(dSelectCursor_c::instance, 0);
}