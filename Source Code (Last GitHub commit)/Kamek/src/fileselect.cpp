#include <game.hpp>
#include <sfx.hpp>

// only affects story mode buttons
void dSelectPlayer_c::newButtonFunctionality() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	// return to file selection
	if (nowPressed & WPAD_ONE) {
		MapSoundPlayer(SoundRelatedClass, SE_SYS_BACK, 0);
		exitBehavior = 0xFFFFFFFF; // returns to file select
		state.setState(&StateID_ExitAnimeEndWait);
		return;
	}

	// press our current button
	if (nowPressed & WPAD_TWO) {
		exitBehavior = 1;

		if (currentChoice == 0) {
			int chance = GenerateRandomNumber(420);
			if (chance == 69)
				MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_PLAYER_DECIDE_MAME, 1);
			else
				MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_PLAYER_DECIDE, 1);
		}
		state.setState(&StateID_StartMemberButtonAnime);
		return;
	}

	// go to prior button (unless we're on 1P)
	if (currentChoice != 0) {
		if (nowPressed & WPAD_LEFT) {
			currentChoice--;
			state.setState(&StateID_ButtonChangeAnimeEndWait);
			return;
		}
	}

	// go to next button (4P will play invalid sound for fake button)
	if (nowPressed & WPAD_RIGHT) {
		if (currentChoice == 3) {
			MapSoundPlayer(SoundRelatedClass, SE_SYS_INVALID, 1);
			return;
		} else {
			currentChoice++;
			state.setState(&StateID_ButtonChangeAnimeEndWait);
			return;
		}
	}
}




bool dFileSelect_c::newLoadLayout() {
	bool gotFile = layout.loadArc("fileSelectBase.arc", false);
	if (!gotFile)
		return false;
	
	layout.build("fileSelectBase_18.brlyt");

	static const char *brlanNames[] = {
		"unfinished :pensive:",
	};

	return true;
}

void dFileSelect_c::newButtonFunctionality() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());
	int selection = currentButton;
	OSReport("current is %i\n", selection);

	// test for videos menu
	if (nowPressed & WPAD_B) {
		DoSceneChange(VideoMenu, 0, false);
	}

	// on one of the save files
	if (selection < 3) {
		if ((nowPressed & WPAD_LEFT) && (currentButton > 0)) {
			selection--;
			selectedOnTopRow = selection;
			//state.setState(&StateID_ButtonSelectAnimeEndWait);
		}

		if ((nowPressed & WPAD_RIGHT) && (currentButton < 2)) {
			selection++;
			selectedOnTopRow = selection;
			//state.setState(&StateID_ButtonSelectAnimeEndWait);
		}

		if (nowPressed & WPAD_UP) {
			// goes to luigi button
		}

		if (nowPressed & WPAD_DOWN) {
			selection += 3;
			//state.setState(&StateID_ButtonSelectAnimeEndWait);
		}

	// middle row
	} else if ((currentButton > 2) && (currentButton < 6)) {
		// returning to the save files
		if (nowPressed & WPAD_UP) {
			//previousButton = currentButton;
			selection -= 3;
			state.setState(&StateID_ButtonSelectAnimeEndWait);
		}
		
		if ((nowPressed & WPAD_RIGHT) && (currentButton < 5)) {

		}

	// bottom row
	} else if ((selection > 5) && (selection < 8)) {


	// luigi button
	} else {

	}


	if (selection == currentButton) {
		int output = returnSelectedFile();
		
		// try to delete a file
		if (nowPressed & WPAD_MINUS) {
			if (output == 3) // all three files are new, don't allow this action
				MapSoundPlayer(SoundRelatedClass, SE_SYS_INVALID, 1);
			else {
				action_isDelete = 1;
				currentCopyOrDeleteSelection = output;
				if (currentButton < 3)
					state.setState(&StateID_FromCopyOrDeleteBeforeButtonReturn);
				else
					state.setState(&StateID_MultiButtonExitAnimeEndWait);
				
				MapSoundPlayer(SoundRelatedClass, SE_SYS_FS_DELETE, 1);
			}
		}

		// try to copy a file
		if (nowPressed & WPAD_PLUS) {
			if (output == 3)
				MapSoundPlayer(SoundRelatedClass, SE_SYS_INVALID, 1);
			else {
				field_2DA = 0;
				action_isDelete = 0;
				currentCopyOrDeleteSelection = output;
				if (currentButton < 3)
					state.setState(&StateID_FromCopyOrDeleteBeforeButtonReturn);
				else
					state.setState(&StateID_MultiButtonExitAnimeEndWait);
				
				MapSoundPlayer(SoundRelatedClass, SE_SYS_FS_COPY, 1);
			}
		}

		// return to the titlescreen
		if (nowPressed & WPAD_ONE) {
			MapSoundPlayer(SoundRelatedClass, SE_SYS_BACK, 1);
			state.setState(&StateID_ReturnTitle);
			return;
		}

		// select whatever button we're on
		if (nowPressed & WPAD_TWO | WPAD_A) {
			if (currentButton < 3)
				dateFiles[currentButton]->activatesHit = 1;

			state.setState(&StateID_ButtonHitAnimeEndWait);
			return;
		}
	} else {
		// update current button
		previousButton = currentButton;
		currentButton = selection;
		state.setState(&StateID_ButtonSelectAnimeEndWait);
		return;
	}
}