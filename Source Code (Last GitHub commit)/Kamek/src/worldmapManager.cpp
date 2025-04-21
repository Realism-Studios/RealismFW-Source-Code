#include <game.hpp>
#include <common.hpp>
#include <profile.hpp>

//map manager actor, do whatever you want with it

int spawnMapManager() {
    dActor_c* worldmapManager = (dActor_c*)fBase_c::searchByProfileId(ProfileId::WM_MANAGER);
	if(!worldmapManager) {
		dActor_c::create(WM_MANAGER, 0, 0, 0);
	}
	return 0;
}

class dWMManager_c : public dActor_c {
public:
	static dActor_c* build();
	static dWMManager_c *instance;

	int onCreate();
	int onDelete();
	int onExecute();

	void createActors();

	bool actorsMade;
};

dWMManager_c *dWMManager_c::instance = 0;

const char* WMManagerNameList[] = {0};
Profile WMManagerProfile(&dWMManager_c::build, ProfileId::WM_MANAGER, NULL, ProfileId::WM_SINKSHIP, ProfileId::WM_MANAGER, "WM_MANAGER", WMManagerNameList);

dActor_c* dWMManager_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMManager_c));
	dWMManager_c *c = new(buffer) dWMManager_c;

	instance = c;
	return c;
}

int dWMManager_c::onCreate() {
	SaveFile *file = GetSaveFile();
	SaveBlock *block = file->GetBlock(file->header.current_file);
    if (block->hasWrittenNewPow == false) {
		dStockItem_c *stockItem = (dStockItem_c*)searchByProfileId(ProfileId::STOCK_ITEM);
        for (int i = 0; i < 7; i++) {
            block->new_powerups_available[i] = block->powerups_available[i];
			stockItem->newCounts[i] = block->new_powerups_available[i];
        }
        block->hasWrittenNewPow = true;
    }
// for the discord presence shit but i'll have to set this up properly some other time
	/*this->worldNumber = getLevelInfoWorldNumber(CurrentWorldNumForWorldMap, CurrentWorldNumForWorldMapSub);
	
	u8 count = 0; 
	for(int i = 0; i<4; i++) {
		if (QueryPlayerAvailability(i)) {
			count++;
		}
	}
	setDiscordPresence(count, worldNumber, false);*/

	setupGameSettings();
	//createActors();

	return true;
}

// create new map actors
void dWMManager_c::createActors() {
	if (actorsMade)
		return;

	dCourseSelectManager_c *csMgr = dCourseSelectManager_c::instance;
	dScWMap_c *map = dScWMap_c::instance;

	actorsMade = true;
}


int dWMManager_c::onDelete() {
	return true;
}


int dWMManager_c::onExecute() {
	return true;
}