#include <game.hpp>
#include <profileid.hpp>
#include <sfx.hpp>
#include "poweruphax.hpp"
#include "powerupData.hpp"

////////////////////////////////////////
// SHELL RENDERERS
////////////////////////////////////////
dShellRenderer_c *dShellRenderer_c::build() {
	return new dShellRenderer_c;
}

dShellRenderer_c::dShellRenderer_c() { }
dShellRenderer_c::~dShellRenderer_c() { }

int dShellRenderer_c::chooseShellType() {
	int type = -1;
	switch (victim->powerup_id) {
		case 7: // hammer
			type = 0;
			break;
		case 9: // lemmy
			type = 1;
			break;
		case 11: // bomb
			type = 2;
			break;
		case 13: // boomerang
			type = 3;
			break;
	}
	return type;
}

void dShellRenderer_c::setup(dPlayerModelHandler_c *handler, int sceneID) {
	victim = (dPlayerModel_c*)handler->mdlClass;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	int type = chooseShellType();
	if (type == -1)
		type = 0;

	nw4r::g3d::ResFile rf(getResource("suit_shell", "g3d/suit.brres"));
	nw4r::g3d::ResAnmTexPat resAnmTexPat = rf.GetResAnmTexPat("color");

	// setup helmet for mario/luigi
	if (victim->player_id_2 < 2) {
		nw4r::g3d::ResMdl resMdl = rf.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		helmet.setup(resMdl, &allocator, 0x3, 1, 0);
		SetupTextures_Player(&helmet, sceneID);

		// setup helmet anim
		helmetTexPat.setup(resMdl, resAnmTexPat, &allocator, 0, 1);
		helmetTexPat.bindEntry(&helmet, &resAnmTexPat, 0, 1);

		helmetTexPat.setFrameForEntry(type, 0);
		helmetTexPat.setUpdateRateForEntry(0.0f, 0);
		helmet.bindAnim(&helmetTexPat);
	}

	nw4r::g3d::ResMdl mdl = rf.GetResMdl("shell");
	shell.setup(mdl, &allocator, 0x3, 1, 0);
	SetupTextures_Player(&shell, sceneID);

	// setup shell anim
    shellTexPat.setup(mdl, resAnmTexPat, &allocator, 0, 1);
    shellTexPat.bindEntry(&shell, &resAnmTexPat, 0, 1);

	shellTexPat.setFrameForEntry(type, 0);
	shellTexPat.setUpdateRateForEntry(0.0f, 0);
    shell.bindAnim(&shellTexPat);

	allocator.unlink();


	// get node ids
	victimModel = &victim->models[0].body;
	nw4r::g3d::ResMdl *playerResMdl = (nw4r::g3d::ResMdl*)(((u32)victimModel->scnObj) + 0xE8);

	if (victim->player_id_2 < 2) {
		nw4r::g3d::ResNode face_1 = playerResMdl->GetResNode("face_1");
		headNodeID = face_1.GetID();
	}

	nw4r::g3d::ResNode skl_root = playerResMdl->GetResNode("skl_root");
	rootNodeID = skl_root.GetID();
}

void dShellRenderer_c::draw() {
	if (victim->powerup_id < 7)
		return;

	// update
	int type = chooseShellType();

	// powerup shouldn't draw shells
	if (type == -1)
		return;

	helmetTexPat.setFrameForEntry(type, 0);
	shellTexPat.setFrameForEntry(type, 0);

	if (victim->player_id_2 < 2) {
		// Materials: 2=hair 3=hat; Modes: BACK=visible ALL=invisible
		SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
		SetCullModeForMaterial(&victim->getCurrentModel()->head, 2, GX_CULL_ALL);

		Mtx headMtx;
		victimModel->getMatrixForNode(headNodeID, &headMtx);

		helmet.setDrawMatrix(&headMtx);
		helmet.setScale(1.0f, 1.0f, 1.0f);
		helmet.calcWorld(false);

		helmet.scheduleForDrawing();
	}

	Mtx rootMtx;
	victimModel->getMatrixForNode(rootNodeID, &rootMtx);

	shell.setDrawMatrix(&rootMtx);
	shell.setScale(1.0f, 1.0f, 1.0f);
	shell.calcWorld(false);

	shell.scheduleForDrawing();
}

// add shell renderers to players
extern "C" void Player2D__ctor(da2DPlayer_c *self);
da2DPlayer_c *da2DPlayer_c::newClassInit() {
	da2DPlayer_c *self = (da2DPlayer_c*)AllocFromGameHeap1(0x388);
	if (self) {
		Player2D__ctor(self);
		self->shellRenderer = dShellRenderer_c::build();
	}
	return self;
}

extern "C" void Player2D__dtor(da2DPlayer_c *self);
void da2DPlayer_c::newDtor() {
	shellRenderer->~dShellRenderer_c();
	Player2D__dtor(this);
}

void da2DPlayer_c::loadShell() {
	loadPlayer();
	shellRenderer->setup(model, 1);
}

void da2DPlayer_c::specialDraw1() {
	if (shouldDraw) {
		model->draw();
		shellRenderer->draw();
	}
}

// AcPy
extern "C" void AcPy__ctor(dAcPy_c *self);
dAcPy_c *dAcPy_c::newClassInit() {
	dAcPy_c *self = (dAcPy_c*)AllocFromGameHeap1(0x2D0C);
	if (self) {
		AcPy__ctor(self);
		self->shellRenderer = dShellRenderer_c::build();
	}
	return self;
}

extern "C" int AcPy__dtor(dAcPy_c *self);
void dAcPy_c::newDtor() {
	shellRenderer->~dShellRenderer_c();
	AcPy__dtor(this);
}

extern "C" int AcPy__onCreate(dAcPy_c *self);
int dAcPy_c::newOnCreate() {
	AcPy__onCreate(this);
	shellRenderer->setup(&modelCls, 0);

	return true;
}

int dAcPy_c::onDraw() {
	if (isStatus(0xBC)) // "PF_HIDES_TEMPORARILY"
		clearFlag(0xBC);
	else {
		modelCls.draw();
		shellRenderer->draw();
	}

	return true;
}


////////////////////////////////////////
// PROJECTILES
////////////////////////////////////////
bool CheckCustomProjectileLimit(int player, int powerup) {
	if (powerup == Powerup::MegaMario) return false;

	// special case
	if (powerup == Powerup::BoomerangSuit)
		return boomerangDoWait[player] == 0;

	// powerup has no limit, just return true
	if (projectileLimits[powerup] == 0) return true;

	PowerupReport("Projectile is %i from player %i\n", powerup, player);
	if (projectileCounts[powerup][player] < projectileLimits[powerup])
		return true;

	return false;
}

bool CheckFireballLimit(int player, int unk);
bool CheckIceballLimit(int player, int unk);

// projectile check for spinning
void dAcPy_c::setSpinPowerup() {
	if (!isCarry()) {
		if (powerup == Powerup::FireFlower && CheckFireballLimit(which_player, 1))
			createProjectile(1);
		else if ((powerup == Powerup::IceFlower || powerup == Powerup::PenguinSuit) && CheckIceballLimit(which_player, 1))
			createProjectile(1);
		else if (powerup > 6 && CheckCustomProjectileLimit(which_player, powerup))
			createProjectile(1);
	} else {
		// carrying a player, let them throw the powerup
		dAcPy_c *carryPly = getCarryPlayer();
		if (carryPly != 0) {
			carryPly->_27CC = this->_27CC;
			setSpinPowerup();
		}
	}
}

// check if we can shoot a projectile (todo: update for other powerups)
bool dAcPy_c::projectileShootCheck() {
	if (!input.willShootProjectile())
		return false;
	
	if (isCarry()) // carrying an object
		return false;
	
	PowerupReport("Powerup ID: %i\n", powerup);
	if (powerup == Powerup::FireFlower)
		return CheckFireballLimit(which_player, 0);
	else if (powerup == Powerup::IceFlower || powerup == Powerup::PenguinSuit)
		return CheckIceballLimit(which_player, 0);
	else if (powerup > 6)
		return CheckCustomProjectileLimit(which_player, powerup);
	else
		return false;
}

#include "bubbleSuit.hpp"
#include "mandySuit.hpp"
#include <bomb_projectile.hpp>
#include "boomeranghax.hpp"

void dAcPy_c::createProjectile(int unk) {
	Mtx matrix58, matrix88;
	Vec local_94, local_ac;

	dPlayerModelBase_c *pyMdl = this->modelCls.mdlClass;
	pyMdl->getModelMatrix(&matrix58, 11);

	PSMTXTrans(matrix88, -1, 4, 4);
	PSMTXConcat(matrix58, matrix88, matrix58);
	MtxToVec(&matrix58, &local_94);

	pyMdl->getModelMatrix(&matrix58, 0);
	MtxToVec(&matrix58, &local_ac);

	float fVar3 = EggSqrt((local_94.x - local_ac.x) * (local_94.x - local_ac.x) + (local_94.z - local_ac.z) * (local_94.z - local_ac.z));
	local_94.z = pos.z;

	float local_a0 = GetLoopPosX(local_ac.x + PlBaseDirectionSpeedModifier[_27CC] * fVar3);
	local_94.x = local_a0;

	u32 param = which_player | _27CC << 4 | currentLayerID << 8 | unk << 0x10 | chainlinkMode << 0xC;

	// fire
	if (powerup == Powerup::FireFlower) {
		playPlayerSound(SE_PLY_THROW_FIRE, 0);
		CreateActor(PL_FIREBALL, param, &local_94, 0, 0);
	}

	// ice
	else if (powerup == Powerup::IceFlower || powerup == Powerup::PenguinSuit) {
		playPlayerSound(SE_PLY_THROW_ICEBALL, 0);
		CreateActor(ICEBALL, param, &local_94, 0, 0);
	}

	// hammer, todo fix this
	else if (powerup == Powerup::HammerSuit) {
		daHammer_c *obj = (daHammer_c*)create(HAMMER, 0, &local_94, 0, 0);

		obj->someActorID = characterID;
		obj->playerID = which_player + 0x100;
		obj->which_player = this->which_player;
		obj->scale.x = 1.0f;
		obj->scale.y = 1.0f;
		obj->scale.z = 1.0f;
	}

	// bubble
	else if (powerup == Powerup::BubbleFlower) {
		dSuitBubble_c *obj = (dSuitBubble_c*)create(ProfileId::BubbleSuit, which_player + 50, &local_94, 0, currentLayerID);

		obj->playerIDForLimit = characterID;
		obj->playerIDForLimit2 = which_player + 0x100;
		obj->which_player = this->which_player;
	}

	// lemmy
	else if (powerup == Powerup::LemmyFlower) {
		//playPlayerSound(SFX_DOOMSDAY, 0);
		int tex = 0, rnd = GenerateRandomNumber(10), rnd2 =  GenerateRandomNumber(20);
		if (rnd == 5) tex = 1;
		if (rnd2 == 10) tex = 2;

		u32 lemmyParam = direction << 28 | tex << 24;
		dEn_c *obj = (dEn_c*)create(EN_BOUNCE_BALL, lemmyParam, &local_94, 0, 0);

		obj->which_player = this->which_player;
	}

	// mandy
	else if (powerup == Powerup::MandyMushroom) {
		playPlayerSound(SE_DEMO_OP_JUMP_JR_0954f, 0);
		dMandyIGuess_c *obj = (dMandyIGuess_c*)create(ProfileId::MandyIGuess, which_player + 50, &local_94, 0, currentLayerID);
		
		obj->playerIDForLimit = characterID;
		obj->playerIDForLimit2 = which_player + 0x100;
		obj->which_player = this->which_player;
	}

	// bomb, todo fix this
	else if (powerup == Powerup::BombFlower) {
		daBombProjectile_c *obj = (daBombProjectile_c*)create(ProfileId::BrosBomb, param, &local_94, 0, currentLayerID);

		obj->player_owner = this;
		obj->playerBomb = true;
		obj->which_player = this->which_player;
	}

	// boomerang
	else if (powerup == Powerup::BoomerangSuit) {
		u32 boomerangParam = which_player << 4 | direction << 8;
		daBoomerangHax_c *obj = (daBoomerangHax_c*)create(ProfileId::BoomerangHax, boomerangParam, &local_94, 0, currentLayerID);

		obj->which_player = this->which_player;
	}

	projectileCounts[powerup][which_player]++;
}


////////////////////////////////////////
// BUGFIXES & HACKS
////////////////////////////////////////
void daHammer_c::deleteIfOffSceen_maybe() {
	if (isHammerOffScreen()) {
		projectileCounts[Powerup::HammerSuit][which_player]--;
		this->Delete(1);
	}
}

extern "C" int daKokoopaBall_c__onDelete(void *self);
int LemmyBallDelete(dStageActor_c *self) {
	projectileCounts[Powerup::LemmyFlower][self->which_player]--;
	return daKokoopaBall_c__onDelete(self);
}


////////////////////////////////////////
// HAMMER COLLISION HACKS
////////////////////////////////////////
void ThwompHammer(dEn_c *thwomp, ActivePhysics *apThis, ActivePhysics *apOther) {
	if (thwomp->profileId == ProfileId::EN_DOSUN)
		thwomp->dEn_c::collisionCat13_Hammer(apThis, apOther);
}
void BooHammer(dEn_c *boo, ActivePhysics *apThis, ActivePhysics *apOther) {
	if (boo->profileId == ProfileId::EN_TERESA)
		boo->dEn_c::collisionCat13_Hammer(apThis, apOther);
}
void UrchinHammer(dEn_c *urchin, ActivePhysics *apThis, ActivePhysics *apOther) { return; }
