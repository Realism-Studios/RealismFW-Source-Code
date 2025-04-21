#include "settingMenu.hpp"
#include "settingMenuArrays.hpp"

CREATE_STATE(dSettingMenu_c, InitWait);
CREATE_STATE(dSettingMenu_c, Appear);
CREATE_STATE(dSettingMenu_c, ButtonChange);
CREATE_STATE(dSettingMenu_c, WaitForInput);
CREATE_STATE(dSettingMenu_c, ButtonHit);
CREATE_STATE(dSettingMenu_c, ShowInfo);
CREATE_STATE(dSettingMenu_c, Close);

const SpriteData SettingMenuSpriteData = {ProfileId::SettingMenu, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile SettingMenuProfile(&dSettingMenu_c::build, NULL, &SettingMenuSpriteData, ProfileId::WM_NOTE, ProfileId::SettingMenu, "Settings Menu", 0);

dSettingMenu_c *dSettingMenu_c::instance = 0;

dActor_c *dSettingMenu_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dSettingMenu_c));
	dSettingMenu_c *c = new(buffer) dSettingMenu_c;

	instance = c;
	return c;
}


int dSettingMenu_c::onCreate() {
	infoWindow = (dInfoWindow_c*)CreateParentedObject(INFO_WINDOW, this, 0, 0);
	
	if (!layoutLoaded) {
		bool gotFile = layout.loadArc("settingMenu.arc", false);
		if (!gotFile)
			return false;

		layout.build("settingMenu.brlyt");

		// anim related info is in the header
		// layout.loadAnimations(brlanNames, 10);
		// layout.loadGroups(groupNames, groupIDs, 40);

		rootPane = layout.getRootPane();

		// static const char *rightNames[] = {
		// 	"N_button_00", "N_button_01", "N_button_02",
		// 	"T_settingS_00", "T_settingS_01", "T_settingS_02",
		// };
		// layout.getPanes(rightNames, &rightPanes[0], 6);

		// static const char *btnNames[] = {
		// 	"P_buttonE_00", "P_buttonE_01", "P_buttonE_02",
		// 	"P_buttonD_00", "P_buttonD_01", "P_buttonD_02",
		// 	"P_infoButton_00", "P_infoButton_01", "P_infoButton_02",
		// 	"P_centerBase_00"
		// };
		// layout.getPictures(btnNames, &P_buttonE_0[0], 10);

		// static const char *txtNames[] = {
		// 	"T_buttonTxt_00", "T_buttonTxt_01", "T_buttonTxt_02",
		// 	"T_buttonTxtS_00", "T_buttonTxtS_01", "T_buttonTxtS_02",
		// 	"T_info_00", "T_info_01", "T_info_02",
		// 	"T_infoS_00", "T_infoS_01", "T_infoS_02",
		// };
		// layout.getTextBoxes(txtNames, &T_buttonTxt_0[0], 12);

		for (int i = 0; i < TOTAL_SETTING_COUNT; i++) {
			char buffer[16];
			sprintf(buffer, "N_buttonN_0%d", i);
			N_buttonN[i] = layout.findPaneByName(buffer);

			sprintf(buffer, "N_buttonY_0%d", i);
			N_buttonY[i] = layout.findPaneByName(buffer);

			sprintf(buffer, "N_cursor_0%d", i);
			N_cursor[i] = layout.findPaneByName(buffer);
		}

		const char *textBoxLangNames[] = {"T_back_00", "T_info_00"};
		int msgIDs[] = {0, 1};
		layout.setLangStrings(textBoxLangNames, msgIDs, 7, 2);

		//layout.resetAnim(0);

		//layout.disableAllAnimations();

		//rootPane->SetVisible(false);

		layout.drawOrder = 140;

		layoutLoaded = true;
	}

	return true;
}

int dSettingMenu_c::beforeExecute() {
	if (dBase_c::beforeExecute())
		return (QueryGlobal5758(-1) == 0);

	return false;
}

int dSettingMenu_c::onExecute() {
	if (layoutLoaded && visible) {
		state.execute();

		//layout.execAnimations();
		//layout.update();
	}

	return true;
}

int dSettingMenu_c::onDelete() {
	OSReport("unloading setting menu!!!\n");
	return layout.free();
}

int dSettingMenu_c::onDraw() {
	if (visible)
		layout.scheduleForDrawing();

	return true;
}

// yikes
void dSettingMenu_c::toggleButtons() {
	// handle setting states
	settingState[0] = collisionDebugEnabled;
	settingState[1] = replayRecordEnabled;
	settingState[2] = starCoinRadarEnabled;
	settingState[3] = tryAgainEnabled;
	
	for (int i = 0; i < TOTAL_SETTING_COUNT; i++) {
		if (settingState[i]) {
			N_buttonY[i]->SetVisible(true);
			N_buttonN[i]->SetVisible(false);
		} else {
			N_buttonY[i]->SetVisible(false);
			N_buttonN[i]->SetVisible(true);
		}
	}
}



void dSettingMenu_c::beginState_InitWait() {}
void dSettingMenu_c::executeState_InitWait() {
	currentButton = 0;
	currentDiffButton = difficultySetting;

	toggleButtons();

	//rootPane->SetVisible(true);

	/*layout.disableAllAnimations();

	for (int i = 9; i < 16; i++) {
		layout.resetAnim(i);
	}

	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	for(int i = 0; i < 3; i++) {
		T_info_0[i]->SetVisible(false);
		T_infoS_0[i]->SetVisible(false);

		bool setting = save->pad[0x10 + i];

		P_buttonE_0[i]->SetVisible(setting);
		P_buttonD_0[i]->SetVisible(!setting);

		//WriteBMGToTextBox(buttonTextBoxes[i], GetBMG(), 0x3EA, 16+(!setting), 0);
		//WriteBMGToTextBox(buttonTextBoxesS[i], GetBMG(), 0x3EA, 16+(!setting), 0);
	}

	layout.enableNonLoopAnim(0);
	layout.enableNonLoopAnim(37);*/

	state.setState(&StateID_Appear);
}
void dSettingMenu_c::endState_InitWait() {}


void dSettingMenu_c::beginState_Appear() {}
void dSettingMenu_c::executeState_Appear() {
	/*if(!layout.isAnimOn(0) && !layout.isAnimOn(37)) {
		layout.enableNonLoopAnim(9);

		state.setState(&StateID_ButtonChange);
	}*/
	state.setState(&StateID_WaitForInput);
}
void dSettingMenu_c::endState_Appear() {}


void dSettingMenu_c::beginState_ButtonChange() {
	HideSelectCursor(SelectCursorPointer, 0);
}
void dSettingMenu_c::executeState_ButtonChange() {
	/*if(!layout.isAnimOn(9) && !layout.isAnimOn(10) && !layout.isAnimOn(11) && !layout.isAnimOn(12) && !layout.isAnimOn(13) && !layout.isAnimOn(14) && !layout.isAnimOn(15)) {
		int btnID = -1;

		if(this->selectedRow == 3) btnID = 9;
		else if(this->selectedColumn == 1) btnID = this->selectedRow + 6;
		else {
			SaveBlock *save = GetSaveFile()->GetBlock(-1);
			if(save->pad[0x10 + this->selectedRow]) btnID = this->selectedRow;
			else btnID = this->selectedRow + 3;
		}

		UpdateSelectCursor(enabledButtons[btnID], 0, false);

		state.setState(&StateID_WaitForInput);
	}*/
	UpdateSelectCursor(N_cursor[currentButton], 0, false);
}
void dSettingMenu_c::endState_ButtonChange() {}


void dSettingMenu_c::beginState_WaitForInput() {}
void dSettingMenu_c::executeState_WaitForInput() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	/*selectedRow = difficultySetting; // using this for difficulty
	//nw4r::lyt::TextBox *T_setting_00 = layout.findTextBoxByName("T_setting_00");

	if (selectedRow < 4) {
		if (nowPressed & WPAD_PLUS) {
			selectedRow++;
			difficultySetting = selectedRow;
			MapSoundPlayer(SoundRelatedClass, SE_OBJ_LIFT_LIMIT_2, 1);
		}
	}

	if (selectedRow > 0) {
		if (nowPressed & WPAD_MINUS) {
			selectedRow--;
			difficultySetting = selectedRow;
			MapSoundPlayer(SoundRelatedClass, SE_OBJ_LIFT_LIMIT_3, 1);
		}
	}*/

	if (nowPressed & WPAD_A) {
		
	}

	/*char string[16];
	sprintf(string, "Difficulty: %i", selectedRow);

	wchar_t buffer[16];
	for (int i = 0; i < 16; i++) {
		buffer[i] = (unsigned short)string[i];
	}
	T_setting_00->SetString(buffer);*/

	/*if((nowPressed & WPAD_RIGHT  || nowPressed & WPAD_LEFT) && this->selectedRow != 3) {
		layout.enableNonLoopAnim(30 + this->selectedRow + (this->selectedColumn * 3));

		this->selectedColumn = !this->selectedColumn;

		layout.enableNonLoopAnim(9 + this->selectedRow + (this->selectedColumn * 3));

		MapSoundPlayer(SoundRelatedClass, 0x78, 1);
		state.setState(&StateID_ButtonChange);
	}
	else if(nowPressed & WPAD_UP) {
		layout.enableNonLoopAnim((this->selectedRow == 3) ? 36 : (30 + this->selectedRow + (this->selectedColumn * 3)));

		if(this->selectedRow == 0) this->selectedRow = 3;
		else this->selectedRow--;

		layout.enableNonLoopAnim((this->selectedRow == 3) ? 15 : (9 + this->selectedRow + (this->selectedColumn * 3)));

		MapSoundPlayer(SoundRelatedClass, 0x78, 1);
		state.setState(&StateID_ButtonChange);
	}
	else if(nowPressed & WPAD_DOWN) {
		layout.enableNonLoopAnim((this->selectedRow == 3) ? 36 : (30 + this->selectedRow + (this->selectedColumn * 3)));

		if(this->selectedRow == 3) this->selectedRow = 0;
		else this->selectedRow++;

		layout.enableNonLoopAnim((this->selectedRow == 3) ? 15 : (9 + this->selectedRow + (this->selectedColumn * 3)));

		MapSoundPlayer(SoundRelatedClass, 0x78, 1);
		state.setState(&StateID_ButtonChange);
	}
	else if(nowPressed & WPAD_TWO) {
		layout.enableNonLoopAnim((this->selectedRow == 3) ? 29 : (23 + this->selectedRow + (this->selectedColumn * 3)));

		MapSoundPlayer(SoundRelatedClass, 0x79, 1);
		state.setState(&StateID_ButtonHit);
	}
	else */if(CheckIfMenuShouldBeCancelledForSpecifiedWiimote(0)) {
		//layout.enableNonLoopAnim(38);
		//layout.enableNonLoopAnim(39);

		//HideSelectCursor((void*)SelectCursorPointer, 0);
		state.setState(&StateID_Close);
	}
}
void dSettingMenu_c::endState_WaitForInput() {}


void dSettingMenu_c::beginState_ButtonHit() {
	timer = 0;

	//if(this->selectedRow == 3) HideSelectCursor((void*)SelectCursorPointer, 0);
}
void dSettingMenu_c::executeState_ButtonHit() {
	/*if(this->selectedColumn == 0 && this->selectedRow != 3 && this->timer < 3) {
		this->timer++;

		if(this->timer == 2) {
			SaveBlock *save = GetSaveFile()->GetBlock(-1);

			bool setting = save->pad[0x10 + this->selectedRow];

			save->pad[0x10 + this->selectedRow] = !setting;

			enabledButtons[this->selectedRow]->SetVisible(!setting);
			disabledButtons[this->selectedRow]->SetVisible(setting);

			WriteBMGToTextBox(buttonTextBoxes[this->selectedRow], GetBMG(), 0x3EA, 16+(setting), 0);
			WriteBMGToTextBox(buttonTextBoxesS[this->selectedRow], GetBMG(), 0x3EA, 16+(setting), 0);
		}
	}

	if(this->selectedColumn == 1 && this->selectedRow != 3) {
		this->timer++;

		if(this->timer == 2) {
			for(int i = 0; i < 6; i++) {
				rightPanes[i]->SetVisible(this->isOnInfo);
			}
			centerButton->SetVisible(this->isOnInfo);

			for(int i = 0; i < 3; i++) {
				if(i != this->selectedRow) {
					infoButtons[i]->SetVisible(this->isOnInfo);
				}
				else {
					infoTextBoxes[i]->SetVisible(!this->isOnInfo);
					infoTextBoxesS[i]->SetVisible(!this->isOnInfo);
				}
			}

			this->isOnInfo = !this->isOnInfo;
		}
	}

	if(!layout.isAnimOn(23) && !layout.isAnimOn(24) && !layout.isAnimOn(25) && !layout.isAnimOn(26) && !layout.isAnimOn(27) && !layout.isAnimOn(28) && !layout.isAnimOn(29)) {
		if(this->selectedRow == 3) {
			layout.enableNonLoopAnim(38);
			layout.enableNonLoopAnim(39);

			state.setState(&StateID_Close);
		}
		else if(this->selectedColumn == 1 && this->isOnInfo) {
			state.setState(&StateID_ShowInfo);
		}
		else {
			state.setState(&StateID_WaitForInput);
		}
	}*/
}
void dSettingMenu_c::endState_ButtonHit() {}


void dSettingMenu_c::beginState_ShowInfo() {}
void dSettingMenu_c::executeState_ShowInfo() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	// if (nowPressed & WPAD_TWO) {
	// 	layout.enableNonLoopAnim(23 + this->selectedRow + (this->selectedColumn * 3));

	// 	MapSoundPlayer(SoundRelatedClass, SE_SYS_DECIDE, 1);
	// 	state.setState(&StateID_ButtonHit);
	// }
}
void dSettingMenu_c::endState_ShowInfo() {}


void dSettingMenu_c::beginState_Close() {
	copySettingsToSave();
	MapSoundPlayer(SoundRelatedClass, SE_SYS_BACK, 1);
}
void dSettingMenu_c::executeState_Close() {
	//if (!layout.isAnimOn(38) && !layout.isAnimOn(39)) {
		visible = false;

		state.setState(&StateID_InitWait);
	//}
}
void dSettingMenu_c::endState_Close() {}
