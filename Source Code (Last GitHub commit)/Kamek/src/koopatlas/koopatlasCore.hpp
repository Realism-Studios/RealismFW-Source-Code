/* KoopAtlas Map Engine
 * Created by Treeki
 */

#ifndef __KOOPATLAS_H
#define __KOOPATLAS_H

#include <common.hpp>
#include <game.hpp>
#include <g3dhax.hpp>
#include <profile.hpp>
#include <sfx.hpp>

#include "levelinfo.hpp"
#include "koopatlas/kpMapData.hpp"
//#include "koopatlas/shop.hpp"
//#include "koopatlas/starcoin.hpp"
#include "koopatlas/kpMapHud.hpp"
#include "koopatlas/kpPathManager.hpp"
#include "koopatlas/kpMusic.hpp"

#define WM_DEBUGGING
#define WM_SPAMMY_DEBUGGING

#ifdef WM_DEBUGGING
#define MapReport OSReport
#else
#define MapReport(...)
#endif

//#ifdef WM_SPAMMY_DEBUGGING
#define SpammyReport OSReport
//#else
//#define SpammyReport(...)
//#endif

void NewerMapDrawFunc();

class daKPPlayer_c;
class dKPMap_c;
class dKPMapHud_c;
//class dWMShop_c;
//class dWMStarCoin_c;
class dKPCamera_c;

class dScKoopatlas_c : public dScene_c {
	public:
		dScKoopatlas_c();

		FunctionChain initChain;

		dStateWrapper_c<dScKoopatlas_c> state;

		USING_STATES(dScKoopatlas_c);
		DECLARE_STATE(Limbo);
		DECLARE_STATE(ContinueWait);
		DECLARE_STATE(Normal);
		DECLARE_STATE(CompletionMsg);
		DECLARE_STATE(CompletionMsgHideWait);
		DECLARE_STATE(CSMenu);
		DECLARE_STATE(TitleConfirmOpenWait);
		DECLARE_STATE(TitleConfirmSelect);
		DECLARE_STATE(TitleConfirmHitWait);
		DECLARE_STATE(PlayerChangeWait);
		DECLARE_STATE(EasyPairingWait);
		DECLARE_STATE(PowerupsWait);
		DECLARE_STATE(ShopWait);
		DECLARE_STATE(CoinsWait);
		DECLARE_STATE(SaveOpen);
		DECLARE_STATE(SaveSelect);
		DECLARE_STATE(SaveWindowClose);
		DECLARE_STATE(SaveDo);
		DECLARE_STATE(SaveEndWindow);
		DECLARE_STATE(SaveEndCloseWait);
		DECLARE_STATE(SaveError);

		dCourseSelectMenu_c *csMenu;
		dSelectCursor_c *selectCursor;
		dNumberOfPeopleChange_c *numPeopleChange;
		dYesNoWindow_c *yesNoWindow;
		dContinue_c *continues;
		dStockItem_c *stockItem;
		dStockItemShadow_c *stockItemShadow;
		dEasyPairing_c *easyPairing;


		int onCreate();
		int onDelete();
		int onExecute();

		static dActor_c *build();
		static dScKoopatlas_c *instance;


		daKPPlayer_c *player;
		dKPMapHud_c *hud;
		dKPMap_c *map;
		//dWMShop_c *shop;
		//dWMStarCoin_c *coins;

		int loadResources();
		int createLayouts();

		int currentMapID;
		const char *mapPath;
		dKPMapData_c mapData;
		dWMPathManager_c pathManager;

		dDvdLoader_c mapListLoader;

		bool isFirstPlay;
		bool isAfterKamekCutscene;
		bool isAfter8Castle;
		bool isEndingScene;
		void startMusic();

		bool hidePlayer;

		bool warpZoneHacks;

		int mustFixYesNoText;

		u32 iterateMapList(u32(*callback)(u32,const char *,int,int), u32 userData, int *ptrIndex = 0);
		const char *getMapNameForIndex(int index);
		int getIndexForMapName(const char *name);

		void startLevel(dLevelInfo_c::entry_s *level);

		bool canDoStuff();
		bool mapIsRunning();

		void showSaveWindow();

		bool keepMusicPlaying;
};

extern void *_8042A788;
void somethingAboutSound(void *beef);

#endif

