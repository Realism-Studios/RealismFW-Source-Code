#include <game.hpp>
#include <newer.hpp>
#include "levelinfo.hpp"

// TODO: match these with our last levels
int lastLevelIDs[] = {
	-1, /*no world*/
	27, 27, 27, 27, 27, 27, 27, 25,
	10,
	24, 24, 21, 24, 3
};

// basically returns an ID for colors
// const GXColor levelColors[][4] {
// 	(GXColor){0,0,0,0},
// 	(GXColor){0,0,0,0},
// };

// GXColor getLevelColor(int colorID) {
// 	return levelColors[colorID];
// }


static const wchar_t *levelNumbers[] = {
	L"0",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L"A", //10
	L"B", //11
	L"C", //12
	L"D", //13
	L"E", //14
	L"F", //15
	L"G", //16
	L"H", //17
	L"I", //18
	L"J", //19
	L"A", //20, ?-block
	L"0", //21, ghost house
	L"/", //22, tower
	L"/" L"2", //23, tower 2
	L".", //24, castle
	L";", //25, "fortress" newer leftover that's supposed to be the tower, but it's just a blue level node
	L"=", //26, final castelo
	L"3", //27, train
	L"8", //28, airship
	L"Palace", //29, switch palacesadawddsdsadffqwewdwqefwq Ok
	L"G", //30, yow shee
	L"Ok", //31, key 1 from newer, unused here so it's Ok for now
	L"Ok", //32, key 2 from newer, unused here so it's Ok for now
	L"Ok", //33, key 3 from newer, unused here so it's Ok for now
	L"Ok", //34, key 4 from newer, unused here so it's Ok for now
	L"Ok", //35, key 5 from newer, unused here so it's Ok for now
	L"1", //36, cannon
	L"8", //37, black house
	L"3", //38, red house
	L"9", //39, green house
	L"Ok", //40, unused (hopefully this isn't insanely broken on world maps)
	L"Ok", //41, unused (hopefully this isn't insanely broken on world maps)
	L"6", //42, unused (hopefully this isn't insanely broken on world maps)
	L"6", //43, unused (hopefully this isn't insanely broken on world maps)
	L"4", //44, to the right now y'all
	L"5", //45, up arrow
	L"H", //46, boat
	L"7", //47, peach
	L"J", //48, e3 tower recreation
	L"12", //49, "12" for world 12-66
	L"66", //50, "66" for world 12-66
	L"x", //51, :grin:
	L"y", //52, :skull:
	L"z", //53, :thumbsup:
	L"-", //54, nunchuk
	L",", //55, wiimote
	L"'", //56, one button
	L"(", //57, two button
	L"T", //58, three button (Vii Remote exclusive)
	L">", //59, green coin and definitely not a toad balloon
	L"G", //60, yoshi
	L" ", //61, blank space used for the minus world
};

static const wchar_t *worldNumbers[] = {
	L"0",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L"A",
	L"B",
	L"C",
	L"D",
	L"E",
	L"F",
	L"12",
};

const wchar_t* getLevelNumber(int world, int level) {
	if ((level >= 30) && (level <= 31)) {
		SaveFile *file = GetSaveFile();
		SaveBlock *block = file->GetBlock(file->header.current_file);
		switch(block->toad_level_idx[world]) {
			case 0: //arrow
				return levelNumbers[level];
				break;
			case 4: //yellow
				return levelNumbers[28];
				break;
			case 5: //red
				return levelNumbers[27];
				break;
			default: //green
				return levelNumbers[26];
				break;
		}
	} else {
		return levelNumbers[level];
	}
}

const wchar_t* getWorldNumber(int world) {
	return worldNumbers[world];
}


void WriteAsciiToTextBox(nw4r::lyt::TextBox *tb, const char *source) {
	int i = 0;
	wchar_t buffer[1024];
	while (i < 1023 && source[i]) {
		buffer[i] = source[i];
		i++;
	}
	buffer[i] = 0;

	tb->SetString(buffer);
}


int getUnspentStarCoinCount() {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int coinsSpent = save->spentStarCoins;
	return getStarCoinCount() - coinsSpent;
}

int getStarCoinCount() {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int coinsEarned = 0;

	for (int w = 0; w < 10; w++) {
		for (int l = 0; l < 42; l++) {
			u32 conds = save->GetLevelCondition(w, l);

			if (conds & COND_COIN1) { coinsEarned++; }
			if (conds & COND_COIN2) { coinsEarned++; }
			if (conds & COND_COIN3) { coinsEarned++; }
		}
	}

	return coinsEarned;
}

int getStarCoinCountForWorld(u32 worldID) {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int coinsEarned = 0;

	dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(worldID + 1);

	for (int i = 0; i < section->levelCount; i++) {
		dLevelInfo_c::entry_s *level = &section->levels[i];
		u32 conds = save->GetLevelCondition(level->worldSlot, level->levelSlot);

		if (conds & COND_COIN1) { coinsEarned++; }
		if (conds & COND_COIN2) { coinsEarned++; }
		if (conds & COND_COIN3) { coinsEarned++; }
	}

	return coinsEarned;
}

int getTotalStarCoinCountForWorld(u32 worldID) {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int totalCoins = 0;

	dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(worldID + 1);

	for (int i = 0; i < section->levelCount; i++) {
		dLevelInfo_c::entry_s *level = &section->levels[i];
		u32 conds = save->GetLevelCondition(level->worldSlot, level->levelSlot);

		if (level->flags & 2)
			totalCoins += 3;
	}

	return totalCoins;
}
