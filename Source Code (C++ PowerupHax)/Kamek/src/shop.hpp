#ifndef SHOP_H
#define SHOP_H
#include <game.hpp>
#include <boss.hpp>
#include <profile.hpp>
#include <sfx.hpp>
#include <stage.hpp>
#include "levelinfo.hpp"

extern char CurrentWorld;
extern char CurrentLevel;

class dWMShop_c : public dActor_c {
	public:
		static dActor_c *build();
		static dWMShop_c *instance;

		dWMShop_c() : state(this, &StateID_Hidden) { }

		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		void showLayout();

		m2d::EmbedLayout_c layout;

		bool layoutLoaded;
		bool visible;

		int selected, lastTopRowChoice;
		int shopKind;

		int coinsRemaining, timerForCoinCountdown;

		// Spritedata
		int itemSet;
		int lakituTex;
		int rndItems;
		bool hideCursor;

		enum Animation {
			SHOW_ALL = 0,
			HIDE_ALL = 1,
			ACTIVATE_BUTTON = 2,   // 3,  4,  5,  6,  7
			DEACTIVATE_BUTTON = 8, // 9,  10, 11, 12, 13
			COUNT_COIN = 14,
		};

		enum ItemTypes {
			MUSHROOM = 0,
			FIRE_FLOWER,
			PROPELLER,
			ICE_FLOWER,
			MINI_SHROOM,
			STARMAN,
			HAMMER,
			BUBBLE_FLOWER,
			FREELY_FLOWER,
			PENGUIN,
			ONE_UP, // probably actually a freely thing
			ITEM_TYPE_COUNT
		};

		enum _Constants {
			ITEM_COUNT = 12,
			MAX_INVENTORY_COUNT = 10,
		};

		static const ItemTypes Inventory[MAX_INVENTORY_COUNT][12];

		nw4r::lyt::TextBox
			*Title, *TitleShadow,
			*CoinCount, *CoinCountShadow,
			*BackText, *BuyText;

		nw4r::lyt::Picture
			*BtnLeft[6], *BtnMid[6], *BtnRight[6],
			*P_item[12], *P_lakitu_0[6];

		nw4r::lyt::Pane
			*Buttons[6], *Btn1Base, *Btn2Base;

		int randomColorValue[8];

		void loadInfo();
		void randomizeColors(bool useRandomAlpha);
		void loadItemIcons();

		void buyItem(int item);

		void showSelectCursor();

		dStateWrapper_c<dWMShop_c> state;

		USING_STATES(dWMShop_c);
		DECLARE_STATE(Hidden);
		DECLARE_STATE(ShowWait);
		DECLARE_STATE(ButtonActivateWait);
		DECLARE_STATE(CoinCountdown);
		DECLARE_STATE(Wait);
		DECLARE_STATE(HideWait);
};

#endif

