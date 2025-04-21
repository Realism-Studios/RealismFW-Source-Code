static const char *stockItemBrlanNames[] = {
	"stockItem_27_inWindow.brlan",
	"stockItem_27_itemOnButton.brlan",
	"stockItem_27_itemIdleButton.brlan",
	"stockItem_27_itemHitButton.brlan",
	"stockItem_27_itemOffButton.brlan",
	"stockItem_27_outWindow.brlan"
};

static const char *stockItemGroupNames[] = {
	"A00_Window",
	"B00_itemButton", "B01_itemButton","B02_itemButton", "B06_itemButton", "B04_itemButton", "B05_itemButton", "B03_itemButton",
	"B00_itemButton", "B01_itemButton","B02_itemButton", "B06_itemButton", "B04_itemButton", "B05_itemButton", "B03_itemButton",
	"B00_itemButton", "B01_itemButton","B02_itemButton", "B06_itemButton", "B04_itemButton", "B05_itemButton", "B03_itemButton",
	"B00_itemButton", "B01_itemButton","B02_itemButton", "B06_itemButton", "B04_itemButton", "B05_itemButton", "B03_itemButton",
	"A00_Window",
	"HammerGroup", "HammerGroup", "HammerGroup",
};
static const int stockItemGroupIDs[] = {
	0,
	1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,
	4,4,4,4,4,4,4,
	5,
	1,3,4,
};

enum StockItemAnimation {
	STOCK_IN_WINDOW = 0,
	ON_KINOKO, ON_FIRE, ON_PROPELLER, ON_ICE, ON_PENGUIN, ON_MINI, ON_STAR,
	// skipping idle animations...
	HIT_KINOKO = 15, HIT_FIRE, HIT_PROPELLER, HIT_ICE, HIT_PENGUIN, HIT_MINI, HIT_STAR,
	OFF_KINOKO, OFF_FIRE, OFF_PROPELLER, OFF_ICE, OFF_PENGUIN, OFF_MINI, OFF_STAR,
	STOCK_OUT_WINDOW,
	// new
	ON_HAMMER, HIT_HAMMER, OFF_HAMMER,

	STOCK_NUM_ANIMS
};


static const char *stockItemPaneNames[] = {
	"N_forUse_1PPos",
	"N_forUse_2PPos",
	"N_forUse_3PPos",
	"N_forUse_4PPos",
	"N_iconKinoko_00",
	"N_iconFlower_00",
	"N_iconPro_00",
	"N_iconIce_00",
	"N_iconPen_00",
	"N_mameKinoko_00",
	"N_iconStar_00",
	"N_stockItem",
	"N_stockItem_01",
	"N_itemSelect_00"
};
static const char *stockItemPictureNames[] = {
	"P_iconKinoko_00",
	"P_iconFlower_00",
	"P_iconPro_00",
	"P_iconIce_00",
	"P_iconPen_00",
	"P_mameKinoko_00",
	"P_iconStar_00",
	"P_buttonBase_00",
	"P_buttonBase_01",
	"P_buttonBase_02",
	"P_buttonBase_06",
	"P_buttonBase_04",
	"P_buttonBase_05",
	"P_buttonBase_03",
	"P_iconBase_00",
	"P_iconBase_01",
	"P_iconBase_02",
	"P_iconBase_03",
	"P_iconBase_04",
	"P_iconBase_05",
	"P_iconBase_06",
	"P_iconBase_07",
	"P_iconBase_08",
	"P_iconBase_09"
};


static const char *bmgTextBoxes[] = {
	"T_titleStock_00",
	"T_titleUse_01"
};
static const int bmgIDs[] = {0x1E, 0x2F};

// SHADOW
static const char *shadowTextboxNames[] = {
	"T_itemRest_00",
	"T_itemRest_01",
	"T_itemRest_02",
	"T_itemRest_03",
	"T_itemRest_04",
	"T_itemRest_05",
	"T_itemRest_06",
	"T_x_00",
	"T_x_01",
	"T_x_02",
	"T_x_03",
	"T_x_04",
	"T_x_05",
	"T_x_06",
};
static const char *shadowPictureNames[] = {
	"P_buttonBaseS_00",
	"P_buttonBaseS_01",
	"P_buttonBaseS_02",
	"P_buttonBaseS_03",
	"P_buttonBaseS_04",
	"P_buttonBaseS_05",
	"P_buttonBaseS_06",
};
