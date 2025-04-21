#ifndef TRY_AGAIN_H
#define TRY_AGAIN_H

#include <game.hpp>
#include <sfx.hpp>
#include <profile.hpp>
#include <stage.hpp>

extern char CurrentWorld;
extern char worldMapLevelNum;

class dScTryAgain_c : public dActor_c {
public:
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dScTryAgain_c> state;

    dScTryAgain_c() : state(this, &StateID_InitWait) { }
    
    int onCreate();
    int onExecute();
    int onDraw();
    int onDelete();

    bool loadLayout();
    bool layoutLoaded;
    bool visible;

    void displayScreen();

    // restart is 0, world map is 1
    int currentButton;

    nw4r::lyt::Picture *P_buttonOff_00, *P_buttonOff_01;

    static dScTryAgain_c *instance;
    static dActor_c *build();

    USING_STATES(dScTryAgain_c);
    DECLARE_STATE(InitWait);
    DECLARE_STATE(OpenAnimeEndWait);
    DECLARE_STATE(KeyWait);
    DECLARE_STATE(OutScreenEndWait);
};

#endif