#include "koopatlas/kpPlayer.hpp"

daKPPlayer_c *daKPPlayer_c::instance = 0;

const char* KP_PlayerFileNameList[] = {0};
Profile KP_PlayerProfile(&daKPPlayer_c::build, ProfileId::KP_Player, NULL, ProfileId::WM_SINKSHIP, ProfileId::KP_Player, "KP_Player", KP_PlayerFileNameList);

dActor_c *daKPPlayer_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daKPPlayer_c));
	daKPPlayer_c *c = new(buffer) daKPPlayer_c;

	instance = c;
	return c;
}

static const char *mdlNames[][4] = {
	{"MB_model", "SMB_model", "PLMB_model", "PMB_model"},
	{"LB_model", "SLB_model", "PLLB_model", "PLB_model"},
	{"KB_model", "SKB_model", "PLKB_model", "PKB_model"},
	{"KB_model", "SKB_model", "PLKB_model", "PKB_model"}
};

static const char *patNames[][4] = {
	{"PB_switch_swim", "PB_switch_swim", "PLMB_switch_swim", "PLB_switch_swim"},
	{"PB_switch_swim", "PB_switch_swim", "PLLB_switch_swim", "PLB_switch_swim"},
	{"PB_switch_swim", "PB_switch_swim", "PLKB_switch_swim", "PLB_switch_swim"},
	{"PB2_switch_swim", "PB2_switch_swim", "PLKB2_switch_swim", "PLB2_switch_swim"}
};

int daKPPlayer_c::onCreate() {
	this->setupModel();

	this->startAnimation(0, 1.2, 10.0, 0.0);

	pos = (Vec){0.0f,0.0f,3000.0f};
	rot = (S16Vec){0x1800,0,0};
	scale = (Vec){1.6f,1.6f,1.6f};

	hasEffect = false;
	hasSound = false;
	step = false;
	effectName = "";
	soundName = 0;
	timer = 0;
	jumpOffset = 0.0;

	// -1 or 0xC000? normally we use -1 but Player uses 0xC000....
	//allocator.link(0xC000, GameHeaps[0], 0, 0x20);
	//allocator.unlink();

	return true;
}

int daKPPlayer_c::onDelete() {
	delete modelHandler;

	return true;
}


int daKPPlayer_c::onExecute() {
	if (Player_Flags[Player_ID[0]] & 1 && visible) {
		modelHandler->mdlClass->enableStarColours();
		modelHandler->mdlClass->enableStarEffects();
		dKPMusic::playStarMusic();
	} else {
		modelHandler->mdlClass->disableStarColours();
		modelHandler->mdlClass->disableStarEffects();
		dKPMusic::stopStarMusic();
	}

	if (dScKoopatlas_c::instance->hidePlayer)
		hide();
	else {
		show();
	}

	if (spinning)
		rot.y += 0xC00;
	//else
	//	SmoothRotation(&rot.y, targetRotY, 0xC00);

	if (dScKoopatlas_c::instance->mapIsRunning())
		dScKoopatlas_c::instance->pathManager.execute();

	this->modelHandler->update();
	pats[((dPlayerModel_c*)modelHandler->mdlClass)->currentPlayerModelID].process();

	mMtx myMatrix;
	myMatrix.scale(scale.x, scale.y, scale.z);
	myMatrix.applyTranslation(pos.x, pos.y + jumpOffset, pos.z);
	if (dScKoopatlas_c::instance->warpZoneHacks && (currentAnim == jump || currentAnim == jumped))
		myMatrix.applyTranslation(0, 0, 600.0f);
	myMatrix.applyRotationX(&rot.x);
	myMatrix.applyRotationY(&rot.y);
	// Z is unused for now
	modelHandler->setMatrix(myMatrix);

	if (dScKoopatlas_c::instance->mapIsRunning()) {
		if (hasEffect) { 
			Vec effPos = {pos.x, pos.y, 3300.0f};
			effect.spawn(effectName, 0, &effPos, &rot, &scale);
		}

		if (hasSound) {
			timer++;

			if (timer == 12) {
				if (step) { MapSoundPlayer(SoundRelatedClass, soundName, 1); step = false; }
				else { MapSoundPlayer(SoundRelatedClass, soundName+1, 1); step = true; }
				timer = 0;
			}

			if (timer > 12) { timer = 0; }
		}
	}

	return true;
}

int daKPPlayer_c::onDraw() {
	if (!visible)
		return true;
	if (dScKoopatlas_c::instance->isEndingScene)
		return true;

	this->modelHandler->draw();
	hammerSuit.draw();

	return true;
}



void daKPPlayer_c::hide() {
	while (scale.z > 0.01f) {
		scale.x -= 0.05f;
		scale.y -= 0.05f;
		scale.z -= 0.05f;
	}
}

void daKPPlayer_c::show() {
	// no need to grow player
	if (scale.x == 1.6f)
		return;
	
	while (scale.z < 1.6f) {
		scale.x += 0.05f;
		scale.y += 0.05f;
		scale.z += 0.05f;
	}
}


void daKPPlayer_c::setupModel() {
	int characterID = Player_ID[0];

	this->modelHandler = new dPlayerModelHandler_c(characterID);
	// loadModel(u8 player_id, int powerup_id, int unk);
	// Unk is some kind of mode: 0=in-game, 1=map, 2=2D

	// Here we do a bit of a hack
	//this->modelHandler->loadModel(0, 3, 1);
	dPlayerModel_c *pm = (dPlayerModel_c*)modelHandler->mdlClass;

	pm->mode_maybe = 1;
	pm->player_id_1 = characterID;
	pm->allocator.link(0xC000, GameHeaps[0], 0, 0x20);
	pm->prepare();

	for (int i = 0; i < 4; i++) {
		nw4r::g3d::ResMdl mdl = pm->modelResFile.GetResMdl(mdlNames[characterID][i]);
		nw4r::g3d::ResAnmTexPat pat = pm->modelResFile.GetResAnmTexPat(patNames[characterID][i]);

		pats[i].setup(mdl, pat, &pm->allocator, 0, 1);
	}

	pm->allocator.unlink();
	pm->setPowerup(Player_Powerup[characterID]);
	pm->finaliseModel();

	Vec pos = {0.0,100.0,-100.0};
	S16Vec rot = {0,0,0};
	Vec scale = {2.0,2.0,2.0};
	modelHandler->setSRT(pos, rot, scale);

	hammerSuit.setup(this->modelHandler);
}

void daKPPlayer_c::refreshPlayerModel() {
	if(this->onDelete()) {
		this->setupModel();

		OSReport("Refresh: Setting anim to %d %f %f %f\n", currentAnim, currentFrame, currentUnk, currentUpdateRate);
		this->modelHandler->mdlClass->startAnimation(currentAnim, currentFrame, currentUnk, currentUpdateRate);
	}
}

void daKPPlayer_c::startAnimation(int id, float frame, float unk, float updateRate) {
	if (id == currentAnim && frame == currentFrame && unk == currentUnk && updateRate == currentUpdateRate)
		return;

	bool isOldSwimming = (currentAnim == swim_wait);
	bool isNewSwimming = (id == swim_wait);

	currentAnim = id;
	currentFrame = frame;
	currentUnk = unk;
	currentUpdateRate = updateRate;
	this->modelHandler->mdlClass->startAnimation(id, frame, unk, updateRate);

	SpammyReport("Setting anim to %d %f %f %f\n", currentAnim, currentFrame, currentUnk, currentUpdateRate);

	if (isOldSwimming != isNewSwimming)
		bindPats();
}

void daKPPlayer_c::bindPats() {
	dPlayerModel_c *pm = (dPlayerModel_c*)modelHandler->mdlClass;
	int id = pm->currentPlayerModelID;

	static const float frames[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 3.0f};
	float frame = frames[pm->powerup_id];
	if (currentAnim == swim_wait)
		frame += (pm->powerup_id == 4 || pm->powerup_id == 5) ? 1.0f : 4.0f;

	nw4r::g3d::ResAnmTexPat pat = pm->modelResFile.GetResAnmTexPat(patNames[Player_ID[0]][id]);
	pats[id].bindEntry(
			&pm->models[id].body,
			&pat,
			0, 4);
	pats[id].setUpdateRateForEntry(0.0f, 0);
	pats[id].setFrameForEntry(frame, 0);

	pm->models[id].body.bindAnim(&pats[id]);
}

