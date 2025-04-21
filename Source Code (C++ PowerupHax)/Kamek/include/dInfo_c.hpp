#ifndef __DINFO_C
#define __DINFO_C

#include <game.hpp>
#include <levelnumber.hpp>

// formerly GMgr8
class dCyuukan_c {
	public:
		void *vtable; // 0x80311130
		int checkpoint_id;
		Vec player_spawn_pos;
		u32 direction; // 1 is facing left
		u8 cur_world, cur_level, cur_area, cur_entrance;
		bool isKinopioInChukan;
		u8 pad[3];
		u32 star_coin_status[3]; // 4 means uncollected
		u32 some_char_id[2]; // 4 means the player isn't in the game?

		int isCyuukanStart(int related_to_some_player_ids, u8 world, u8 level);
};

// formerly GMgrA0
class AmbushData {
	public:
		u32 mapSubmodel, pathNode, reverse;
		u8 _C;
		u8 pad[3];
};

enum WM_PlayerMovementType {
	PLAYER_MOVEMENT_TYPE_STANDING = 0,
	PLAYER_MOVEMENT_TYPE_WALKING,
	PLAYER_MOVEMENT_TYPE_AIRSHIP,
	PLAYER_MOVEMENT_TYPE_CANNON
};

// formerly GameMgr
class dInfo_c {
    public:
        void *vtable; // 0x80315EA0
		u32 _4;
		dCyuukan_c cyuukan;

		u32 currentWorld, currentLevel;
		u32 currentLevelNode; // used when exiting to the world map
		u32 _48, _4C, some_world;
		LevelNumber some_level;

		u32 _58, _5C, _60, numFireworksAfterLevelClear, finishedFireworks_maybe;
		bool _6C, isSuperGuide;
		u8 _6E, _6F;

		int field_70[10];
		u8 _98, _99, _9A, _9B;

		// some world map data
		u32 lastAmbushEnemy;
		AmbushData ambushData[10][4];
		u32 m_IbaraOld[10], m_IbaraNow[10];
		WM_PlayerMovementType playerMovementType;

		u32 _374, worldMapSceneNo, _37C;

		bool switch_flag; // W3 switch flag
		u8 _381, _382, _383;

		u32 CharIDs[4];
		u8 _394, toad_level_in_world[10], _39F[10], _3A9[10], currentScreenType;

		// layout and BMG escape sequence related stuff
		int layoutWorldNum, layoutCourseNum, numberToInsertIntoThing3, numberToInsertIntoThing4;
		int numberToInsertIntoThing5, numberToInsertIntoThing6, play_number;
		u32 msgCategory, msgID;

		u8 _3D8, currentControllerType, layoutShadowFlag, _3DB;

		// used for layouts with the special scissor BMG effect
		// formerly numberToInsertIntoThing10/11
		nw4r::lyt::Pane *N_Scissor_00;
		nw4r::lyt::DrawInfo *drawInfoStruct;

		u32 _3E4, _3E8, _3EC, _3F0, _3F4, _3F8;
		u8 unk4[8];
		int field_404[1][2];

		u8 blob[0x368];
		int field_774[1][2];

		u8 blob2[0x364];

		u8 worldsUnlockedMulti[11], _AEB;
		u32 modelPlayCurrentPageID, modelPlayCurrentButtonID;
		u32 _AF4, someItemID;

		bool continueCountActive;
		u8 _AFD;

		// might be flag if fukidashi info should not show for this object
		// probably set to 1 when object is picked up
		u8 hideFukidashiForCarryableObject[4][22];

		u8 _B56, _B57, _B58, _B59, _B5A;
		bool easyPairingOpen;


		// there's more functions but they aren't really needed
		void addStockItem(int type); // 0x800BB330
		void subStockItem(int type); // 0x800BB380
		int  getStockItem(int type); // returns current item count // 0x800BB3D0
		void clsStockItem(int type); // reset stockitem values // 0x800BB410
        
        static u32 mGameFlag; // 0x8042A260
        static dInfo_c *instance; // 0x8042A25C
};

// notes:
// _5C might be start node toad house type for world map
// _64 (numFireworksAfterLevelClear) might be wrong
// _394 might be a flag to not draw effects when true
// _AF4 is checked to be > -1 when initing stage players, could be player count? code that runs if condition is true seems to make all four players small
// _AFC (continueCountActive) is also used when stockItem opens
// _AFD is set to 0 by dCourseSelectManager_c::executeState_FadeInWait()

#endif // __DINFO_C
