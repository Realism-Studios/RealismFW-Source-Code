#include <common.hpp>
#include <game.hpp>
#include <g3dhax.hpp>
#include <sfx.hpp>
#include <stage.hpp>
#include "msgbox.hpp"


const char*  NPCArc[] = { "RYOMRes1", NULL };

class NPC : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void setupAnim(const char* name, float rate);
	void setupModel(const char* arcName, const char* brresName, const char* mdlName);

	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	int message;
	int model;
	void updateModelMatrix();

	nw4r::g3d::ResMdl mdl;
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

};

const SpriteData NPCSpriteData = { ProfileId::NPC, 8, -16, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 }; // WHAT are these numbers
Profile KonbuProfile(&NPC::build, SpriteId::NPC, &NPCSpriteData, ProfileId::NPC, ProfileId::NPC, "NPC", NPCArc, 0);

dActor_c* NPC::build() {
	void *buffer = AllocFromGameHeap1(sizeof(NPC));
	return new(buffer) NPC;
}

void NPC::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther){
	u16 name = ((dEn_c*)apOther->owner)->name;

	if (name == PLAYER) {
		int buttonPressed = Remocon_GetPressed(GetActiveRemocon());
		if (buttonPressed & WPAD_UP) {
			dMsgBoxManager_c::instance->showMessage(this->message);
		}
	}
}

void NPC::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther){
	return playerCollision(apThis, apOther);
}

void NPC::setupAnim(const char* name, float rate) {
	nw4r::g3d::ResAnmChr anmChr;

	anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.setup(this->mdl, anmChr, &this->allocator, 0);
	this->animationChr.bind(&this->bodyModel, anmChr, 1);
	this->bodyModel.bindAnim(&this->animationChr, 0.0);
	this->animationChr.setUpdateRate(rate);
}

void NPC::setupModel(const char* arcName, const char* brresName, const char* mdlName) {
	this->resFile.data = getResource(arcName, brresName);
	this->mdl = this->resFile.GetResMdl(mdlName);

	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
}

//Size modified due to my buddies not wanting to modify it.
int NPC::onCreate() {

	this->scale = (Vec) {1.0, 1.0, 1.0};
	this->model = settings >> 24 & 0xFF;		//nybbles 5-6
	this->message = settings >> 12 & 0xFFF;		//nybble 7-9

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	//GUYS NOW IT'S UP TO YOU TO PUT WHATEVER YOU WANT IN THIS SWITCH BOX!!!

	switch (this->model) {

		case 0:

			setupModel("RYOMRes1", "g3d/bre0.brres", "ballon_red");
			SetupTextures_Item(&bodyModel, 0);
			this->pos.z = -3300.0;

			setupAnim("anim00", 1.0);

			break;

		case 1:

			setupModel("RYOMRes1", "g3d/bre1.brres", "ballon_green");
			SetupTextures_Item(&bodyModel, 0);
			this->pos.z = 3300.0;

			setupAnim("anim01", 1.0);
			break;

		case 2:

			setupModel("RYOMRes1", "g3d/bre18.brres", "mario_ts");
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = -3000.0;

			setupAnim("anim02", 1.0);
			break;

	}

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = 8.0;
	HitMeBaby.yDistToEdge = 16.0;
	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0xFFC01051;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;
	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->onExecute();
	u8 facing = dSprite_c__getXDirectionOfFurthestPlayerRelativeToVEC3(this, this->pos);
	if (facing != this->direction) {
		this->direction = facing;
	}
	this->rot.y = (direction) ? 0xD800 : 0x2800;	//Make the NPC face the player

	return true;
}

int NPC::onExecute() {
	bodyModel._vf1C();
	updateModelMatrix();

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}

void NPC::updateModelMatrix() {
	// This won't work with wrap because I'm lazy.
    matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int NPC::onDelete() {
	return true;
}

int NPC::onDraw() {

	bodyModel.scheduleForDrawing();
	return true;
}
