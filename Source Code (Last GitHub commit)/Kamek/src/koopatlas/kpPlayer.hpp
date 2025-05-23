#ifndef __KOOPATLAS_PLAYER_H
#define __KOOPATLAS_PLAYER_H

#include "koopatlas/koopatlasCore.hpp"
#include "poweruphax.hpp"
#include <playerAnim.hpp>


class daKPPlayer_c : public dActor_c {
	public:
		dPlayerModelHandler_c *modelHandler;

		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		void setupModel();

		int currentAnim;
		float currentFrame;
		float currentUnk;
		float currentUpdateRate;

		bool hasEffect;
		const char *effectName;
		bool hasSound;
		bool step;
		int soundName;

		int timer;

		float jumpOffset;

		mEf::es2 effect;
		dHammerSuitRenderer_c hammerSuit;

		bool spinning;
		s16 targetRotY;

		void setTargetRotY(s16 value) {
			targetRotY = value;
			spinning = false;
		}
		void setSpinning() {
			spinning = true;
		}

		void hide();
		void show();

		mHeapAllocator_c allocator;
		m3d::anmTexPat_c pats[4];
		void bindPats();

		void refreshPlayerModel();
		void startAnimation(int id, float frame, float unk, float updateRate);

		static dActor_c *build();
		static daKPPlayer_c *instance;
};

#endif

