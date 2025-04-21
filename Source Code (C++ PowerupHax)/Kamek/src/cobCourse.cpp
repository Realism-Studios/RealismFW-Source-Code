#include <game.hpp>
#include "levelinfo.hpp"

extern "C" void bindAnimCobCourse(u32 thisPtr, u32 animID, float unk1, float unk2);
extern "C" u8 CurrentWorldNumForWorldMap;
extern "C" u8 CurrentWorldNumForWorldMapSub;

// chooses our name
extern "C" const char *cobCourseChooseFilename(const char *filename) {
	u32 worldNum = getLevelInfoWorldNumber(CurrentWorldNumForWorldMap, CurrentWorldNumForWorldMapSub);

	// subtract 1 from the in-game number
	switch (worldNum) {
		default: // default model
			filename = "g3d/model.brres";
			break;

		case 3: // W4, test out the triangle nodes
			filename = "g3d/triangle.brres";
			break;

		case 4: // W5, the newer world
			filename = "g3d/newer.brres";
			break;
	}

	return filename;
}

void cobCourseNewColor(u32 thisPtr, u32 animID, float unk1, float unk2) {
	uint worldNum = getLevelInfoWorldNumber(CurrentWorldNumForWorldMap, CurrentWorldNumForWorldMapSub);
	uint levelNum = *(u32*)(thisPtr + 4) & 0xFF;

	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldNum, levelNum);

	if(level) {
		if(level->flags & 0x20) {
			// OSReport("SExit: %02d-%02d\n", worldNum+1, levelNum+1);

			SaveBlock *save = GetSaveFile()->GetBlock(-1);
			u32 conds = save->GetLevelCondition(worldNum, levelNum);

			if(!((conds & COND_NORMAL) && (conds & COND_SECRET))) {
				bindAnimCobCourse(thisPtr, 3, unk1, unk2);
				return;
			}
		}
	}

	bindAnimCobCourse(thisPtr, 0, unk1, unk2);
}