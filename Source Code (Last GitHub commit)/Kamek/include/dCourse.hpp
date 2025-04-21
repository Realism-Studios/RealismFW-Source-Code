#ifndef __DCOURSE_H
#define __DCOURSE_H

// Proper headers for Course, not the old shit

//////////////////////////////////////////////////////
// Entrances
//////////////////////////////////////////////////////

enum EntranceTypes {
	ET_Normal = 0,
	ET_Normal_1,
	ET_Door_Exit,
	ET_Pipe_Up,
	ET_Pipe_Down,
	ET_Pipe_Left,
	ET_Pipe_Right,
	ET_Falling,
	ET_Ground_Pound,
	ET_Sliding,
	ET_Swimming,
	ET_Normal_B,
	ET_Normal_C,
	ET_Normal_D,
	ET_Unk_E,
	ET_Unk_F,
	ET_Mini_Pipe_Up,
	ET_Mini_Pipe_Down,
	ET_Mini_Pipe_Left,
	ET_Mini_Pipe_Right,
	ET_Jump_Right,
	ET_Vine,
	ET_Pipe_Forward,
	ET_Boss_Standing,
	ET_Jump_Left,
	ET_Boss_Jump_Right,
	ET_Boss_Fall_Right,
	ET_Door_Enter,
	ET_Unk_1C,
	ET_Unk_1D
};

// "direct pipe" refers to connected pipes
enum EntranceFlags {
	DIRECT_PIPE_END = 0x1,
	UNKNOWN_2 = 0x2,
	IS_FORWARD_PIPE = 0x4,
	IS_DIRECT_PIPE = 0x8,
	SPAWN_HALF_TILE_LEFT = 0x40,
	NON_ENTERABLE = 0x80,
};

class dNext_c {
public:
	// represents entrance data in a stage
	struct NextGoto {
		u16 x, y;
		u16 _4, _6; // could be related to camera???

		u8 id, destArea, destID;
		EntranceTypes type;
		u8 _C, zoneID, layer, directPipePathID;

		EntranceFlags flags;
		bool leaveStage;
		u8 directPipeDirection;
	};

	NextGoto m_nextGoto;
	u32 warpInfo; // 0 = Different Area; 1 = Same Area, Different Zone; 2 = Same Zone
	bool exitIsScheduled, switchingAreas, switchingScene;
	u8 _1B;
	u16 m_timer; // timer before transition, used in multiplayer when some players haven't used the entrance
	u16 _1E;
	u32 faderType;
};

// represents entrance collision block
class daNextGotoBlock_c : public dActorState_c {
public:
	float xDistToCenter, yDistToCenter;
	float xDistToEdge, yDistToEdge;

	dAcPy_c *players[4];
	u32 nyb5;

	u16 width, height, requireVine;
	u16 setPerPlayer[4], _402;

	u16 nyb6_bottom;
	u16 alsoSetPerPlayer[4];
	bool activatedByPlayer[4];

	u8 exitID, exitStage;
};


//////////////////////////////////////////////////////
// And everything comes together to make...
//////////////////////////////////////////////////////

class dCourse_c {
public:
	virtual ~dCourse_c();

	void loadFromFile(int courseID);

	struct TilesetNames {
		char name0[32];
		char name1[32];
		char name2[32];
		char name3[32];
	};

	struct AreaSettings {			
		enum AreaFlags {
			AF_WRAP_EDGES,
			// there's probably more, idk
		};
		
		u32 initialEventIDs_1;
		u32 initialEventIDs_2;
		s16 areaFlags;
		u16 timeLimit;

		bool isCredits;
		u8 _D[3];
		u8 startEntrance;

		bool isAmbush;
		u8 levelType; // 0 = NORMAL; 1 = YELLOW_TOAD_HOUSE; 2 = RED_TOAD_HOUSE; 3 = GREEN_TOAD_HOUSE
		u8 _13;
	};

	struct Bounding {
		int upper;
		int lower;
		int lakituUpper;
		int lakituLower;
		u16 id;
		u16 multiplayerScreenHeightAdjust;
		u16 multiplayerUpperAdjust;
		u16 multiplayerLowerAdjust;
	};

	struct AreaBlock3 {
		u8 unk[8];
	};

	struct BackgroundEntry {
		u16 entryID;
		u16 xScrollRate;
		u16 yScrollRate;
		u16 yPosOffset;
		u16 xPosOffset;
		u16 fileID1;
		u16 fileID2;
		u16 fileID3;
		u16 unk1;
		u16 scale;
		u32 unk2;
	};

	struct Sprite {
		u16 type;
		u16 xPos;
		u16 yPos;

		// data       nybbles:
		u16 eventIDs; // 1-4
		u32 settings; // 5-12
		u8 zoneID;    // 13/14
		u8 layerID;   // 15/16

		u16 unused;
	};

	struct LoadedSprites {
		u16 type;
		u16 unused;
	};

	struct Zone {
		u16 xPos, yPos;
		u16 width, height;

		u16 zoneTheme, terrainLighting;

		u8 zoneID;

		u8 boundingID, cameraMode, cameraZoom, zoomChange;

		u8 visibility;

		u8 bgA_ID, bgB_ID;
		u8 zoneDirection;
		u8 unk3;
		u8 music, bgmMode; // Upper 4 bits are sound modulation. If the lower 4 bits are set, the music does not begin immediately
	};

	struct dRect_c {
		s16 xPos, yPos;
		s16 width, height;
		u8 entryID;
		u8 unused[3];
	};

	struct cameraFlag_s {
		u32 unk1;
		u32 unk2;
		u32 unk3;
		u8 unk4;
		u8 scrollMode;
		u8 zoomMode;
		u8 unk5;
		u16 unk6;
		u8 eventID;
		u8 unk7;
	};

	struct rail_s {
		enum Flag {
			LOOP = 2,
		};

		u8 entryID;
		u8 unknown;
		u16 startNode;
		u16 nodeCount;
		u16 flags;
	};

	struct railNode_s {
		u16 xPos;
		u16 yPos;
		float speed;
		float accel;
		short unk1;
		short unk2;
	};


	int areaNum;


	union {
		struct {
			TilesetNames		*tilesets;
			AreaSettings		*areaSettings;
			Bounding			*bounding; // camera bounding settings
			AreaBlock3			*area3;    // unknown area settings?
			BackgroundEntry		*bgA;
			BackgroundEntry		*bgB;
			dNext_c::NextGoto	*entranceSettings;
			Sprite				*sprite;
			LoadedSprites		*loadedSprites; // might just be a u32
			Zone				*zoneSettings;
			dRect_c *rect;
			cameraFlag_s *cameraFlag;
			rail_s *rail;
			railNode_s *railNode;
		};
		void *blocks[14];
	};

	union {
		struct {
			int tilesetsSize, settingsSize, boundingSize, unkSize;
			int topBGSize, bottomBGSize, nextGotoSize, spriteSize;
			int loadSize, zoneSize, rectSize, cameraFlagSize;
			int railSize, railNodeSize;
		};
		int blockSizes[14];
	};

	union {
		struct {
			int tilesetsCount, settingsCount, boundingCount, unkCount;
			int topBGCount, bottomBGCount, nextGotoCount, spriteCount;
			int loadCount, zoneCount, rectCount, cameraFlagCount;
			int railCount, railNodeCount;
		};
		int blockCounts[14];
	};

	Sprite *zoneFirstSprite[64];
	int zoneSpriteCount[64];
	int zoneFirstSpriteIdx[64];



	Bounding *getBoundingByID(u8 id);
	BackgroundEntry *getTopBGByID(u16 id);
	BackgroundEntry *getBottomBGByID(u16 id);
	dNext_c::NextGoto *getNextGotoByID(u8 id);
	Zone *getZoneByID(u8 id, mRect *output = 0);

	u8 getZoneID(u8 id);
	u8 getBoundingIDForZone(u8 id);
	u8 getScrollForZone(u8 id);
	u8 getZoomForZone(u8 id);
	u8 getUnk10ForZone(u8 id);
	u8 getMusicForZone(u8 id);
	u8 getAudioModifierForZone(u8 id);
	u8 getVisibilityForZone(u8 id);
	u8 getTopBGIDForZone(u8 id);
	u8 getBottomBGIDForZone(u8 id);
	u16 getModelShadingForZone(u8 id);
	u16 getTerrainShadingForZone(u8 id);
	u8 getMPBiasForZone(u8 id);
	u16 getWidthForZone(u8 id);

	dRect_c *getRectByID(u8 id, mRect *output = 0);

	u8 getZoneIDContainingPosition(Vec *pos);
	u8 getZoneIDContainingRect(mRect16 *rect);

	bool doesZoneContainPosition(Vec *pos, Zone *zone);
	bool doesZoneContainRect(mRect16 *rect, Zone *zone);
};


class dCourseFull_c {
	public:
		dCourse_c courses[4];

		void loadFromFile();

		dCourse_c *get(int id) {
			if (courses[id].zoneSettings)
				return &courses[id];
			else
				return 0;
		}

		static dCourseFull_c *instance;

		static void createOnHeap(/*EGG::Heap*/void *heap);
		static void deleteInstance();

		// might not be part of this class, dunno
		static void generateLevelFilename(int world, int level, char *outBuffer);
		static char levelFilename[10];
};

#endif
