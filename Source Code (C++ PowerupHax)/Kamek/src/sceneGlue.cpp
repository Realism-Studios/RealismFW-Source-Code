#include <game.hpp>
#include <profileid.hpp>
#include <stage.hpp>

extern char CurrentLevel, CurrentWorld;
extern u8 MaybeFinishingLevel[2];
extern "C" void ExitStageReal(int scene, int sceneParams, int powerupStoreType, int wipe);


extern "C" void ExitStageWrapper(int scene, int sceneParams, int powerupStoreType, int wipe) {
	// TO RE-ENABLE CUTSCENES, UNCOMMENT THIS (don't unless you know what you're doing)
	// if (scene == WORLD_MAP && powerupStoreType == BEAT_LEVEL) {
	// 	if (CurrentWorld == 6 && CurrentLevel == STAGE_DOOMSHIP) {
	// 		if (MaybeFinishingLevel[0] == 6 && MaybeFinishingLevel[1] == STAGE_DOOMSHIP) {
	// 			// We're done with 7-38
	// 			ExitStage(MOVIE, 0x10000000, powerupStoreType, wipe);
	// 			return;
	// 		}
	// 	}
	// }

	ExitStageReal(scene, sceneParams, powerupStoreType, wipe);
}

//if you're enabling newer's opening cutscene, just note that by using the default level slot (2-41)
//you'll spawn in world 2 when the cutscene ends, not my fault, it's the vanilla maps fault
//you can change it here if you want to start in World 1
extern "C" void EnterOpeningLevel() {
	DontShowPreGame = true;
	RESTART_CRSIN_LevelStartStruct.screenType = 0;
	RESTART_CRSIN_LevelStartStruct.world1 = 0;
	RESTART_CRSIN_LevelStartStruct.world2 = 0;
	RESTART_CRSIN_LevelStartStruct.level1 = 97;
	RESTART_CRSIN_LevelStartStruct.level2 = 97;
	RESTART_CRSIN_LevelStartStruct.area = 0;
	RESTART_CRSIN_LevelStartStruct.entrance = 0xFF;
	RESTART_CRSIN_LevelStartStruct.isReplay = 0; // load replay
	DoSceneChange(ProfileId::RESTART_CRSIN, 0, 0);
}

