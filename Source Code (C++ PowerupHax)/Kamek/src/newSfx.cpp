#include <game.hpp>
#include <sfx.hpp>
#include "music.hpp"
#include "fileload.hpp"

const char* SFXNameList [] = {
	"original",				// 1999, DON'T USE THIS ONE
	"smasnugs",				// 2000, Used for the Samsung Galaxy S24 Ultra™ block
	"diebitch",				// 2001, "You must die!!" sound effect, used in the evil red switch things
	"spike_trap",			// 2002, Spike trap
	"doomsday",				// 2003, Used in the bullet hell section of 1-2. Credits to NamiOkami (https://www.youtube.com/watch?v=kK5oKljPNFQ) for the restoration
	"yeowch",				// 2004, Used when killing whatever I give this sound to
	"morse",				// 2005, Single morse code message for the pit of 100 tricky trials
	"ButtonPress",			// 2006, asu chan
	"PageFlip",				// 2007, asu chan
	"FNFly",				// 2008, beboobop or something i don't fucking know LEAVE ME ALONE GET OUT OF MY SKIN GET OUT OF MY SKIN GET OUT OF MY SKIN
	"DS_back",				// 2009, NSMBDS menu: cancelling
	"DS_cursor",			// 2010, NSMBDS menu: cursor
	"DS_decide",			// 2011, NSMBDS menu: selecting button
	"DS_pause",				// 2012, NSMBDS menu: pausing
	"Discord",				// 2013, Discord notification
	"MedallionPickup",		// 2014, https://youtu.be/ByMKJ_3bCgs?si=VJh0BlHuPc5DhXn6&t=60
	"SE_EMY_KABOCHAN_KILL",	// 2015, splunkin
	"SE_EMY_KABOCHAN_BREAK",// 2016, splunkin
	"TacOvershieldBreak",	// 2017, unused fornite sound
	"You_got_mail",			// 2018, You've got mail
	"SE_PLY_CHANGE_LMMY",	// 2019, sound effect for collecting lemmy suit
	NULL
};

int currentSFX = -1;
u32 *currentPtr = 0;

extern void loadFileAtIndex(u32 *filePtr, u32 fileLength, u32* whereToPatch);

// static FileHandle handle;
extern u32* GetCurrentPC();


extern "C" u32 NewSFXTable[];
extern "C" u32 NewSFXIndexes;

void loadAllSFXs() {
	u32 currentIdx = (u32)&NewSFXIndexes;

	for(int sfxIndex = 0; sfxIndex < (sizeof(SFXNameList) - 1) / sizeof(SFXNameList[0]); sfxIndex++) {
		FileHandle handle;

		char nameWithSound[80] = "";
		snprintf(nameWithSound, 79, "/Sound/stream/sfx/%s.rwav", SFXNameList[sfxIndex]);

		u32 filePtr = (u32)LoadFile(&handle, nameWithSound);

		NewSFXTable[sfxIndex] = currentIdx;
		loadFileAtIndex((u32*)filePtr, handle.length, (u32*)currentIdx);
		currentIdx += handle.length;
		currentIdx += (currentIdx % 0x10);
		FreeFile(&handle);
	}
}

int hijackSFX(int SFXNum) {
	int nameIndex = SFXNum - 1999;
	if(currentSFX == nameIndex) {
		return 189;
	}

	currentPtr = (u32*)NewSFXTable[nameIndex];
	currentSFX = nameIndex;

	return 189;
}

static nw4r::snd::StrmSoundHandle yoshiHandle;

void fuckingYoshiStuff() {
	PlaySoundWithFunctionB4(SoundRelatedClass, &yoshiHandle, 189, 1);
}
