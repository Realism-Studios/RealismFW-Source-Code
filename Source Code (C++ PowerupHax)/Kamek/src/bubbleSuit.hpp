#pragma once

#include <game.hpp>
#include "powerupData.hpp"

class dSuitBubble_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDraw();
	int onDelete();

    void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();
	
    int playerID;
    int timer;
    void popTheBubble();
    bool canBePopped;
    bool hasTrappedEnemy;
	bool hasCoin;
	dStageActor_c *coin;
	dEn_c *victim;
	nw4r::snd::SoundHandle handle;
	nw4r::snd::SoundHandle coinHandle;
	int playerIDForLimit; //540
	int playerIDForLimit2; //544

	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
    bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther); 

	void addScoreWhenHit(void *other);
	void spawnHitEffectAtPosition(Vec2 pos);
	void doSomethingWithHardHitAndSoftHitEffects(Vec pos);
	void playEnemyDownSound2();
	void playHpdpSound1(); // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
	void playEnemyDownSound1();
	void playEnemyDownComboSound(void *player); // AcPy_c/daPlBase_c?
	void playHpdpSound2(); // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
	void _vf260(void *other); // AcPy/PlBase? plays the SE_EMY_FUMU_%d sounds based on some value
	void _vf264(dStageActor_c *other); // if other is player or yoshi, do Wm_en_hit and a few other things
	void _vf268(void *other); // AcPy/PlBase? plays the SE_EMY_DOWN_SPIN_%d sounds based on some value
	void _vf278(void *other); // AcPy/PlBase? plays the SE_EMY_YOSHI_FUMU_%d sounds based on some value
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

    USING_STATES(dSuitBubble_c);
    DECLARE_STATE(Throw);
	DECLARE_STATE(Kill);
};