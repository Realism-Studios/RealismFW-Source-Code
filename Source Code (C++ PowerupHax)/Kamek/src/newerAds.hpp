#ifndef __NEWERADS_H
#define __NEWERADS_H

#include <game.hpp>
#include <profile.hpp>

class dNewerAds_c : public dActor_c {
public:
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dNewerAds_c> state;

    dNewerAds_c() : state(this, &StateID_InitWait) { }

    int onCreate();
    int onExecute();
    int onDraw();
    int onDelete();

    void activate();

    nw4r::lyt::Pane *rootPane;

    bool layoutLoaded, visible;

    int timer;

    enum AdType {
        ScreenTopWide = 0,
        MobileAppRow,
    };

    static dNewerAds_c *instance;
    static dActor_c *build();

    USING_STATES(dNewerAds_c);
    DECLARE_STATE(InitWait);
    DECLARE_STATE(Appear);
    DECLARE_STATE(Active);
    DECLARE_STATE(Disrupt); // fullscreen ads that disrupt the game
    DECLARE_STATE(Disappear);
};

#endif