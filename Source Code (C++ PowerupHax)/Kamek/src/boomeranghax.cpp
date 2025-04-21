#include <profile.hpp>
#include <sfx.hpp>
#include "boomeranghax.hpp"

extern u8 boomerangDoWait[4];

dActor_c *daBoomerangHax_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daBoomerangHax_c));
	return new(buffer) daBoomerangHax_c;
}

const char* EmptyBoomerangHaxList[] = {NULL};
Profile BoomerangHaxProfile(&daBoomerangHax_c::build, ProfileId::BoomerangHax, NULL, ProfileId::BoomerangHax, ProfileId::BoomerangHax, "BoomerangHax", EmptyBoomerangHaxList);

CREATE_STATE(daBoomerangHax_c, Thrown);
CREATE_STATE(daBoomerangHax_c, GotBack);


////////////////////////
// Collision Functions
////////////////////////


	void daBoomerangHax_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		u16 name = ((dEn_c*)apOther->owner)->name;

		if (name == EN_COIN || name == EN_EATCOIN || name == AC_BLOCK_COIN || name == EN_COIN_JUGEM || name == EN_COIN_ANGLE
			|| name == EN_COIN_JUMP || name == EN_COIN_FLOOR || name == EN_COIN_VOLT || name == EN_COIN_WIND
			|| name == EN_BLUE_COIN || name == EN_COIN_WATER || name == EN_REDCOIN || name == EN_GREENCOIN
			|| name == EN_JUMPDAI || name == EN_ITEM || name == EN_STAR_COIN || name == BoomerangHax || name == AC_BLOCK_GROUP) //this list will probably be updated in the future if I find other sprites that unfortunatly don't have to die
			{ return; }

		dAcPy_c *player = dAcPy_c::findByID((this->settings >> 4) & 0xF);
		this->addScoreWhenHit(player);

		if(name == EN_KANITAMA) {
			((dEn_c*)apOther->owner)->collisionCat7_GroundPound(apThis, apOther);
			return;
		}

		dEn_c::spriteCollision(apThis, apOther);
		((dEn_c*)apOther->owner)->collisionCat13_Hammer(apThis, apOther);
	}

	void daBoomerangHax_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
			if(canI) {
				doStateChange(&StateID_GotBack);
			}
		}
	}

	void daBoomerangHax_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
			if(canI) {
				doStateChange(&StateID_GotBack);
			}
		}
	}

	bool daBoomerangHax_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
		return false;
	}

	bool daBoomerangHax_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
			if(canI) {
				doStateChange(&StateID_GotBack);
			}
		}
		return false;
	}

	bool daBoomerangHax_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
			if(canI) {
				doStateChange(&StateID_GotBack);
			}
		}
		return false;
	}

	bool daBoomerangHax_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
		return false;
	}

	bool daBoomerangHax_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther){
		if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
			if(canI) {
				doStateChange(&StateID_GotBack);
			}
		}
		return false;
	}

	bool daBoomerangHax_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther){
		if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
			if(canI) {
				doStateChange(&StateID_GotBack);
			}
		}
		return false;
	}

	bool daBoomerangHax_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
		return false;
	}

	bool daBoomerangHax_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther){
		if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
			if(canI) {
				doStateChange(&StateID_GotBack);
			}
		}
		return false;
	}

	bool daBoomerangHax_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther){
		return false;
	}

	bool daBoomerangHax_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
		return false;
	}
	
	bool daBoomerangHax_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) { 
		return false;
	}

int daBoomerangHax_c::onCreate() {

	this->deleteForever = true;

	// Model creation
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("boomerang", "g3d/boomerang.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("bros_boomerang");

	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	nw4r::g3d::ResAnmTexPat resAnmTexPat = this->resFile.GetResAnmTexPat("color");
    texPat.setup(mdl, resAnmTexPat, &allocator, 0, 1);
    texPat.bindEntry(&bodyModel, &resAnmTexPat, 0, 1);

    int player = (this->settings >> 4) & 0xF;
	texPat.setFrameForEntry(player, 0);
	texPat.setUpdateRateForEntry(0.0f, 0);
    bodyModel.bindAnim(&texPat);

	allocator.unlink();

	// Stuff I do understand
	this->scale.x = 0.85f;
	this->scale.y = 0.85f;
	this->scale.z = 0.85f;

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;
	this->direction = (this->settings >> 8) & 0xF; // Heading where the player look.

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 6.375;
	HitMeBaby.yDistToEdge = 6.375;		

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xFFBA7FFE;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();


	this->pos.y += 16;
	this->pos.z = 4000;

	doStateChange(&StateID_Thrown); 

	this->onExecute();
	return true;
}

int daBoomerangHax_c::onDelete() {
	projectileCounts[Powerup::BoomerangSuit][which_player]--;
	this->Delete(1);
	return true;
}

int daBoomerangHax_c::onExecute() {
	acState.execute();
	updateModelMatrices();



	this->playBoomerangSound();
	bodyModel._vf1C();

	this->rot.z += (this->direction == 1) ? 2000 : -2000;

	return true;
}

int daBoomerangHax_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daBoomerangHax_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 2.0, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


///////////////
// Thrown State
///////////////
	void daBoomerangHax_c::beginState_Thrown() {

	}
	void daBoomerangHax_c::executeState_Thrown() { 
		HandleXSpeed();
		doSpriteMovement();
		float speedx = (this->direction == 1) ? -2.7 : 2.7;
		this->timer++;
		if(this-> timer > 0 && this->timer <= 20) {
			this->max_speed.x = speedx;
			this->x_speed_inc = 0;
			this->speed.x = speedx;
			this->pos.y += 1.5;
		}
		if(this->timer > 20 && this->timer <= 55) {
			this->speed.x = speedx;
		}
		if(this-> timer > 55 && this->timer <= 70) {
			canI = true;
			this->max_speed.x = -speedx;
			this->x_speed_inc = 0.2;
			this->pos.y -= 2;
		}
		if(this->timer > 70) {
			this->x_speed_inc = 0;
			this->max_speed.x = -speedx;
			this->speed.x = -speedx;
		}
		if(this->timer == 600) {
			this->direction = (this->direction == 1) ? 0 : 1;
			doStateChange(&StateID_GotBack);
		}
	}
	void daBoomerangHax_c::endState_Thrown() { 

	}


///////////////
// GotBack State
///////////////
	void daBoomerangHax_c::beginState_GotBack() {
		this->x_speed_inc = 0;
	}
	void daBoomerangHax_c::executeState_GotBack() { 
		HandleXSpeed();
		doSpriteMovement();
		float speedx = (this->direction == 1) ? -2.7 : 2.7;
		this->max_speed.x = -speedx;
		this->speed.x = -speedx;
		this->scale.x -= 0.1;
		this->scale.y -= 0.1;
		this->scale.z -= 0.1;
		if(this->scale.x <= 0) {
			this->Delete(1);
		}
	}
	void daBoomerangHax_c::endState_GotBack() { 

	}




/*****************************************************/
/**************Gabon Rock replacing shit**************/
/*Don't ask me how does it work, because i don't know*/
/*****************************************************/

extern "C" float pow(float num, float power);

int daGabonRock_c::getsettings() {									 //I know bleh bleh bleh that's not optimised, but lemme ask something: Do I care ? The answer is no.
	int orig_val = this->onCreate_orig();
	if(getNybbleValue(this->settings, 12, 12) > 1) {
		int playerID = getNybbleValue(this->settings, 6, 6);
		dAcPy_c *player = dAcPy_c::findByID(playerID);
		int settings = 0 | (playerID << 4) | (player->direction << 8);
		CreateActor(BoomerangHax, settings, player->pos, 0, 0);
		boomerangDoWait[playerID] = 60;
	}
	return orig_val;
}

void dGameDisplay_c::doWaitCheck() {
	for (int i = 0; i < 4; i++) {
		if (boomerangDoWait[i] > 0) {
			boomerangDoWait[i]--;
		}
	}
}