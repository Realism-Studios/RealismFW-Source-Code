// TODO:
/* test if having an array of actors works
/* see if groundpound collision and shit is needed, maybe playerCollision will handle it?
/* obviously finish this...
/* update reggie spritedata
*/

#include <sfx.hpp>
#include <profile.hpp>
#include "mandySuit.hpp"

extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);

CREATE_STATE(dMandyIGuess_c, Throw);

const char *mandyFileList[] = {"mandy"};
const SpriteData mandySpriteData = {ProfileId::MandyIGuess, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8};
Profile mandyProfile(&dMandyIGuess_c::build, NULL, &mandySpriteData, ProfileId::MandyIGuess, ProfileId::MandyIGuess, "MandyIGuess", mandyFileList);

dActor_c* dMandyIGuess_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dMandyIGuess_c));
	return new(buffer) dMandyIGuess_c;
}


//collisions
void dMandyIGuess_c::addScoreWhenHit(void *other) {}
void dMandyIGuess_c::spawnHitEffectAtPosition(Vec2 pos) {}
void dMandyIGuess_c::doSomethingWithHardHitAndSoftHitEffects(Vec pos) {}
void dMandyIGuess_c::playEnemyDownSound2() {}
void dMandyIGuess_c::playHpdpSound1() {} // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
void dMandyIGuess_c::playEnemyDownSound1() {}
void dMandyIGuess_c::playEnemyDownComboSound(void *player) {} // AcPy_c/daPlBase_c?
void dMandyIGuess_c::playHpdpSound2() {} // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
void dMandyIGuess_c::_vf260(void *other) {} // AcPy/PlBase? plays the SE_EMY_FUMU_%d sounds based on some value
void dMandyIGuess_c::_vf264(dStageActor_c *other) {} // if other is player or yoshi, do Wm_en_hit and a few other things
void dMandyIGuess_c::_vf268(void *other) {} // AcPy/PlBase? plays the SE_EMY_DOWN_SPIN_%d sounds based on some value
void dMandyIGuess_c::_vf278(void *other) {}

// todo: sort these in order of ID
static const u16 spritesToIgnore[] = {
	EN_COIN,
	EN_EATCOIN,
	AC_BLOCK_COIN,
	EN_COIN_JUGEM,
	EN_COIN_ANGLE,
	EN_COIN_JUMP,
	EN_COIN_FLOOR,
	EN_COIN_VOLT,
	EN_COIN_WIND,
	EN_STAR_COIN_LINE,
	EN_BLUE_COIN,
	EN_COIN_WATER,
	EN_REDCOIN,
	EN_GREENCOIN,
	EN_MARUTA,
	EN_BAKUBAKU,
	EN_BIG_TOGETEKKYU,
	EN_RAIL_POLY_ICE,
	LIFT_ZEN_TOGE,
	EN_HANACHAN,
	AC_PROP_BLOCK,
	AC_PROP_BLOCK, // twice in case the game forgets about the first one
	AC_PROP_BLOCK, // thrice in case the game forgets about the second one
	AC_PROP_BLOCK, // frice in case the game forgets about the third one (https://en.wiktionary.org/wiki/frice)
	AC_PROP_BLOCK, // fivice in case the game forgets about the fourth one
	AC_LIGHT_BLOCK,
	EN_WATERSTREAM,
	EN_WIRE_TURN,
	EN_BIG_WIRE_TURN,
	ProfileId::ShyGuyGiant,
	ProfileId::ChallengeStar,
	ProfileId::FakeStarCoin,
	ProfileId::Meteor,
	ProfileId::CustomClownShot, // i don't know if this is necessary but i'm pretty sure the game will crash if the player clown cars are killed by a custom actor
};

static const u16 spritesToKillMandy[] = {
	EN_CHOROBON,
    EN_IGA_KURIBO,
    EN_UNIZOO,
    EN_SANBO,
    EN_BOSS_LARRY,
    EN_BOSS_CASTLE_LARRY,
    EN_BOSS_ROY,
    EN_BOSS_CASTLE_ROY,
    EN_BOSS_LEMMY,
    EN_BOSS_CASTLE_LEMMY,
    EN_BOSS_WENDY,
    EN_BOSS_CASTLE_WENDY,
    EN_BOSS_IGGY,
    EN_BOSS_CASTLE_IGGY,
    EN_BOSS_MORTON,
    EN_BOSS_CASTLE_MORTON,
    EN_BOSS_LUDWIG,
    EN_BOSS_CASTLE_LUDWIG,
    EN_BOSS_KAMECK,
    EN_JIMEN_PAKKUN,
    EN_JIMEN_BIG_PAKKUN,
    EN_JIMEN_FPAKKUN,
    EN_IGAPUKU,
    EN_TOGEZO,
    EN_IBARAMUSHI,
    EN_GURUGURU,
};

void dMandyIGuess_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    u16 name = ((dEn_c*)apOther->owner)->profileId;

    // mandy-on-mandy violence... what has this world come to...
    if (name == ProfileId::MandyIGuess) {
        killMandy();
        return;
    }

    // stuff that our mandy must ignore
    u16 size = sizeof(spritesToIgnore) / sizeof(spritesToIgnore[0]);
	for (u16 i = 0; i < size; i++) {
	    if (name == spritesToIgnore[i])
            return;
    }

    // stuff that'll kill our mandy
    size = sizeof(spritesToKillMandy) / sizeof(spritesToKillMandy[0]);
	for (u16 i = 0; i < size; i++) {
	    if (name == spritesToKillMandy[i]) {
            killMandy();
            return;
        }
    }

    // everything else can get killed by mandy
    dEn_c *victim = ((dEn_c*)apOther->owner);
    victim->kill();
}

void dMandyIGuess_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
    if (hitType == (1 || 3)) {
        killMandy();
		this->counter_504[apOther->owner->which_player] = 0xA;
    } else {
        if (canDie)
            killMandy();
    }
}

void dMandyIGuess_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->playerCollision(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
    killMandy();
    return true;
}
bool dMandyIGuess_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
    char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
    if(hitType == (1 || 3)) {
        killMandy();
    } else {
        if(this->canDie)
            killMandy();
    }
	return true;
}
bool dMandyIGuess_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}
bool dMandyIGuess_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat5_Mario(apThis, apOther);
}



// now the real shit begins

void dMandyIGuess_c::killMandy() {
    PlaySoundAsync(this, SE_EMY_KERONPA_BOMB);
    
    Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 2.0f, 2.0f};
	SpawnEffect("Wm_en_burst_s", 0, &efPos, &nullRot, &efScale);

    this->Delete(true);
}

void dMandyIGuess_c::chooseRandomFrame() {
    int frame = GenerateRandomNumber(7);
    anmTexPat.setFrameForEntry(frame, 0);
}

int dMandyIGuess_c::onCreate() {
    nw4r::snd::SoundHandle bubbleHandle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &bubbleHandle, SE_PLY_BALLOON_SELF, 1);

    playerID = (settings >> 24) & 0xF;
	dAcPy_c *player = dAcPy_c::findByID(playerID);
	int variation2 = (settings >> 4) & 0xF;
	
    Vec position = {player->pos.x + ((player->direction == 1) ? -16 : 16), player->pos.y + 16, 0.0f};
    this->pos = position;
    
    float randDist = (float(MakeRandomNumber(4)) / 4.0f);
	this->settings = 0 | (playerID << 4) | (player->direction << 8) | (variation2 << 12);


    allocator.link(-1, GameHeaps[0], 0, 0x20);

	// setup the model
    resFile.data = getResource("mandy", "g3d/mandy.brres");
	nw4r::g3d::ResMdl mdl = resFile.GetResMdl("mandy");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

    // and the texPat anim
    nw4r::g3d::ResAnmTexPat resAnmTexPat = this->resFile.GetResAnmTexPat("switch_mandy");
    anmTexPat.setup(mdl, resAnmTexPat, &allocator, 0, 1);
    anmTexPat.bindEntry(&bodyModel, &resAnmTexPat, 0, 1);

    // choose a random mandy to use
    chooseRandomFrame();
    anmTexPat.setUpdateRateForEntry(0.0f, 0);
    bodyModel.bindAnim(&anmTexPat);

	allocator.unlink();


	this->scale.x = 0.8; 
	this->scale.y = 0.8; 
	this->scale.z = 0.8;
	this->direction = (this->settings >> 8) & 0xF; // heads in the direction that the player looks
	this->speed.x = (this->direction == 1) ? -(3.5f + randDist) : (3.5f + randDist);
    this->speed.y = (this->direction == 1) ? -1.0f : 1.0f;
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

    doStateChange(&StateID_Throw);

	this->onExecute();
	return true;
}

void dMandyIGuess_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int dMandyIGuess_c::onExecute() {
    acState.execute();
	updateModelMatrices();
	bodyModel._vf1C();
    this->timer++;
    
    if (this->timer > 180) {
        killMandy();
	}

	return true;
}

int dMandyIGuess_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

int dMandyIGuess_c::onDelete() {
    projectileCounts[Powerup::MandyMushroom][which_player]--;
    this->Delete(1);
    return true;
}


//throwing
void dMandyIGuess_c::beginState_Throw() {
	this->scale.x = 0.0;
    this->scale.y = 0.0;
    this->scale.z = 0.0;
}
void dMandyIGuess_c::executeState_Throw() { 
	HandleXSpeed();
	doSpriteMovement();
	if (this->direction == 1) {
        if (this->speed.x > 0) {
			this->speed.x = 0;
			this->x_speed_inc = 0;
		}

        if (speed.y > 0)
            speed.y = 0;
	}
	if (this->direction == 0) {
		if (this->speed.x < 0) {
			this->speed.x = 0;
			this->x_speed_inc = 0;
		}

        if (speed.y < 0)
            speed.y = 0;
	}
	if (this->scale.x < 0.8)
		this->scale = (Vec){this->scale.x + 0.05, this->scale.y + 0.05, this->scale.z + 0.05};
    else
        canDie = true;
}
void dMandyIGuess_c::endState_Throw() {}
