#include <game.hpp>

extern char CurrentWorld;

// for dScene_c::setNextScene
extern "C" void WorldMapRedirect(int doesNotMatter, int settings, bool exitingStage) {
	// world 5
	/*if ((settings & 0xF) == 4) {
		DoSceneChange(Koopatlas, settings, false);
		return;
	}*/

	DoSceneChange(WORLD_MAP, settings, false);
}

// for ReturnToAnotherSceneAfterLevel
extern "C" void ReturnToWorldMapAfterLevel(int doesNotMatter, int settings, int powerupStore, int wipe) {
	// world 5
	if (CurrentWorld == 4) {
		ReturnToAnotherSceneAfterLevel(Koopatlas, settings, powerupStore, wipe);
		return;
	}

	ReturnToAnotherSceneAfterLevel(WORLD_MAP, settings, powerupStore, wipe);
}