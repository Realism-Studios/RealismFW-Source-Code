#include <common.hpp>
#include <game.hpp>
#include <g3dhax.hpp>
#include <profileid.hpp>
#include <sfx.hpp>
#include <stage.hpp>
#include "boss.hpp"

int KeyCounter = 0;
extern "C" void *KeyLoopSet(dStageActor_c *);

void endKeyLevel(dStageActor_c *key) {
	OSReport("Key End Level");

	if (key->settings) {
		KeyCounter += 1;
		OSReport("Key Counter now at: %d", KeyCounter);
		ExitStage(ProfileId::WORLD_MAP, 0, BEAT_LEVEL, MARIO_WIPE);
	}
}

void startEndKeyLevel(dStageActor_c *key) {
	key->speed.y = 4.0;
	OSReport("Key Set Speed");

	if (key->settings) {
		StopBGMMusic();
		PlaySound(key, STRM_BGM_COURSE_CLEAR_ZORO);
		MakeMarioEnterDemoMode();
	}
}

void soundsEndKeyLevel(dStageActor_c *key) {
	OSReport("Key Set Loop");
	KeyLoopSet(key);

	if (key->settings) {
		UpdateGameMgr();
		if (GetSpecificPlayerActor(0) != 0) { PlaySound(key, SE_VOC_MA_CLEAR_MULTI); }
		if (GetSpecificPlayerActor(1) != 0) { PlaySound(key, SE_VOC_LU_CLEAR_MULTI); }
		if (GetSpecificPlayerActor(2) != 0) { PlaySound(key, SE_VOC_KO_CLEAR_MULTI); }
		if (GetSpecificPlayerActor(3) != 0) { PlaySound(key, SE_VOC_KO2_CLEAR_MULTI); }
	}
}


