#include <common.hpp>
#include <game.hpp>
#include <g3dhax.hpp>
#include <profile.hpp>

// RYOMRes shit

const char* RYOMRes1ArcNameList [] = { "RYOMRes1", NULL };
const char* RYOMRes2ArcNameList [] = { "RYOMRes2", NULL };
const char* RYOMRes3ArcNameList [] = { "RYOMRes3", NULL };
const char* RYOMRes4ArcNameList [] = { "RYOMRes4", NULL };
const char* RYOMRes5ArcNameList [] = { "RYOMRes5", NULL };
const char* RYOMRes6ArcNameList [] = { "RYOMRes6", NULL };
const char* RYOMRes7ArcNameList [] = { "RYOMRes7", NULL };

class dRYOMRes_c : public dEn_c {
public:
	static dActor_c* build();
	mHeapAllocator_c allocator;
};

dActor_c* dRYOMRes_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dRYOMRes_c));
	return new(buffer) dRYOMRes_c;
}

const SpriteData RYOMRes1SpriteData = 
{ ProfileId::RYOMRes1, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes1Profile(&dRYOMRes_c::build, SpriteId::RYOMRes1, &RYOMRes1SpriteData, ProfileId::RYOMRes1, ProfileId::RYOMRes1, "RYOMRes1", RYOMRes1ArcNameList);

const SpriteData RYOMRes2SpriteData = 
{ ProfileId::RYOMRes2, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes2Profile(&dRYOMRes_c::build, SpriteId::RYOMRes2, &RYOMRes2SpriteData, ProfileId::RYOMRes2, ProfileId::RYOMRes2, "RYOMRes2", RYOMRes2ArcNameList);

const SpriteData RYOMRes3SpriteData = 
{ ProfileId::RYOMRes3, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes3Profile(&dRYOMRes_c::build, SpriteId::RYOMRes3, &RYOMRes3SpriteData, ProfileId::RYOMRes3, ProfileId::RYOMRes3, "RYOMRes3", RYOMRes3ArcNameList);

const SpriteData RYOMRes4SpriteData = 
{ ProfileId::RYOMRes4, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes4Profile(&dRYOMRes_c::build, SpriteId::RYOMRes4, &RYOMRes4SpriteData, ProfileId::RYOMRes4, ProfileId::RYOMRes4, "RYOMRes4", RYOMRes4ArcNameList);

const SpriteData RYOMRes5SpriteData = 
{ ProfileId::RYOMRes5, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes5Profile(&dRYOMRes_c::build, SpriteId::RYOMRes5, &RYOMRes5SpriteData, ProfileId::RYOMRes5, ProfileId::RYOMRes5, "RYOMRes5", RYOMRes5ArcNameList);

const SpriteData RYOMRes6SpriteData = 
{ ProfileId::RYOMRes6, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes6Profile(&dRYOMRes_c::build, SpriteId::RYOMRes6, &RYOMRes6SpriteData, ProfileId::RYOMRes6, ProfileId::RYOMRes6, "RYOMRes6", RYOMRes6ArcNameList);

const SpriteData RYOMRes7SpriteData = 
{ ProfileId::RYOMRes7, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes7Profile(&dRYOMRes_c::build, SpriteId::RYOMRes7, &RYOMRes7SpriteData, ProfileId::RYOMRes7, ProfileId::RYOMRes7, "RYOMRes7", RYOMRes7ArcNameList);

// actual RYOM sprite

const char *RYOMArcNameList[] = {
	NULL
};

//////////////////////////////////////////////////////////
//
//	How it works:
//
//		1) Skip down to line 70 - read the comments along the way if you like
//		2) Change the stuff inside " " to be what you want.
//		3) Copy paste an entire 'case' section of code, and change the number to change the setting it uses
//		4) give it back to Tempus to compile in
//



// This is the class allocator, you don't need to touch this
class dMakeYourOwn : public dEn_c {
	// Let's give ourselves a few functions
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	public: static dActor_c *build();

	// And a model and an anmChr
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	nw4r::g3d::ResFile resFile;
	m3d::anmChr_c chrAnimation;

	nw4r::g3d::ResMdl mdl;

	// Some variables to use
	int model;
	bool isAnimating;
	float size;
	float zOrder;
	bool customZ;

	void setupAnim(const char* name, float rate);
	void setupModel(const char* arcName, const char* brresName, const char* mdlName);
};
const SpriteData RYOMSpriteData = {ProfileId::RYOM, 0, 0, 0xFFFFFC00 ,0xFFFFFC00 ,0x400 ,0x400, 0, 0, 0, 0, 8};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile RYOMProfile(&dMakeYourOwn::build, SpriteId::RYOM, &RYOMSpriteData, ProfileId::WM_KOOPA_CASTLE, ProfileId::RYOM, "RYOMLoader", RYOMArcNameList);

// This sets up how much space we have in memory
dActor_c *dMakeYourOwn::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dMakeYourOwn));
	return new(buffer) dMakeYourOwn;
}


// Saves space when we do it like this
void dMakeYourOwn::setupAnim(const char* name, float rate) {
	if (isAnimating) {
		nw4r::g3d::ResAnmChr anmChr;

		anmChr = this->resFile.GetResAnmChr(name);
		this->chrAnimation.setup(this->mdl, anmChr, &this->allocator, 0);
		this->chrAnimation.bind(&this->bodyModel, anmChr, 1);
		this->bodyModel.bindAnim(&this->chrAnimation, 0.0);
		this->chrAnimation.setUpdateRate(rate);
	}
}

void dMakeYourOwn::setupModel(const char* arcName, const char* brresName, const char* mdlName) {
	this->resFile.data = getResource(arcName, brresName);
	this->mdl = this->resFile.GetResMdl(mdlName);

	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
}


// This gets run when the sprite spawns!
int dMakeYourOwn::onCreate() {

	// Settings for your sprite!

	this->model = this->settings & 0xFF; 						// Sets nubble 12 to choose the model you want
	this->isAnimating = this->settings & 0x100;					// Sets nybble 11 to a checkbox for whether or not the model has an anmChr to use
	this->size = (float)((this->settings >> 24) & 0xFF) / 4.0; 	// Sets nybbles 5-6 to size. Size equals value / 4.


	float zLevels[16] = {-6500.0, -5000.0, -4500.0, -2000.0, 
						 -1000.0, 300.0, 800.0, 1600.0, 
						  2000.0, 3600.0, 4000.0, 4500.0, 
						  6000.0, 6500.0, 7000.0, 7500.0 };

	this->zOrder = zLevels[(this->settings >> 16) & 0xF];

	this->customZ = (((this->settings >> 16) & 0xF) != 0);

	// Setup the models inside an allocator
	allocator.link(-1, GameHeaps[0], 0, 0x20);


	// Makes the code shorter and clearer to put these up here

	// A switch case, add extra models in here
	switch (this->model) {

		// TITLESCREEN STUFF
		// DEFAULT 

		case 0:		//Red ballon, bobs

			setupModel("RYOMRes1", "g3d/bre0.brres", "ballon_red"); 
			SetupTextures_Item(&bodyModel, 0);
			this->pos.z = -3300.0;

			setupAnim("anim00", 1.0); 

			break;	

		case 1:		//Green ballon, bobs

			setupModel("RYOMRes1", "g3d/bre1.brres", "ballon_green"); 
			SetupTextures_Item(&bodyModel, 0);
			this->pos.z = 3300.0;

			setupAnim("anim01", 1.0); 
			break;	
			
		case 2:		// Mario, using "wait" with mouth open

			setupModel("RYOMRes1", "g3d/bre2.brres", "mario_ts"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = -3000.0;

			setupAnim("anim02", 1.0); 
			break;	
						
		case 3:		// Peach, custom anim, bobs

			setupModel("RYOMRes1", "g3d/bre3.brres", "peach_ts"); 
			SetupTextures_Enemy(&bodyModel, 0);
			this->pos.z = -3000.0;

			setupAnim("anim03", 1.0); 
			break;	

		case 4:		// Luigi with mouth open using "wait", bobs

			setupModel("RYOMRes1", "g3d/bre4.brres", "luigi_ts"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim04", 1.0); 
			break;	
			
		case 5:	 // Yellow Toad with mouth open, does wait, bobs

			setupModel("RYOMRes1", "g3d/bre5.brres", "toady_ts"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim05", 1.0); 
			break;	

		case 6:		// Blue Toad with mouth open, bobs head and himself

			setupModel("RYOMRes1", "g3d/bre6.brres", "toadb_ts"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim06", 1.0); 
			break;	
			
		// BOWSER BEAT TS
		
		case 7:		// Mario's clowncar, bobs, animates propeller

			setupModel("RYOMRes2", "g3d/bre7.brres", "clowncar_mario"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 3300.0;

			setupAnim("anim07", 1.0); 
			break;	
			
		case 8:		// Weegee clowncar, bobs, animates propeller, spins

			setupModel("RYOMRes2", "g3d/bre8.brres", "clowncar_luigi"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 3300.0;

			setupAnim("anim08", 2.0); 
			break;	
			
		case 9:		// Toad Yellow clowncar, bobs, animates propeller

			setupModel("RYOMRes2", "g3d/bre9.brres", "clowncar_toady"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 3300.0;

			setupAnim("anim09", 1.0); 
			break;	
			
		case 10:	// Toad Blue, bobs, animates propeller

			setupModel("RYOMRes2", "g3d/bre10.brres", "clowncar_toadb"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 3300.0;

			setupAnim("anim10", 1.0); 
			break;	
		
		case 11:	// Peach clowncar, bobs, animates propeller

			setupModel("RYOMRes2", "g3d/bre11.brres", "clowncar_peach"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 3300.0;

			setupAnim("anim11", 1.0); 
			break;	
	
		case 12:	// Mario in a clowncar, bobbing, with fist outstretched.
		
			setupModel("RYOMRes2", "g3d/bre12.brres", "mario_end"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim12", 1.0); 
			break;	
			
		case 13:	// Weegee failing

			setupModel("RYOMRes2", "g3d/bre13.brres", "luigi_end"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim13", 2.0); 
			break;	
			
		case 14:		// Toad Yellow, bobs head, bobs

			setupModel("RYOMRes2", "g3d/bre14.brres", "toady_end"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim14", 1.0);
			break;	
			
		case 15:		// Blue Toad, bobs head, bobs

			setupModel("RYOMRes2", "g3d/bre15.brres", "toadb_end"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim15", 1.0); 
			break;	
			
		case 16:		// Peach laughing, bobbing

			setupModel("RYOMRes2", "g3d/bre16.brres", "peach_end"); 
			SetupTextures_Enemy(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim16", 1.0); 
			break;	
			
	//PERFECT FILE TS
			
		case 17:		// This is the peach castle backdrop

			setupModel("RYOMRes1", "g3d/bre17.brres", "ground_perfect"); 
			SetupTextures_Map(&bodyModel, 0);
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 3300.0;

			setupAnim("anim17", 1.0); 
			break;	
			
		case 18:		// Mario very small, looking up.

			setupModel("RYOMRes1", "g3d/bre18.brres", "mario_perfect"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3300.0;

			setupAnim("anim18", 1.0);
			break;	
			
		case 19:		// Weegee very small, looking up.

			setupModel("RYOMRes1", "g3d/bre19.brres", "luigi_perfect"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim19", 1.0); 
			break;	
			
		case 20:		// Yellow Toad, very small, looking up.

			setupModel("RYOMRes1", "g3d/bre16.brres", "toady_perfect"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim20", 1.0); 
			break;	
			
		case 21:		// Blue Toad, very small, looking up.

			setupModel("RYOMRes1", "g3d/bre16.brres", "toadb_perfect"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim21", 1.0); 
			break;	
			
		case 22:		// I don't think this is used, actually :|

			setupModel("RYOMRes1", "g3d/bre22.brres", "peach_perfect"); 
			SetupTextures_Enemy(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim22", 1.0); 
			break;	
			
		case 23:		// I don't think this is used, actually :|

			setupModel("RYOMRes1", "g3d/bre23.brres", "backdrop"); 
			SetupTextures_Map(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("anim23", 1.0); 
			break;	
			
	// A level thing
			
		case 24:		// Small cloud, bobs up and down

			setupModel("RYOMRes1", "g3d/bre24.brres", "cloud"); 
			SetupTextures_Item(&bodyModel, 0);
			this->pos.z = -3300.0;

			setupAnim("anim24", 1.0); 

			break;	
	
	// Here begins the ending crap 
	
		case 25:		// Ship fallen, with broken propellers and cannons.

			setupModel("RYOMRes7", "g3d/ShipFallen.brres", "KoopaShip"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("animation", 1.0);

			break;	
			
		case 26:		// A tree. From the ghost bg.

			setupModel("RYOMRes6", "g3d/tree_end.brres", "tree"); 
			SetupTextures_Map(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("animation", 1.0); 
			
			break;	
			
		case 27:		// Bowser, laying down, eyes closed. Medic? Medic!

			setupModel("RYOMRes7", "g3d/bowser_dead.brres", "koopa"); 
			SetupTextures_Boss(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("animation", 1.0);

			break;	
		
		case 28:		// A car. The animation has it tilted slightly. It's a bit darker than usual.

			setupModel("RYOMRes6", "g3d/clown_car_end.brres", "car"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 3000.0;

			setupAnim("animation", 1.0); 

			break;	
	//CREDITS SHIT
		case 29:		// Mario's clowncar, bobs, animates propeller

			setupModel("RYOMRes3", "g3d/bre29.brres", "clowncar_mario"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim29", 1.0); 
			break;	
			
		case 30:		// Weegee clowncar, bobs, animates propeller

			setupModel("RYOMRes3", "g3d/bre30.brres", "clowncar_luigi"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim30", 1.0); 
			break;	
			
		case 31:		// Toad Yellow clowncar, bobs, animates propeller

			setupModel("RYOMRes3", "g3d/bre31.brres", "clowncar_toady"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim31", 1.0); 
			break;	
			
		case 32:	// Toad Blue, bobs, animates propeller

			setupModel("RYOMRes3", "g3d/bre32.brres", "clowncar_toadb"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim32", 1.0); 
			break;	
		
		case 33:	// Peach clowncar, bobs, animates propeller

			setupModel("RYOMRes3", "g3d/bre33.brres", "clowncar_peach"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim33", 1.0); 
			break;	
	
		case 34:	// Mario in a clowncar, bobbing, with fist outstretched.
		
			setupModel("RYOMRes3", "g3d/bre34.brres", "mario_end"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 0;

			setupAnim("anim34", 1.0); 
			break;	
			
		case 35:	// Weegee 

			setupModel("RYOMRes3", "g3d/bre35.brres", "luigi_end"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim35", 1.0); 
			break;	
			
		case 36:		// Toad Yellow, bobs head, bobs

			setupModel("RYOMRes3", "g3d/bre36.brres", "toady_end"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim36", 1.0);
			break;	
			
		case 37:		// Blue Toad, bobs head, bobs

			setupModel("RYOMRes3", "g3d/bre37.brres", "toadb_end"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim37", 1.0); 
			break;	
			
		case 38:		// Peach laughing, bobbing

			setupModel("RYOMRes3", "g3d/bre38.brres", "peach_end"); 
			SetupTextures_Enemy(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim38", 1.0); 
			break;	
			
		case 39:		// PC Backdrop again

			setupModel("CreditsBG", "g3d/dupa.brres", "ground_perfect"); 
			SetupTextures_Map(&bodyModel, 0);
			this->pos.z = 0.0;

			setupAnim("anim38", 1.0); 
			break;	

		case 40:		// RYOMRes4 Canopy

			setupModel("RYOMRes4", "g3d/canopy.brres", "canopy"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			// setupAnim("anim38", 1.0); 
			break;	

		case 41:		// RYOMRes4 Canopy

			setupModel("RYOMRes4", "g3d/canopy_1.brres", "canopy_1"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			// setupAnim("anim38", 1.0); 
			break;	

		case 42:		// RYOMRes4 Canopy

			setupModel("RYOMRes4", "g3d/canopy_2.brres", "canopy_2"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			// setupAnim("anim38", 1.0); 
			break;	
		
		case 43:		// BallonR

			setupModel("RYOMRes5", "g3d/ballon.brres", "ballon_red"); 
			SetupTextures_Item(&bodyModel, 0);
			this->pos.z = 0.0;

				setupAnim("anim", 1.0); 
			break;	
			
		case 44:		// BallonG

			setupModel("RYOMRes5", "g3d/ballon2.brres", "ballon_green"); 
			SetupTextures_Item(&bodyModel, 0);
			this->pos.z = 0.0;

				setupAnim("anim", 1.0); 
			break;	
			
		case 45:		// Luigi Opening

			setupModel("RYOMRes5", "g3d/weeg.brres", "weeg"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 0.0;

				setupAnim("anim", 1.0); 
			break;	
			
		case 46:		// Mario Opening

			setupModel("RYOMRes5", "g3d/maleo.brres", "maleo"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 0.0;

				setupAnim("anim", 1.0); 
			break;	
			
		case 47:		// ToaB

			setupModel("RYOMRes5", "g3d/todb.brres", "todb"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 0.0;

				setupAnim("anim", 1.0); 
			break;	

		case 48:		// ToaY

			setupModel("RYOMRes5", "g3d/tody.brres", "tody"); 
			SetupTextures_Player(&bodyModel, 0);
			this->pos.z = 0.0;

				setupAnim("anim", 1.0); 
			break;	

		case 49:		// RYOMRes4 Canopy

			setupModel("RYOMRes4", "g3d/canopy_3.brres", "canopy_3"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			// setupAnim("anim38", 1.0); 
			break;	
			
		case 53:		// Pink coin

			setupModel("RYOMRes4", "g3d/pinkcoin.brres", "pinkcoin"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			 setupAnim("pinkcoin", 1.0); 
			break;	
		
		case 54:		// Wonder bush

			setupModel("RYOMRes4", "g3d/bush.brres", "bush"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			 setupAnim("bush", 1.0); 
			break;	
			
		case 55:		// Wonder bush "seed" (reverse effects of Wodereses)

			setupModel("RYOMRes4", "g3d/busha.brres", "bush"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			 setupAnim("bush", 1.0); 
			break;	
		
		case 56:		// Peter Griffin's Medallion, from Fortnite

			setupModel("RYOMRes4", "g3d/medallion.brres", "medallion"); 
			SetupTextures_MapObj(&bodyModel, 0);
			this->pos.z = 0.0;

			 setupAnim("spin", 1.0); 
			break;	
	}

	allocator.unlink();

	if (size == 0.0) {	// If the person has the size nybble at zero, make it normal sized
		this->scale = (Vec){1.0,1.0,1.0};	
	}
	else {				// Else, use our size
		this->scale = (Vec){size,size,size};	
	}
		
	this->onExecute();
	return true;
}


// YOU'RE DONE, no need to do anything below here.


int dMakeYourOwn::onDelete() {
	return true;
}

int dMakeYourOwn::onExecute() {
	if (isAnimating) {
		bodyModel._vf1C();	// Advances the animation one update

		if(this->chrAnimation.isAnimationDone()) {
			this->chrAnimation.setCurrentFrame(0.0);	// Resets the animation when it's done
		}
	}

	return true;
}

int dMakeYourOwn::onDraw() {
	if (customZ) {
		matrix.translation(pos.x, pos.y, this->zOrder); }	// Set where to draw the model : -5500.0 is the official behind layer 2, while 5500.0 is in front of layer 0.
	else {
		matrix.translation(pos.x, pos.y, pos.z - 6500.0); }	// Set where to draw the model : -5500.0 is the official behind layer 2, while 5500.0 is in front of layer 0.

	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);	// Set how to rotate the drawn model 

	bodyModel.setDrawMatrix(matrix);	// Apply matrix
	bodyModel.setScale(&scale);			// Apply scale
	bodyModel.calcWorld(true);			// Do some shit

	bodyModel.scheduleForDrawing();		// Add it to the draw list for the game
	return true;
}
