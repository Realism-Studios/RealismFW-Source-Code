#include <fileload.hpp>
#include <profile.hpp>
#include "config.hpp"

#define clamp(n, lower, upper) n <= lower ? lower : n >= upper ? upper : n

const ConfigKey keys[] = {
    {ConfigKey::LaunchType, "LaunchType"},
    {ConfigKey::SaveNumber, "SaveNumber"},
    {ConfigKey::PlayerCount, "PlayerCount"},
    {ConfigKey::Player1ID, "Player1ID"},
    {ConfigKey::Player2ID, "Player2ID"},
    {ConfigKey::Player3ID, "Player3ID"},
    {ConfigKey::Player4ID, "Player4ID"},
    {ConfigKey::PowerUp, "PowerUp"},
    {ConfigKey::HasStar, "HasStar"},
    {ConfigKey::World, "World"},
    {ConfigKey::Level, "Level"},
    {ConfigKey::Area, "Area"},
    {ConfigKey::Entrance, "Entrance"},
    {ConfigKey::GameMode, "GameMode"},
    {ConfigKey::HintMovieType, "HintMovieType"},
    {ConfigKey::ShowPregame, "ShowPregame"},
    {ConfigKey::DrawColliders, "DrawColliders"},
    {ConfigKey::MovieId, "MovieId"},
    {ConfigKey::DebugReporting, "DebugReport"},
    {ConfigKey::SpammyReporting, "SpammyReport"},
    {ConfigKey::PowerupReporting, "PowerupReport"},
    {ConfigKey::ReplayRecord, "ReplayRecord"},
    {ConfigKey::ReplaySave, "ReplaySave"},
    {ConfigKey::ActorLog, "ActorLog"},
    {ConfigKey::HeapBar, "HeapBar"},
    {ConfigKey::LayoutDebug, "LayoutDebug"},
};

static dConfigManager_c instance;
dConfigManager_c* dConfigManager_c::instancePtr = &instance;

dConfigManager_c::dConfigManager_c() {

    // Initialize the values to sane defaults
    this->launchType = LaunchType::Normal;

    this->saveNo = 0;
    this->playerCount = 0;
    this->playerIDs[0] = 0;
    this->playerIDs[1] = 1;
    this->playerIDs[2] = 3;
    this->playerIDs[3] = 2;
    this->powerUp = Powerup::None;
    this->star = false;

    this->world = 0;
    this->level = 0;
    this->area = 0;
    this->entrance = 0xFF;
    this->gameMode = LaunchGameMode::Normal;
    this->hintMovieType = MovieType::SuperSkills;
    this->showPregame = false;
    this->collisionDebug = false;
    this->debugReport = false;
    this->spammyReport = false;
    this->powerupReport = false;
    this->replayRecord = false;
    this->replaySave = false;
    this->actorLog = false;
    this->heapBar = false;
    this->layoutDebug = false;

    this->movieId = 1;
}

void dConfigManager_c::parseConfigLine(char* key, char* param, int paramSize) {

    // Initialize variables
    int decodedParam = 0;
    bool isNumeric = true;

    // If the first two characters are "0x", assume it's a hex number
    bool isHex = (strstr(param, "0x") == param);
    if (isHex) {
        param += 2;
        paramSize -= 2;
    }

    // Check if the value is numeric and convert all uppercase characters to lowercase
    for (int i = 0; i < paramSize; i++) {
        u8 currChar = param[i];
        if (currChar < '0' || currChar > '9') {
            isNumeric = false;
            if (currChar >= 'A' && currChar <= 'Z')
                param[i] = currChar - 0x20;
        }
    }

    // If the value is numeric, convert it
    if (isNumeric) {
        if (isHex)
            decodedParam = XStrToU32_((const u8*)param);
        else
            decodedParam = atoi(param);

    // If the value is not numeric, check for booleans
    } else
        decodedParam = (strcmp(param, "true") == 0);

    // Parse the key
    int keyId = -1;
    for (int i = 0; i < sizeof(keys) / sizeof(ConfigKey); i++) {
        if (strcmp(key, keys[i].keyName) == 0) {
            keyId = keys[i].keyId;
            break;
        }
    }

    // Act depending on the key we found
    switch(keyId) {
        case ConfigKey::LaunchType:
            this->launchType = clamp(decodedParam, 0, 4);
            break;

        case ConfigKey::SaveNumber:
            this->saveNo = (decodedParam + 1) & 3;
            break;

        case ConfigKey::PlayerCount:
            this->playerCount = (decodedParam - 1) & 3;
            break;

        case ConfigKey::Player1ID:
            this->playerIDs[0] = (decodedParam - 1) & 3;
            break;
        
        case ConfigKey::Player2ID:
            this->playerIDs[1] = (decodedParam - 1) & 3;
            break;
        
        case ConfigKey::Player3ID:
            this->playerIDs[2] = (decodedParam - 1) & 3;
            break;
        
        case ConfigKey::Player4ID:
            this->playerIDs[3] = (decodedParam - 1) & 3;
            break;
        
        case ConfigKey::PowerUp:
            this->powerUp = clamp(decodedParam, 0, TOTAL_POWERUP_COUNT);
            break;

        case ConfigKey::HasStar:
            this->star = decodedParam & 1;
            break;

        case ConfigKey::World:
            this->world = clamp(decodedParam - 1, 0, 99);
            break;

        case ConfigKey::Level:
            this->level = clamp(decodedParam - 1, 0, 99);
            break;

        case ConfigKey::Area:
            this->area = (decodedParam - 1) & 3;
            break;

        case ConfigKey::Entrance:
            this->entrance = (decodedParam - 1) & 0xFF;
            break;

        case ConfigKey::GameMode:
            this->gameMode = clamp(decodedParam, 0, 5);
            break;

        case ConfigKey::HintMovieType:
            this->hintMovieType = decodedParam & 3;
            break;

        case ConfigKey::ShowPregame:
            this->showPregame = decodedParam & 1;
            break;

        case ConfigKey::DrawColliders:
            this->collisionDebug = decodedParam & 1;
            break;

        case ConfigKey::MovieId:
            this->movieId = clamp(decodedParam, 1, 3);
            break;

        case ConfigKey::DebugReporting:
            this->debugReport = decodedParam & 1;
            break;

        case ConfigKey::SpammyReporting:
            this->spammyReport = decodedParam & 1;
            break;

        case ConfigKey::PowerupReporting:
            this->powerupReport = decodedParam & 1;
            break;

        case ConfigKey::ReplayRecord:
            this->replayRecord = decodedParam & 1;
            break;
        
        case ConfigKey::ReplaySave:
            this->replaySave = decodedParam & 1;
            break;
        
        case ConfigKey::ActorLog:
            this->actorLog = decodedParam & 1;
            break;

        case ConfigKey::HeapBar:
            this->heapBar = decodedParam & 1;
            break;
        
        case ConfigKey::LayoutDebug:
            this->layoutDebug = decodedParam & 1;
            break;

        default:
            break;
    }
}

void dConfigManager_c::parseConfig(nw4r::ut::CharStrmReader* reader, void* bufferEnd) {

    // Initialize variables
    bool isComment = false;
    bool isParam = false;
    char keyBuffer[64];
    char paramBuffer[64];
    int keyBufferSize = 0;
    int paramBufferSize = 0;

    // Clear the buffer
    memset(keyBuffer, 0, sizeof(keyBuffer));
    memset(paramBuffer, 0, sizeof(paramBuffer));

    // Set the file read function (needed by the hex to u32 function later)
    GetCharPtr_ = &GetCharOnMem_;

    // Read one character at a time until the file ends
    do {
        wchar_t nextChar = reader->Next();

        // Ignore all non-ASCII characters, i cannot be arsed
        if (nextChar >= 0x80)
            continue;
        u8 currChar = nextChar & 0x7F;

        // Ignore whitespace characters
        if (currChar == ' ' || currChar == '\t' || currChar == '\r')
            continue;

        // If a newline is reached, clear the variables
        if (currChar == '\n') {

            // Only parse lines with a proper key and param
            if (keyBufferSize != 0 && paramBufferSize != 0)
                this->parseConfigLine(keyBuffer, paramBuffer, paramBufferSize);

            // Reset variables
            memset(keyBuffer, 0, keyBufferSize);
            memset(paramBuffer, 0, paramBufferSize);
            keyBufferSize = 0;
            paramBufferSize = 0;
            isComment = false;
            isParam = false;

        // Ignore any comment character on the rest of this line
        } else if (!isComment) {

            // Mark characters after a # as a comment
            if (currChar == '#')
                isComment = true;

            // Mark characters after a = as the param
            else if (currChar == '=')
                isParam = true;

            // Store character in the key buffer unless it's full
            else if (!isParam) {
                if (keyBufferSize < sizeof(keyBuffer) - 1) {
                    keyBuffer[keyBufferSize] = currChar;
                    keyBufferSize++;
                }
            }

            // Store character in the param buffer unless it's full
            else if (paramBufferSize < sizeof(paramBuffer) - 1) {
                paramBuffer[paramBufferSize] = currChar;
                paramBufferSize++;
            }
        }

    } while (reader->GetCurrentPos() < bufferEnd);
}

void dConfigManager_c::loadConfig() {

    // Locate the file
    int entrynum = DVDConvertPathToEntrynum(CONFIG_PATH);
    if (entrynum == -1) {
        OSReport("Config file not found!\n");
        return;
    }

    // Try to load it
    DVDHandle dvdHandle;
    bool fileLoaded = DVDFastOpen(entrynum, &dvdHandle);
    if (!fileLoaded) {
        OSReport("Config file found, but not loaded!\n");
        return;
    }

    // Allocate the necessary space
    size_t size = OSRoundUp32B(dvdHandle.length);
    void *buffer = EGG__Heap__alloc(size, 0x20, GetArchiveHeap());
    if (buffer == NULL) {
        OSReport("Config file found and loaded, but heap creation failed!\n");
        return;
    }

    // Read the file
    s32 length = DVDReadPrio(&dvdHandle, buffer, size, 0, 2);
    if (length > 0) {

        // Initialize the stream reader, assume the file uses UTF-8
        nw4r::ut::CharStrmReader reader;
        reader.Set(buffer);
        reader.readFunc = &nw4r::ut::CharStrmReader::ReadNextCharUTF8;

        // Parse the file
        this->parseConfig(&reader, (void*)((u32)buffer + dvdHandle.length));
    }

    // Close the file, free the buffer and return
    DVDClose(&dvdHandle);
    EGG__Heap__free(buffer, NULL);
    OSReport("Config file successfully loaded!\n");
}


extern "C" int loadGameModels();

extern bool enableDebugMode;
extern bool isReplayEnabled;
//extern "C" static void SetCourseTypeForce(int world, int level, u32 type);

// Config loader hook
void dConfigManager_c::doConfigLoad() {
    // Load the config data
    instance.loadConfig();

    debugReporting = instance.debugReport;
    spammyReporting = instance.spammyReport;
    powerupReporting = instance.powerupReport;
    actorLogging = instance.actorLog;
    heapBarActive = instance.heapBar;
    layoutDebugging = instance.layoutDebug;
    enableDebugMode = instance.collisionDebug;
    isReplayEnabled = instance.replayRecord;

    // If launch type is 0, do the original call and nothing else
    u8 launchType = instance.launchType;
    if (launchType == LaunchType::Normal) {
        int rnd = GenerateRandomNumber(8);
        if (rnd == 4)
            DoSceneChange(LanguageSelect, 0, false);
        else
            StartTitleScreenStage(false, false);
        return;
    }

    // Set up the save game
    dGameCom::initGame();
    u8 saveNo = (instance.saveNo > 0) ? instance.saveNo - 1 : SaveFileInstance->header.current_file;
    SaveFileInstance->initLoadGame(saveNo);

    // Setup player variables
    if (launchType != LaunchType::Normal) {
        for (int i = 0; i <= instance.playerCount; i++) {
            Player_Active[i] = 1;
        }
    }

    if (launchType == LaunchType::ToStage) {
        for (int i = 0; i <= instance.playerCount; i++) {
            Player_ID[i] = instance.playerIDs[i];
            Player_Powerup[i] = instance.powerUp;
            Player_Flags[i] = instance.star;
        }
    }

    // Act depending on the launch type
    if (launchType == LaunchType::ToStage) {
        //SetCourseTypeForce(instance.world, instance.level, 2); levelinfo already does this?
		RESTART_CRSIN_LevelStartStruct.world1 = instance.world;
		RESTART_CRSIN_LevelStartStruct.world2 = instance.world;
		RESTART_CRSIN_LevelStartStruct.level1 = instance.level;
		RESTART_CRSIN_LevelStartStruct.level2 = instance.level;
		RESTART_CRSIN_LevelStartStruct.area = instance.area;
		RESTART_CRSIN_LevelStartStruct.entrance = instance.entrance;

        DontShowPreGame = !instance.showPregame;

        switch(instance.gameMode) {
            case LaunchGameMode::CoinBattle:
                GameFlag |= CoinBattle;

            case LaunchGameMode::FreePlay:
                GameFlag |= ExtraMode;
                break;

            case LaunchGameMode::SuperGuideReplay:
                RESTART_CRSIN_LevelStartStruct.isReplay = true;
                RESTART_CRSIN_LevelStartStruct.screenType = ScreenType::SuperGuide;
                break;

            case LaunchGameMode::HintMovieReplay:
                RESTART_CRSIN_LevelStartStruct.isReplay = true;
                RESTART_CRSIN_LevelStartStruct.screenType = ScreenType::HintMovie;
                RESTART_CRSIN_LevelStartStruct.hintMovieType = instance.hintMovieType;
                break;

            case LaunchGameMode::TitleReplay:
                RESTART_CRSIN_LevelStartStruct.isReplay = true;
                RESTART_CRSIN_LevelStartStruct.screenType = ScreenType::TitleReplay;
                break;

            default:
                break;
        }

        DoSceneChange(ProfileId::RESTART_CRSIN, 0, false);
    } else if (launchType == LaunchType::ToMovie) {
        DoSceneChange(ProfileId::MOVIE, instance.movieId, false);
    } else if (launchType == LaunchType::ToWorldMap) {
        loadGameModels();
        DoSceneChange(ProfileId::WORLD_MAP, instance.world, false);
    } else {
        loadGameModels();
        DoSceneChange(ProfileId::Koopatlas, 0, false);
    }
        
}
