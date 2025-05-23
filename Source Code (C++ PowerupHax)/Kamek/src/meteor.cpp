#include <common.hpp>
#include <game.hpp>
#include <g3dhax.hpp>
#include <profileid.hpp>
#include <sfx.hpp>
#include "boss.hpp"
#include <profile.hpp>

class dMeteor : public dEn_c {
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	public: static dActor_c *build();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	nw4r::g3d::ResFile resFile;
	mEf::es2 effect;

	int timer;
	int spinSpeed;
	char spinDir;
	char isElectric;

	Physics MakeItRound;

	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);

	public:
		void kill();
};
const char* MEarcNameList [] = {
	"kazan_rock",
	NULL
};
const SpriteData MeteorSpriteData = {ProfileId::Meteor, 8, 0, 8, 0, 0x200, 0x200, 0x30, 0x30, 0, 0, 8};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile MeteorProfile(&dMeteor::build, SpriteId::Meteor, &MeteorSpriteData, ProfileId::EN_TARZANROPE, ProfileId::Meteor, "Meteor", MEarcNameList);

dActor_c *dMeteor::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dMeteor));
	return new(buffer) dMeteor;
}

// extern "C" dStageActor_c *GetSpecificPlayerActor(int num);
// extern "C" void *modifyPlayerPropertiesWithRollingObject(dStageActor_c *Player, float _52C);


void dMeteor::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	DamagePlayer(this, apThis, apOther);
}

void MeteorPhysicsCallback(dMeteor *self, dEn_c *other) {
	if (other->profileId == ProfileId::CustomClownShot) {
		OSReport("CANNON COLLISION");

		SpawnEffect("Wm_en_explosion", 0, &other->pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
		SpawnEffect("Wm_en_explosion_smk", 0, &other->pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
		PlaySound(other, SE_OBJ_TARU_BREAK);
		other->Delete(1);

		switch ((self->settings >> 24) & 3) {
			case 1:
				dStageActor_c::create(ProfileId::EN_HATENA_BALLOON, 0x100, &self->pos, 0, self->currentLayerID);
				break;
			case 2:
				VEC3 coinPos = {self->pos.x - 16.0f, self->pos.y, self->pos.z};
				dStageActor_c::create(ProfileId::EN_COIN, 9, &coinPos, 0, self->currentLayerID);
				break;
		}

		self->kill();
	}
}

bool dMeteor::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	DamagePlayer(this, apThis, apOther);
	return true;
}


int dMeteor::onCreate() {

	// Setup Model
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("kazan_rock", "g3d/kazan_rock.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kazan_rock");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	allocator.unlink();


	// Retrieve Scale and set it up
	float sca = (float)((this->settings >> 8) & 0xFF);
	sca = (sca/5.0) + 0.2;

	this->scale = (Vec){sca,sca,sca};

	// Other settings
	this->spinDir = this->settings & 0x1;
	this->spinSpeed = ((this->settings >> 16) & 0xFF) * 20;
	this->isElectric = (this->settings >> 4) & 0x1;


	// Setup Physics
	if (isElectric) {
		ActivePhysics::Info elec;
		elec.xDistToCenter = 0.0;
		elec.yDistToCenter = 0.0;

		elec.xDistToEdge = 13.0 * sca;
		elec.yDistToEdge = 13.0 * sca;

		elec.category1 = 0x3;
		elec.category2 = 0x0;
		elec.bitfield1 = 0x4F;
		elec.bitfield2 = 0x200;
		elec.unkShort1C = 0;
		elec.callback = &dEn_c::collisionCallback;

		this->aPhysics.initWithStruct(this, &elec);
		this->aPhysics.addToList();
	}

	MakeItRound.baseSetup(this, &MeteorPhysicsCallback, &MeteorPhysicsCallback, &MeteorPhysicsCallback, 1, 0);

	MakeItRound.x = 0.0;
	MakeItRound.y = 0.0;

	MakeItRound.diameter = 13.0 * sca;
	MakeItRound.isRound = 1;

	MakeItRound.update();

	MakeItRound.addToList();

	this->pos.z = (settings & 0x1000000) ? -2000.0f : 3458.0f;

	this->onExecute();
	return true;
}

int dMeteor::onDelete() {
	return true;
}

int dMeteor::onExecute() {

	if (spinDir == 0) 	{ rot.z -= spinSpeed; }
	else 				{ rot.z += spinSpeed; }

	MakeItRound.update();
	updateModelMatrices();

	if (isElectric) {
		effect.spawn("Wm_en_birikyu_biri", 0, &(Vec){pos.x, pos.y, pos.z+500.0}, &rot, &(Vec){scale.x*0.8, scale.y*0.8, scale.z*0.8});
		PlaySound(this, SE_EMY_BIRIKYU_SPARK);
	}

	// for (i=0; i<4; i++) {
	// 	dStageActor_c *player = GetSpecificPlayerActor(i);
	// 	modifyPlayerPropertiesWithRollingObject(player, );
	// }

	return true;
}

int dMeteor::onDraw() {

	bodyModel.scheduleForDrawing();
	bodyModel._vf1C();
	return true;
}

void dMeteor::updateModelMatrices() {
	// This won't work with wrap because I'm lazy.
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

void dMeteor::kill() {
	PlaySound(this, SE_OBJ_ROCK_LAND);
	SpawnEffect("Wm_ob_cmnboxsmoke", 0, &pos, &rot, &scale);
	SpawnEffect("Wm_ob_cmnboxgrain", 0, &pos, &rot, &scale);

	this->Delete(1);
}


