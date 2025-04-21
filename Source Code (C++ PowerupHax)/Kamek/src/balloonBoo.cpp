#include <game.hpp>

class dMj2dHeader_c {
public:
    //u8 titleID[4];
    u8 version[2];
    u8 lastUsedFile;
    // settings idk how you wanna do it
};

class dMj2dGame_c {
public:
    bool unlockedWorlds[2]; // should probably have this be u8's since bools are the same size
    u8 levelData[128]; // not sure if you wanna do [10][42] or not
    u8 playerCoins; // [4]? idk if each player needs a coin counter, not even sure how the retail game uses them tbh
    u16 playerLives[4];
    u8 powerupCounts[16];
    u8 playerPowerupState[4];
    u8 retryCounts; // are these level death counts or continues???
    //u16 score;
    u8 currentWorld;
    u8 currentNode;
    u8 pad;
};

class daEnBalloonBoo_c : public dEn_c {
public:
    m3d::mdl_c model;
    mAllocator_c allocator;

    int onCreate();
    int onExecute();
};

int daEnBalloonBoo_c::onCreate() {
    
    return true;
}