#include <common.hpp>
#include <game.hpp>
#include <g3dhax.hpp>
#include <sfx.hpp>
#include <profile.hpp>


extern "C" bool SpawnEffect(const char*, int, Vec*, S16Vec*, Vec*);

const char* FakeStarCoinFileList [] = {
	"star_coin_fake",
	NULL
};

class daFakeStarCoin : public dEn_c {
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;

	u64 eventFlag;
	s32 timer;
	u32 delay;

	u32 effect;
	u8 type;

	public: static dActor_c *build();

	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);

};

const SpriteData fakeStarCoinSpriteData = { ProfileId::FakeStarCoin, 0x80, 0 , 0x80, 0x80, 0x80, 0x80, 0, 0, 0x60, 0x50, 8};
Profile fakeStarCoinProfile(&daFakeStarCoin::build, SpriteId::FakeStarCoin, &fakeStarCoinSpriteData, ProfileId::AC_LIFT_SEESAW, ProfileId::FakeStarCoin, "FakeStarCoin", FakeStarCoinFileList);

void daFakeStarCoin::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) { 

	PlaySound(this, SE_VOC_LU_ITEM_COMPLETE);
	SpawnEffect("Wm_en_obakedoor", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	
	//FIXME changed to dStageActor_c::Delete(u8) from fBase_c::Delete(void)
	this->Delete(1);
}

bool daFakeStarCoin::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daFakeStarCoin::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daFakeStarCoin::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daFakeStarCoin::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}

bool daFakeStarCoin::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) { 
	SpawnEffect("Wm_en_explosion", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	SpawnEffect("Wm_en_explosion_smk", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){3.0, 3.0, 3.0});

	PlaySound(this, SE_OBJ_EMY_FIRE_DISAPP);
	this->Delete(1);

	return true;
}
bool daFakeStarCoin::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) { 
	SpawnEffect("Wm_ob_cmnicekira", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){1.5, 1.5, 1.5});
	SpawnEffect("Wm_ob_icebreakwt", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	SpawnEffect("Wm_ob_iceattack", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){1.5, 1.5, 1.5});

	PlaySound(this, SE_OBJ_PNGN_ICE_BREAK);

	this->Delete(1);
	return true; 
}
bool daFakeStarCoin::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	SpawnEffect("Wm_ob_cmnboxgrain", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	SpawnEffect("Wm_en_obakedoor_sm", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});

	PlaySound(this, SE_BOSS_JR_FLOOR_BREAK);

	//FIXME changed to dStageActor_c::Delete(u8) from fBase_c::Delete(void)
	this->Delete(1);
	return true;
}
bool daFakeStarCoin::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daFakeStarCoin::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool daFakeStarCoin::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}



dActor_c *daFakeStarCoin::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daFakeStarCoin));
	return new(buffer) daFakeStarCoin;
}


int daFakeStarCoin::onCreate() {
	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResFile rf(getResource("star_coin_fake", "g3d/star_coin.brres"));
	bodyModel.setup(rf.GetResMdl("star_coinA"), &allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	allocator.unlink();
	
	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = -3.0;
	HitMeBaby.xDistToEdge = 12.0;
	HitMeBaby.yDistToEdge = 15.0;
	HitMeBaby.category1 = 0x5;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0xFFFFFFFF;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->scale.x = 1.0;
	this->scale.y = 1.0;
	this->scale.z = 1.0;

	this->pos.x -= 120.0;
	this->pos.z = 3300.0;
	
	this->onExecute();
	return true;
}


int daFakeStarCoin::onDelete() {
	return true;
}

int daFakeStarCoin::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daFakeStarCoin::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daFakeStarCoin::onExecute() {
	updateModelMatrices();

	this->rot.y += 0x400;
	return true;
}
