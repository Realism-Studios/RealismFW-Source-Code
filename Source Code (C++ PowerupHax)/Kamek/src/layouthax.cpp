#include <game.hpp>

bool dControllerInformation_c::newLoadLayout() {
	const char *filename = "controllerMisinformation.arc";

	// choose random layout
	int chosenLyt = GenerateRandomNumber(4);
	if (chosenLyt == 3)
		filename = "controllerInformation.arc";

	if (!layout.loadArc(filename, false))
		return false;
	
	layout.build("controllerInformation_07.brlyt");

	static const char *brlanNames[3] = {
		"ControllerInformation_07_in2btn.brlan",
		"ControllerInformation_07_loop2btn.brlan",
		"ControllerInformation_07_hit2btn.brlan"
	};
	layout.loadAnimations(brlanNames, 3);

	static const char *groupNames[3] = {
		"A00_2btn_00", "A00_2btn_00", "A00_2btn_00"
	};
	static const int groupIDs[3] = {0, 1, 2};
	layout.loadGroups(groupNames, groupIDs, 3);

	static const char *textBoxNames[3] = {
		"T_contInfo_00",
		"T_contInfo_01"
	};
	static const int msgIDs[2] = {0x8, 0x8};
	layout.setLangStrings(textBoxNames, msgIDs, 0x68, 2);

	if (chosenLyt == 3) {
		// this is the new shit
		nw4r::lyt::Picture *P_window_01 = layout.findPictureByName("P_window_01");
		int chance = GenerateRandomNumber(69420);
		if (chance == 45067)
			P_window_01->SetVisible(true);
		// already invisible in layout so no need to hide it otherwise
	}

	return true;
}

// this kinda sucks but idc
void dOtasukeInfo_c::writeMessageStrings() {
	dScript::Res_c *msgRes = GetBMG();
	int screenType = m_startGameInfo.screenType;
	static const char *textBoxNames[] = {"T_infoS_00", "T_info_00", "T_yes_00", "T_yes_01", "T_no_00", "T_no_01"};

	// yuck
	static const int messageIDs[] = {0xA, 0xA, 0xB, 0xB, 0xC, 0xC}; // hitting the guide block in a regular stage
	static const int guideMsgIDs[] = {0xE, 0xE, 0xF, 0xF, 0x10, 0x10}; // pressing plus in super guide, if luigi is still replay-controlled
	static const int hintMsgIDs[] = {0x15, 0x15, 0x16, 0x16, 0x17, 0x17}; // pressing plus in hint movies

	switch (screenType) {
		case 0: // regular stage
			layout.setLangStrings(textBoxNames, messageIDs, 1, 6);
			break;
		case 1: // super guide
			layout.setLangStrings(textBoxNames, guideMsgIDs, 1, 6);
			break;
		default: // everything else, such as hint movies
			layout.setLangStrings(textBoxNames, hintMsgIDs, 1, 6);
			break;
	};
	
	// print special message if the player is luigi, and it's a normal stage
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	if ((screenType == 0) && (save->lastP1Character == 1)) {
		WriteBMGToTextBox(T_infoS_00, msgRes, 1, 9, 0);
		WriteBMGToTextBox(T_info_00, msgRes, 1, 9, 0);
	}
}

// doesn't work
/*extern "C" bool CollectionCoinBase_NewLoadLayout(int self) {
	m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)((int)(self) + 0xA0);
	nw4r::lyt::Pane *rootPane = (nw4r::lyt::Pane*)((int)(self) + 0x238);
	nw4r::lyt::Pane *N_coinDateAll_00 = (nw4r::lyt::Pane*)((int)(self) + 0x23C); // all the coinBarPos panes come after this
	int thing = ((int)(self) + 0x124);

	// check if layout archive was loaded
	if (thing != 0) {
		OSReport("Starting getting all of the shit\n");
		layout->build("worldCollectionCoinBase_01.brlyt");

		OSReport("built layout, getting rootPane\n");
		rootPane = layout->getRootPane();

		OSReport("got rootPane, getting test pane\n");
		nw4r::lyt::Picture *P_winBg_00;
		layout->findPictureByName("P_winBg_00");
		
		OSReport("got rootPane, getting other panes\n");
		static const char *paneNames[] = {
			"N_coinDateAll_00", "N_coinBarPos_00", "N_coinBarPos_01",
			"N_coinBarPos_02", "N_coinBarPos_03", "N_coinBarPos_04",
			"N_coinBarPos_05", "N_coinBarPos_06", "N_coinBarPos_07",
			"N_coinBarPos_08", "N_coinBarPos_09", "N_coinBarPos_10",
			"N_coinBarPos_11"
		};
		layout->getPanes(paneNames, &N_coinDateAll_00, 0xD);
		OSReport("did it!\n");
	}

	return true;
}*/
