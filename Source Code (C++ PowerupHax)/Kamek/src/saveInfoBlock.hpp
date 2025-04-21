#ifndef SAVEINFOBLOCK_H
#define SAVEINFOBLOCK_H
#include <game.hpp>
#include <profile.hpp>
#include <sfx.hpp>

/*class dSaveInfoData_c : public dStageActor_c {
public:
    m2d::EmbedLayout_c layout;

    int onCreate();
    int onExecute();
    int onDraw();
    int onDelete();

    void show();

    // Data functions
    void getPlayerLives();

    bool layoutLoaded;
    bool visible;

    static dSaveInfoData_c *instance;
    static dActor_c *build();
};*/

// Manager sprite, creates layout functionality
class dSaveInfoManager_c : public dStageActor_c {
public:
    m2d::EmbedLayout_c layout;
    mEf::es2 effect;

    dSaveInfoManager_c() : state(this, &StateID_Hidden) { }

    //dSaveInfoData_c data;

    int onCreate();
    int onExecute();
    int onDraw();
    int onDelete();

    void showMenu();

    // Data functions
    void setupCompletionStars();
    void calculateCompletionPercentage();

    enum Animation {
        IN_WINDOW = 0,
        OUT_WINDOW,
    };

    nw4r::lyt::Picture *P_rankStarS_0[5], *P_rankStar_0[5];
    
    nw4r::lyt::TextBox
        *T_totalCompSignS_00, *T_totalCompSign_00, 
        *T_totalCompS_00, *T_totalComp_00;
    
    bool layoutLoaded;
	bool visible;

    dStateWrapper_c<dSaveInfoManager_c> state;
    
    USING_STATES(dSaveInfoManager_c);
    DECLARE_STATE(Hidden);
    DECLARE_STATE(OpenWait);
	DECLARE_STATE(Wait);
    DECLARE_STATE(HideWait);

    static dSaveInfoManager_c *instance;
    static dActor_c *build();
};


// Block sprite, creates collision and model
class dSaveInfoBlock_c : public daEnBlockMain_c {
public:
    Physics::Info physicsInfo;
    mHeapAllocator_c mAllocator;
	m3d::mdl_c model;
    m3d::anmTexSrt_c anmTexSRT;

    int onCreate();
    int onExecute();
    int onDraw();
    int onDelete();

    void updateModelMatrices();

    void calledWhenUpMoveExecutes();
	void calledWhenDownMoveExecutes();

	void blockWasHit(bool isDown);

	USING_STATES(dSaveInfoBlock_c);
	DECLARE_STATE(Wait);

    static dSaveInfoBlock_c *instance;
    static dActor_c *build();
};

#endif
