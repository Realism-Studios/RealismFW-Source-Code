#include <game.hpp>
#include "profile.hpp"


const char* pakkun_tane [] = { "pakkun_tane", NULL };

class pakkunTane : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

    mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c bodyModel;

	m3d::anmChr_c animationChr;
	m3d::anmTexPat_c animationPat;
	nw4r::g3d::ResAnmTexPat anmPat;

	mEf::es2 effect;

	void updateModelMatrices();


	static dActor_c* build();

	void spawnPakkun();
	u32 cmgr_returnValue;

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
    //bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);

	void _vf148();
	void _vf14C();
	bool CreateIceActors();
};


void pakkunTane::updateModelMatrices() {
	// This won't work with wrap because I'm lazy.

	if (this->frzMgr._mstate == 1)
		matrix.translation(pos.x, pos.y + 2.0, pos.z);
	else
		matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* pakkunTane::build() {
	void *buffer = AllocFromGameHeap1(sizeof(pakkunTane));
	return new(buffer) pakkunTane;
}

const SpriteData pakkunTaneData = { ProfileId::pakkunTane, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile pakkunTaneProfile(&pakkunTane::build, SpriteId::pakkunTane, &pakkunTaneData, ProfileId::pakkunTane, ProfileId::pakkunTane, "pakkunTane", pakkun_tane, 0);

///////////////////////
// Externs and States
///////////////////////
extern "C" bool SpawnEffect(const char*, int, Vec*, S16Vec*, Vec*);

//FIXME make this dEn_c->used...
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
// extern "C" int SomeStrangeModification(dStageActor_c* actor);
// extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk);
//extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);
// extern "C" void addToList(StandOnTopCollider *self);

// extern "C" bool HandlesEdgeTurns(dEn_c* actor);


int pakkunTane::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("pakkun_tane", "g3d/pakkun_tane.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("pakkun_tane");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	allocator.unlink();

	this->scale.x = 1.0;
	this->scale.y = 1.0;
	this->scale.z = 1.0;

	// These structs tell stupid collider what to collide with - these are from koopa troopa
	static const lineSensor_s below(-5<<12, 5<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(6<<12, 9<<12, 6<<12);

	collMgr.init(this, &below, &above, &adjacent);
    collMgr.calculateBelowCollisionWithSmokeEffect();

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 2.0;
	HitMeBaby.xDistToEdge = 8.0;
	HitMeBaby.yDistToEdge = 8.0;
	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4f;
	HitMeBaby.bitfield2 = 0xFFFFFFFF;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;
	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->max_speed.y = -1.0;
	this->speed.y = -1.0;
	this->y_speed_inc = -0.1875;

	this->onExecute();

	return true;
}

int pakkunTane::onExecute() {
	updateModelMatrices();
	bodyModel._vf1C();
	// spawn it if it's on the ground
	if (collMgr.isOnTopOfTile() == true) {
        spawnPakkun();
        this->Delete(1);
	}
	// looping "animation"
	this->rot.x += 0x200;
	// the gravity's
	HandleYSpeed();
	UpdateObjectPosBasedOnSpeedValuesReal();


	return true;
}

int pakkunTane::onDelete() {
    this->aPhysics.removeFromList();
    return true;
}

int pakkunTane::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

////////////////////////
// Collision Functions
////////////////////////
void pakkunTane::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	u16 name = ((dEn_c*)apOther->owner)->profileId;

	if (name == EN_COIN || name == EN_EATCOIN || name == AC_BLOCK_COIN || name == EN_COIN_JUGEM || name == EN_COIN_ANGLE
		|| name == EN_COIN_JUMP || name == EN_COIN_FLOOR || name == EN_COIN_VOLT || name == EN_COIN_WIND
		|| name == EN_BLUE_COIN || name == EN_COIN_WATER || name == EN_REDCOIN || name == EN_GREENCOIN
		|| name == EN_JUMPDAI || name == EN_ITEM)
		{ return; }
    else {
        spawnPakkun();
        this->Delete(1);
    }
	dEn_c::spriteCollision(apThis, apOther);
}

void pakkunTane::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) { // just damage the stupid player
    this->dEn_c::playerCollision(apThis, apOther);
    this->_vf220(apOther->owner);
}

void pakkunTane::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);

    if (hitType == 1 || hitType == 2) {	// regular jump or mini mario
		this->_vf278(apOther->owner);
		doStateChange(&StateID_DieSmoke);
	}
	else if (hitType == 3) { // spin jump (isn't possible on yoshi so damage Realismario)
		this->dEn_c::playerCollision(apThis, apOther);
        this->_vf220(apOther->owner);
	}
	else if (hitType == 0) { // sides
		this->dEn_c::playerCollision(apThis, apOther);
		this->_vf220(apOther->owner);
	}
}

bool pakkunTane::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
    this->dEn_c::playerCollision(apThis, apOther);
    this->_vf220(apOther->owner);
    return true;
}
bool pakkunTane::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool pakkunTane::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool pakkunTane::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &(S16Vec){0, 0, 0}, &(Vec){1.0, 1.0, 1.0});
	dEn_c::_vf148();
	return true;
}
bool pakkunTane::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool pakkunTane::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool pakkunTane::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat9_RollingObject(apThis, apOther);
}
bool pakkunTane::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	dActorMng_c::instance->spawnCoinJump(pos, 0, 2, 0);
	return this->collisionCat9_RollingObject(apThis, apOther);
}

bool pakkunTane::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	bool FreelyWut = dEn_c::collisionCat3_StarPower(apThis, apOther);
	return FreelyWut;
}

bool pakkunTane::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool pakkunTane::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	dActorMng_c::instance->spawnCoinJump(pos, 0, 1, 0);
	return this->collisionCat9_RollingObject(apThis, apOther);
	return true;
}

// void pakkunTane::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {

// 	dEn_C::collisionCat2_IceBall_15_YoshiIce(apThis, apOther);
// }

// These handle the ice crap
void pakkunTane::_vf148() {
	dEn_c::_vf148();
}
void pakkunTane::_vf14C() {
	dEn_c::_vf14C();
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

bool pakkunTane::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.25, 1.25, 1.25}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
	__destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
	this->animationChr.setUpdateRate(0.0f);
	this->animationPat.setUpdateRateForEntry(2.0f, 0);
	return true;
}

void pakkunTane::spawnPakkun() {

//WIP and a broken.
dStageActor_c::create(ProfileId::EN_JIMEN_PAKKUN, 9, &this->pos, &this->rot, this->currentLayerID);
OSReport("Successfully created a Pakkun (from a pakkun seed)\n");

}
