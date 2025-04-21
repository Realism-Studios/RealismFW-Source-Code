/* THINGS I WANTED TO DO:
* improve bubble movement to match wonder
* allow more than strictly 2 bubbles
* disable enemy collisons when bubble has a coin
* allow yoshi to eat and spit out bubbles
* change how collision works 
   * thought of using yoshi's tounge interactions to control how bubble interacts with actors
   * i.e. tounge passes through actor, bubble ignores; yoshi eats actor, bubble "kills" it, etc
* spawn the flower item inside a bubble and have it move with prop movement to simulate how wonder's works
feel free to do them yourself.
*/

#include <sfx.hpp>
#include <profile.hpp>
#include "bubbleSuit.hpp"

const char* bubbleNameList [] = {0};

void dSuitBubble_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* dSuitBubble_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dSuitBubble_c));
	return new(buffer) dSuitBubble_c;
}

Profile bubbleSuitProfile(&dSuitBubble_c::build, ProfileId::BubbleSuit , NULL, ProfileId::BubbleSuit, ProfileId::BubbleSuit, "Bubble Suit Bubble", bubbleNameList);

//states and externs
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
CREATE_STATE(dSuitBubble_c, Throw);
CREATE_STATE(dSuitBubble_c, Kill);

//collisions
void dSuitBubble_c::addScoreWhenHit(void *other) {}
void dSuitBubble_c::spawnHitEffectAtPosition(Vec2 pos) {}
void dSuitBubble_c::doSomethingWithHardHitAndSoftHitEffects(Vec pos) {}
void dSuitBubble_c::playEnemyDownSound2() {}
void dSuitBubble_c::playHpdpSound1() {} // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
void dSuitBubble_c::playEnemyDownSound1() {}
void dSuitBubble_c::playEnemyDownComboSound(void *player) {} // AcPy_c/daPlBase_c?
void dSuitBubble_c::playHpdpSound2() {} // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
void dSuitBubble_c::_vf260(void *other) {} // AcPy/PlBase? plays the SE_EMY_FUMU_%d sounds based on some value
void dSuitBubble_c::_vf264(dStageActor_c *other) {} // if other is player or yoshi, do Wm_en_hit and a few other things
void dSuitBubble_c::_vf268(void *other) {} // AcPy/PlBase? plays the SE_EMY_DOWN_SPIN_%d sounds based on some value
void dSuitBubble_c::_vf278(void *other) {}

void dSuitBubble_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if (!this->hasTrappedEnemy && this->canBePopped) {
    	u16 name = ((dEn_c*)apOther->owner)->profileId;
		if (name != BubbleSuit) {OSReport("Bubble is colliding with %d\n", name);}
		// list of actors to ignore
    	if (name == ProfileId::EN_JUMPDAI || name == ProfileId::BubbleSuit || name == ProfileId::AC_BLOCK_GROUP || name == ProfileId::EN_MARUTA || name == ProfileId::EN_BAKUBAKU || name == ProfileId::EN_BIG_TOGETEKKYU || 
		name == ProfileId::EN_RAIL_POLY_ICE || name == ProfileId::LIFT_ZEN_TOGE || name == ProfileId:: EN_HANACHAN || name == ProfileId::AC_PROP_BLOCK)
    	{if (name != BubbleSuit) {OSReport("Bubble is colliding with %d, so we're ignoring it\n", name);} return; }
		// list of actors that pop
		else if (name == ProfileId::EN_CHOROBON || name == ProfileId::EN_IGA_KURIBO || name == ProfileId::EN_UNIZOO || name == ProfileId::EN_SANBO || name == ProfileId::EN_BOSS_LARRY || 
		name == ProfileId::EN_BOSS_WENDY || name == ProfileId::EN_BOSS_IGGY || name == ProfileId::EN_BOSS_LEMMY || name == ProfileId::EN_BOSS_MORTON || name == ProfileId::EN_BOSS_ROY || name == ProfileId::EN_BOSS_LUDWIG || name == ProfileId::EN_BOSS_KAMECK ||
		name == ProfileId::EN_BOSS_CASTLE_LARRY || name == ProfileId::EN_BOSS_CASTLE_WENDY|| name == ProfileId::EN_BOSS_CASTLE_IGGY || name == ProfileId::EN_BOSS_CASTLE_LEMMY || name == ProfileId::EN_BOSS_CASTLE_MORTON || name == ProfileId::EN_BOSS_CASTLE_ROY || 
		name == ProfileId::EN_BOSS_CASTLE_LUDWIG|| name == ProfileId::EN_JIMEN_PAKKUN || name == ProfileId::EN_JIMEN_BIG_PAKKUN || name == ProfileId::EN_JIMEN_FPAKKUN || name == EN_JIMEN_BIG_PAKKUN|| name == ProfileId:: EN_IGAPUKU || name == ProfileId::EN_TOGEZO ||
		name == ProfileId::EN_IBARAMUSHI)
		{OSReport("Bubble is colliding with %d, so we're popping\n", name); popTheBubble(); return; }
		else {
		OSReport("Bubble is colliding with %d, so we're killing it\n", name);
		this->victim = ((dEn_c*)apOther->owner);
        //doStateChange(&StateID_Kill); 
		((dEn_c*)apOther->owner)->Delete(1);
		this->hasTrappedEnemy = true; }
    }
    return;
}
void dSuitBubble_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
    if(hitType == (1 || 3)) {
        popTheBubble();
		this->counter_504[apOther->owner->which_player] = 0xA;
    } else {
        if(this->canBePopped)
            popTheBubble();
    }
}
void dSuitBubble_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->playerCollision(apThis, apOther);
}
bool dSuitBubble_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
    if(this->canBePopped)
		popTheBubble();
    return true;
}
bool dSuitBubble_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
    char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
    if(hitType == (1 || 3)) {
        popTheBubble();
    } else {
        if(this->canBePopped)
            popTheBubble();
    }
	return true;
}
bool dSuitBubble_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dSuitBubble_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
void dSuitBubble_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
		nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
		this->animationChr.bind(&this->bodyModel, anmChr, unk);
		this->bodyModel.bindAnim(&this->animationChr, unk2);
		this->animationChr.setUpdateRate(rate);
}

int dSuitBubble_c::onCreate() {
    nw4r::snd::SoundHandle bubbleHandle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &bubbleHandle, SE_PLY_BALLOON_SELF, 1);

    playerID = (settings >> 24) & 0xF;
	dAcPy_c *player = dAcPy_c::findByID(playerID);
	int variation2 = (settings >> 4) & 0xF;
	this->pos = (Vec){player->pos.x + ((player->direction == 1) ? -16 : 16), player->pos.y + 16, player->pos.z};
    float randDist = (float(MakeRandomNumber(4)) / 4.0f);
	this->settings = 0 | (playerID << 4) | (player->direction << 8) | (variation2 << 12);

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("balloon", "g3d/balloon.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("balloon");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

    bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("float");
	ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink(); 

	this->scale.x = 0.8; 
	this->scale.y = 0.8; 
	this->scale.z = 0.8;
	this->direction = (this->settings >> 8) & 0xF; // Heading where the player look.
	this->speed.x = (this->direction == 1) ? -(3.5f + randDist) : (3.5f + randDist);
	this->x_speed_inc = 0.1;

    ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 10;
	HitMeBaby.yDistToEdge = 10;		

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x8028E;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

    bindAnimChr_and_setUpdateRate("float", 1, 0.0, 1.0);

    doStateChange(&StateID_Throw);

	this->onExecute();
	return true;
}

int dSuitBubble_c::onExecute() {
    acState.execute();
	updateModelMatrices();
	bodyModel._vf1C();
    this->timer++;

    if(this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	if(this->timer > 300) {
        this->popTheBubble();
	}

	if (this->hasTrappedEnemy && !this->hasCoin) {
		coin = CreateActor(BubbleCoin, 0x3, this->pos, 0, 0);
		this->hasCoin = true;
	} else if (this->hasCoin) {
		coin->pos = this->pos;
	}
	return true;
}

int dSuitBubble_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

int dSuitBubble_c::onDelete() {
	projectileCounts[Powerup::BubbleFlower][which_player]--;
	this->Delete(1);
	return true;
}

//throwing
void dSuitBubble_c::beginState_Throw() {
	this->scale = (Vec){0.2, 0.2, 0.2};
}
void dSuitBubble_c::executeState_Throw() { 
	HandleXSpeed();
	doSpriteMovement();
	if(this->direction == 1) {
		if(this->speed.x > 0) {
			this->speed.x = 0;
			this->x_speed_inc = 0;
		}
	}
	if(this->direction == 0) {
		if(this->speed.x < 0) {
			this->speed.x = 0;
			this->x_speed_inc = 0;
		}
	}
	if (this->scale.x < 0.8)
		this->scale = (Vec){this->scale.x + 0.05, this->scale.y + 0.05, this->scale.z + 0.05};
	else
		this->canBePopped = true;
}
void dSuitBubble_c::endState_Throw() {}

//killing
void dSuitBubble_c::beginState_Kill() {
	this->canBePopped = false;
}
void dSuitBubble_c::executeState_Kill() {
	this->victim->pos = this->pos;
	//this->victim->rot.x += 0x400;
	this->victim->scale = (Vec){this->victim->scale.x - 0.05, this->victim->scale.y - 0.05, this->victim->scale.z - 0.05};
	if (this->victim->scale.x <= 0)
		this->victim->Delete(1);
		this->hasTrappedEnemy = true;
		this->canBePopped = true;
}
void dSuitBubble_c::endState_Kill() {}

void dSuitBubble_c::popTheBubble() {
	PlaySoundWithFunctionB4(SoundRelatedClass, &coinHandle, SE_PLY_BALLOON_BRAKE, 1);
	Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
	S16Vec nullRot = {0,0,0};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	SpawnEffect("Wm_mr_balloonburst", 0, &efPos, &nullRot, &oneVec);
    if (this->hasTrappedEnemy) {
		coin->Delete(1);
	    int coinsettings = 0 | (0x2 << 0) | (2 << 18) | (4 << 9) | (2 << 10) | ((playerID + 8) << 16);
		VEC3 newPos = {this->pos.x, this->pos.y - 16, this->pos.z};
		dStageActor_c *coin = dStageActor_c::create(EN_ITEM, coinsettings, &newPos, 0, 0);
		PlaySoundWithFunctionB4(SoundRelatedClass, &coinHandle, SE_OBJ_GET_COIN, 1);
    }
	this->Delete(1);
}

//coin, should probably be part of the bubble but fuck that I geuss 

class dBubbleCoin_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDraw();

	void updateModelMatrices();
	

	static dActor_c* build();
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	nw4r::g3d::ResFile resFile;

};


void dBubbleCoin_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* dBubbleCoin_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dBubbleCoin_c));
	return new(buffer) dBubbleCoin_c;
}

Profile BubbleCoinProfile(&dBubbleCoin_c::build, ProfileId::BubbleCoin, NULL, ProfileId::BubbleCoin, ProfileId::BubbleCoin, "Bubble Coin", bubbleNameList);

int dBubbleCoin_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("obj_coin", "g3d/obj_coin.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("obj_coin");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	allocator.unlink(); 

	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;

	this->onExecute();

	return true;
}

int dBubbleCoin_c::onExecute() {
	updateModelMatrices();
	bodyModel._vf1C();

    this->rot.y += 0x400;

	return true;
}

int dBubbleCoin_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}