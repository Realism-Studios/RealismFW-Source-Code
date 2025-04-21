#pragma once

#include <game.hpp>
#include "powerupData.hpp"

class daBoomerangHax_c : public dEn_c {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c bodyModel;
	m3d::anmTexPat_c texPat;

	int alreadyOnTop;

	float XSpeed;
	bool canI;
	u32 cmgr_returnValue;
	bool isOnTopOfTile;

	int timer;

	void playBoomerangSound();

	static dActor_c *build();

	void updateModelMatrices();
	bool calculateTileCollisions();

	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);
	
	USING_STATES(daBoomerangHax_c);
	DECLARE_STATE(Thrown);
	DECLARE_STATE(GotBack);
};

class daGabonRock_c : dEn_c { //The daGabonRock_c class, very important.
    public:
        int onCreate_orig();
        int getsettings();
};