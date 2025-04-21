#include <common.hpp>
#include <game.hpp>

#define CONFIG_PATH "/debug_config.ini"

struct LaunchType {
    enum Value {
        Normal,
        ToStage,
        ToMovie,
        ToWorldMap,
        ToLevelSelect,
    };
};

struct LaunchGameMode {
    enum Value {
        Normal,
        FreePlay,
        CoinBattle,
        SuperGuideReplay,
        HintMovieReplay,
        TitleReplay,
    };
};

struct ConfigKey {
    enum KeyId {
        LaunchType,
        SaveNumber,
        PlayerCount,
        Player1ID,
        Player2ID,
        Player3ID,
        Player4ID,
        PowerUp,
        HasStar,
        World,
        Level,
        Area,
        Entrance,
        GameMode,
        HintMovieType,
        ShowPregame,
        DrawColliders,
        MovieId,
        DebugReporting,
        SpammyReporting,
        PowerupReporting,
        ReplayRecord,
        ReplaySave,
        ActorLog,
        HeapBar,
        LayoutDebug,
    };

    KeyId keyId;
    char* keyName;
};

class dConfigManager_c {
    public:
        dConfigManager_c();

        void loadConfig();
        void parseConfig(nw4r::ut::CharStrmReader* reader, void* bufferEnd);
        void parseConfigLine(char* key, char* param, int paramSize);
        void doConfigLoad();

        // Generic
        u8 launchType;
        u8 saveNo;
        u8 playerCount;
        u8 playerIDs[4];
        u8 powerUp;
        bool star;

        // Levels only
        u8 world;
        u8 level;
        u8 area;
        u8 entrance;
        u8 gameMode;
        u8 hintMovieType;
        bool showPregame;

        // Movie only
        u8 movieId;

        //debugging
        bool debugReport;
        bool spammyReport;
        bool powerupReport;
        bool collisionDebug;
        bool replayRecord;
        bool replaySave;
        bool actorLog;
        bool heapBar;
        bool layoutDebug;

        // Static instance
        static dConfigManager_c* instancePtr;
};
