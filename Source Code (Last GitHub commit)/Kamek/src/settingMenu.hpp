#ifndef _SETTINGSMENU_H
#define _SETTINGSMENU_H

#include <game.hpp>
#include <profile.hpp>
#include <sfx.hpp>
#include "fileload.hpp"

#define TOTAL_SETTING_COUNT 4

class dSettingMenu_c : public dActor_c {
public:
	m2d::EmbedLayout_c layout;
	dStateWrapper_c<dSettingMenu_c> state;

	dInfoWindow_c *infoWindow;
	
	dSettingMenu_c() : state(this, &StateID_InitWait) { }

	int onCreate();
	int beforeExecute();
	int onExecute();
	int onDelete();
	int onDraw();

	void toggleButtons();

	enum Animation {
		IN_WINDOW = 0,
        LOOP_WINDOW = 1,
        IN_BUTTON = 2, //  3,  4,  5,  6,  7,  8
        ON_BUTTON = 9, // 10, 11, 12, 13, 14, 15
        IDLE_BUTTON = 16, // 17, 18, 19, 20, 21, 22
        HIT_BUTTON = 23, // 24, 25, 26, 27, 28, 29
        OFF_BUTTON = 30, // 31, 32, 33, 34, 35, 36
        IN_BG = 37,
        OUT_BG = 38,
        OUT_WINDOW = 39
	};

	nw4r::lyt::Pane *rootPane;
	nw4r::lyt::Pane
		*N_buttonN[TOTAL_SETTING_COUNT],
		*N_buttonY[TOTAL_SETTING_COUNT],
		*N_cursor[TOTAL_SETTING_COUNT];

	bool layoutLoaded, visible, settingState[TOTAL_SETTING_COUNT];

	u8 currentButton, currentDiffButton;

	u32 timer, isOnInfo;

    nw4r::lyt::Pane *rightPanes[6];

	nw4r::lyt::Picture
		*P_buttonE_0[3], *P_buttonD_0[3],
		*P_infoButton_0[3], *P_centerBase_00;

	nw4r::lyt::TextBox
		*T_buttonTxt_0[3], *T_buttonTxtS_0[3],
		*T_info_0[3], *T_infoS_0[3];

	static dSettingMenu_c *instance;
    static dActor_c *build();

	USING_STATES(dSettingMenu_c);
	DECLARE_STATE(InitWait);
	DECLARE_STATE(Appear);
	DECLARE_STATE(ButtonChange);
	DECLARE_STATE(WaitForInput);
	DECLARE_STATE(ButtonHit);
	DECLARE_STATE(ShowInfo);
	DECLARE_STATE(Close);
};

#endif