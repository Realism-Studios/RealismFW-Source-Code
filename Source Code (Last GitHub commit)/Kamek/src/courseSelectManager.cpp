#include <game.hpp>
#include <sfx.hpp>

// handles button functionality for the options on the CourseSelectMenu
void dCourseSelectManager_c::executeState_PushAnimeEndWait() {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	
	switch (selectedMenuItem) {
		case 0: // Settings
			SetScreenFader(WIPE_FADE);
			DoSceneChange(VideoMenu, 0, false);
			break;
		
		case 1: // Star Coins Menu
			MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_IN, 1);
			collectionCoinPtr->worldNum = GetCurrentWorld();
			collectionCoinPtr->visible = true;
			state.setState(&StateID_CollectionCoinWait);
			break;
		
		case 2: // Add/Drop Players
			MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_IN, 1);
			numberOfPeopleChangePtr->isCurrentlyActive = true;
			state.setState(&StateID_CharacterChangeWait);
			break;
		
		case 3: // Save
			state.setState(&StateID_SaveWindowOpen);
			break;
		
		case 4: // Titlescreen
			state.setState(&StateID_TitleConfirmationWindowOpen);
			break;
	}
}