#include <game.hpp>
#include <sfx.hpp>
#include "music.hpp"
#include "fileload.hpp"

struct HijackedStream {
    u32 stringOffset;
    u32 stringOffsetFast;
    u32 infoOffset;
    u8 originalID;
    int streamID;
};

struct Hijacker {
    HijackedStream stream[2];
    u8 currentStream;
    u8 currentCustomTheme;
};

struct Song {
    const char* name;
    bool hasYoshiDrums;
};

const Song SongNameList[] = {
    {"Overworld", true},    	// 100
    {"OverworldAlt", false},    // 101
    {"Sewer", false},        	// 102
    {"Athletic", false},    	// 103
    {"Space", false},        	// 104
    {"Beach", false},        	// 105
    {"Pumpkin", false},       	// 106
    {"Factory", false},        	// 107
    {"Rain", false},            // 108
    {"Fort", false},            // 109
    {"Kirblue", false},        	// 110
    {"Galvanized", false},    	// 111
    {"Fort2", false},        	// 112
    {"NewerBeach", false},    	// 113
    {"NewerForest", false},   	// 114
    {"Pit", false},            	// 115
    {"City", false},            // 116
    {"YoshiHouse", false},    	// 117
    {"U_Overworld", true},    	// 118
    {"Shop", false},            // 119
    {"NewerCave", false},    	// 120
    {"Credits", false},        	// 121
    {"NewerBonus", false},    	// 122
    {"U_Athletic", true},    	// 123
    {"Bonetail", false},    	// 124
    {"Furniture", false},    	// 125
    {"Rygis", false},        	// 126
    {"Lobby", false},        	// 127
    {"Singalong", false},    	// 128
    {"k_d742", false},        	// 129
    {"IsanaRemix", false},    	// 130
    {"Savannah", false},    	// 131
    {"WonderMessage", false},   // 132
    {"FiveFrontDoors", false},  // 133
    {"NewerFDS", false},        // 134
    {"Forgotten", false},       // 135
    {"Past", false},            // 136
    {"Lab", false},             // 137
    {"R7038XX", false},         // 138
    {"Tanetane", false},        // 139
    {"StrongOne", false},       // 140
    NULL
};

#define _I(offs) ((offs) - 0x212C0)

Hijacker Hijackers[2] = {
    {
        {
            {_I(0x4A8F8), _I(0x4A938), _I(0x476C4), 4, STRM_BGM_ATHLETIC},
            {_I(0x4B2E8), _I(0x4B320), _I(0x48164), 10, STRM_BGM_SHIRO}
        },
        0, 0
    },
    {
        {
            {_I(0x4A83C), _I(0x4A8B4), 0, 1, STRM_BGM_CHIJOU},
            {_I(0x4A878), _I(0x4A780), 0, 2, STRM_BGM_CHIKA},
        },
        0, 0
    }
};

extern void* SoundRelatedClass;
inline char* BrsarInfoOffset(u32 offset) {
    return (char*)(*(u32*)(((u32)SoundRelatedClass) + 0x5CC)) + offset;
}

void FixFilesize(u32 streamNameOffset);

u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID) {
    Hijacker* hj = &Hijackers[channelCount == 4 ? 1 : 0];

    if ((themeID >= 0) && hj->currentCustomTheme == themeID) {
        return hj->stream[hj->currentStream].originalID;
    }

    hj->currentStream ^= 1;
    hj->currentCustomTheme = themeID;

    HijackedStream* stream = &hj->stream[hj->currentStream];

    if (stream->infoOffset) {
        u16* thing = (u16*)(BrsarInfoOffset(stream->infoOffset) + 4);
        thing[0] = channelCount;
        thing[1] = (1 << trackCount) - 1;
    }

    snprintf(BrsarInfoOffset(stream->stringOffset), 63, "stream/%s.brstm", songName);
    snprintf(BrsarInfoOffset(stream->stringOffsetFast), 63, hasFast ? "stream/%s_F.brstm" : "stream/%s.brstm", songName);

    FixFilesize(stream->stringOffset);
    FixFilesize(stream->stringOffsetFast);

    if (wantRealStreamID) {
        *wantRealStreamID = stream->streamID;
    }

    return stream->originalID;
}

void FixFilesize(u32 streamNameOffset) {
    char* streamName = BrsarInfoOffset(streamNameOffset);
    char nameWithSound[80];
    snprintf(nameWithSound, 79, "/Sound/%s", streamName);

    s32 entryNum = DVDConvertPathToEntrynum(nameWithSound);
    if (entryNum >= 0) {
        DVDHandle info;
        if (DVDFastOpen(entryNum, &info)) {
            *(u32*)(streamName - 0x1C) = info.length;
        }
    }
}

extern "C" u8 after_course_getMusicForZone(u8 realThemeID) {
    if (realThemeID < 100) {
        return realThemeID;
    }

    int songIndex = realThemeID - (realThemeID >= 200 ? 200 : 100);
    const Song& song = SongNameList[songIndex];

    return hijackMusicWithSongName(song.name, realThemeID, true, song.hasYoshiDrums ? 4 : 2, song.hasYoshiDrums ? 2 : 1, 0);
}