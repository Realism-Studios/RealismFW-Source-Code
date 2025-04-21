#include <common.hpp>
#include <game.hpp>
#include <g3dhax.hpp>
#include <profileid.hpp>
#include <sfx.hpp>

// hammer hacks
void ThwompHammer(dEn_c *thwomp, ActivePhysics *apThis, ActivePhysics *apOther) {
	if (thwomp->profileId == ProfileId::EN_DOSUN) {
		thwomp->dEn_c::collisionCat13_Hammer(apThis, apOther);
	}
	return;
}

void BooHammer(dEn_c *boo, ActivePhysics *apThis, ActivePhysics *apOther) {
	if (boo->profileId == ProfileId::EN_TERESA) {
		boo->dEn_c::collisionCat13_Hammer(apThis, apOther);
	}
	return;
}

void UrchinHammer(dEn_c *urchin, ActivePhysics *apThis, ActivePhysics *apOther) {
	return;
}

// now begins the actual stuff
#include "poweruphax.hpp"

dHammerSuitRenderer_c *dHammerSuitRenderer_c::build() {
	return new dHammerSuitRenderer_c;
}

dHammerSuitRenderer_c::dHammerSuitRenderer_c() { }
dHammerSuitRenderer_c::~dHammerSuitRenderer_c() { }

void dHammerSuitRenderer_c::setup(dPlayerModelHandler_c *handler) {
	setup(handler, 0);
}

void dHammerSuitRenderer_c::setup(dPlayerModelHandler_c *handler, int sceneID) {
	victim = (dPlayerModel_c*)handler->mdlClass;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResFile hammer(getResource("hammerM", "g3d/suit.brres"));
	nw4r::g3d::ResFile lemmy(getResource("lemmyM", "g3d/suit.brres"));
	nw4r::g3d::ResFile bomb(getResource("bombM", "g3d/suit.brres"));
	nw4r::g3d::ResFile boomerang(getResource("boomerangM", "g3d/suit.brres"));

	if (victim->player_id_2 <= 1) {
		// Hammer
		nw4r::g3d::ResMdl rmhammer = hammer.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		hammerHelmet.setup(rmhammer, &allocator, 0, 1, 0);
		SetupTextures_Player(&hammerHelmet, sceneID);
		
		// Lemmy
		nw4r::g3d::ResMdl rmlemmy = lemmy.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		lemmyHelmet.setup(rmlemmy, &allocator, 0, 1, 0);
		SetupTextures_Player(&lemmyHelmet, sceneID);
		
		// Bomb
		nw4r::g3d::ResMdl rmbomb = bomb.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		bombHelmet.setup(rmbomb, &allocator, 0, 1, 0);
		SetupTextures_Player(&bombHelmet, sceneID);
		
		// Boomerang
		nw4r::g3d::ResMdl rmboomerang = boomerang.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		boomerangHelmet.setup(rmboomerang, &allocator, 0, 1, 0);
		SetupTextures_Player(&boomerangHelmet, sceneID);
	}

	const char *shellNames[] = {
		"shell", "shell", "shell", "shell", "shell"
	};

	// Hammer
	hammerShell.setup(hammer.GetResMdl(shellNames[victim->player_id_2]), &allocator, 0, 1, 0);
	SetupTextures_Player(&hammerShell, sceneID);
	
	// Lemmy
	lemmyShell.setup(lemmy.GetResMdl(shellNames[victim->player_id_2]), &allocator, 0, 1, 0);
	SetupTextures_Player(&lemmyShell, sceneID);
	
	// Bomb
	bombShell.setup(bomb.GetResMdl(shellNames[victim->player_id_2]), &allocator, 0, 1, 0);
	SetupTextures_Player(&bombShell, sceneID);
	
	// Boomerang
	boomerangShell.setup(boomerang.GetResMdl(shellNames[victim->player_id_2]), &allocator, 0, 1, 0);
	SetupTextures_Player(&boomerangShell, sceneID);

	allocator.unlink();


	victimModel = &victim->models[0].body;
	nw4r::g3d::ResMdl *playerResMdl =
		(nw4r::g3d::ResMdl*)(((u32)victimModel->scnObj) + 0xE8);

	//headNodeID = playerResMdl->GetResNode("player_head").GetID();
	if (victim->player_id_2 <= 1) {
		nw4r::g3d::ResNode face_1 = playerResMdl->GetResNode("face_1");
		headNodeID = face_1.GetID();
	}

	nw4r::g3d::ResNode skl_root = playerResMdl->GetResNode("skl_root");
	rootNodeID = skl_root.GetID();
}

void dHammerSuitRenderer_c::draw() {
    if (victim->powerup_id < 7)
        return;

    Mtx headMtx;
    victimModel->getMatrixForNode(headNodeID, &headMtx);

    Mtx rootMtx;
    victimModel->getMatrixForNode(rootNodeID, &rootMtx);

    switch (victim->powerup_id) {
        case 7: // Hammer Suit
            if (victim->player_id_2 <= 1) {
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 2, GX_CULL_ALL);
                hammerHelmet.setDrawMatrix(&headMtx);
                hammerHelmet.setScale(1.0f, 1.0f, 1.0f);
                hammerHelmet.calcWorld(false);
                hammerHelmet.scheduleForDrawing();
            }
            hammerShell.setDrawMatrix(&rootMtx);
            hammerShell.setScale(1.0f, 1.0f, 1.0f);
            hammerShell.calcWorld(false);
            hammerShell.scheduleForDrawing();
            break;

        case 9: // Lemmy Flower
            if (victim->player_id_2 <= 1) {
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 2, GX_CULL_ALL);
                lemmyHelmet.setDrawMatrix(&headMtx);
                lemmyHelmet.setScale(1.0f, 1.0f, 1.0f);
                lemmyHelmet.calcWorld(false);
                lemmyHelmet.scheduleForDrawing();
            }
            lemmyShell.setDrawMatrix(&rootMtx);
            lemmyShell.setScale(1.0f, 1.0f, 1.0f);
            lemmyShell.calcWorld(false);
            lemmyShell.scheduleForDrawing();
            break;

        case 11: // Bomb Suit
            if (victim->player_id_2 <= 1) {
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 2, GX_CULL_ALL);
                bombHelmet.setDrawMatrix(&headMtx);
                bombHelmet.setScale(1.0f, 1.0f, 1.0f);
                bombHelmet.calcWorld(false);
                bombHelmet.scheduleForDrawing();
            }
            bombShell.setDrawMatrix(&rootMtx);
            bombShell.setScale(1.0f, 1.0f, 1.0f);
            bombShell.calcWorld(false);
            bombShell.scheduleForDrawing();
            break;
			
		case 13: // Boomerang Suit
            if (victim->player_id_2 <= 1) {
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 2, GX_CULL_ALL);
                boomerangHelmet.setDrawMatrix(&headMtx);
                boomerangHelmet.setScale(1.0f, 1.0f, 1.0f);
                boomerangHelmet.calcWorld(false);
                boomerangHelmet.scheduleForDrawing();
            }
            boomerangShell.setDrawMatrix(&rootMtx);
            boomerangShell.setScale(1.0f, 1.0f, 1.0f);
            boomerangShell.calcWorld(false);
            boomerangShell.scheduleForDrawing();
            break;

        default: // Handle other power-up IDs
            if (victim->player_id_2 <= 1) {
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_BACK); // Revert cull mode
                SetCullModeForMaterial(&victim->getCurrentModel()->head, 2, GX_CULL_BACK); // Revert cull mode
            }
            break;
    }
}
