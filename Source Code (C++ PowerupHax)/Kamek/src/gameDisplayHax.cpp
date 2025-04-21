#include <game.hpp>
#include <stage.hpp>
#include <sfx.hpp>
#include "fileload.hpp"

extern char CurrentWorld;
extern char CurrentLevel;

static bool shouldHideGameSceneClock = false;

#define ANIM_OUT_EXIT 0

int dGameDisplay_c::onCreate() {
	if (!layoutCreated) {
		bool setupLyt = createLayout();
		if (!setupLyt)
			return false;

		layout.drawOrder = 17;

		// set a bunch of values
		for (int i = 0; i < 3; i++) {
			m_playNum[i] = -1;
		}
		_424 = 0;
		grayColorSetForLives[0] = false;
		_3D0 = -1;
		_428 = 0;
		grayColorSetForLives[1] = false;
		_3D4 = -1;
		_42C = 0;
		grayColorSetForLives[2] = false;
		_3D8 = -1;
		_430 = 0;
		grayColorSetForLives[3] = false;

		score = 1;
		_3E4 = 1; // probably some value
		coins = 1;
		timer = -1;
		mustAtLeast2ForScoreToCount = 2;

		setPlayNum(m_playNum);
		setCoinNum(0);
		setTime(0);
		setScore(0);

		// back to this
		isPausing = 0;
		isUnpausing = 0;
		_3FC = -1;
		_3F8 = -1;
		isCountingScore = 0;
		isCountingScore2 = 0;
		_400 = 0;
		_404 = 30;
		_408 = 30;
		countdownInterval = 0;
		gamePaused = 0;

		_540 = 0;
		_53C = 0;
		_538 = 0;

		restDispSetup();
		sub_801585C0();

		layout.disableAllAnimations();

		/* hide the layout if at least one of the following is true:
		* the screen type is a titlescreen replay/the actual titlescreen
		* the level type is not 0 (probably "Toad House Type" in level Area settings),
		* the level is Peach's Castle (XX-41)
		* the credits mode is activated for the area
		*/
		if ((((stageScreenType == 2) || (stageScreenType == 3)) || (levelType != 0)) || ((m_startGameInfo.level1 == STAGE_PEACH || (CreditsModeActive == true))))
			rootPane->SetVisible(false);
		else
			rootPane->SetVisible(true);

		// super guide mode
		if (stageScreenType == 1)
			N_otasukeInfo_00->SetVisible(true);
		else
			N_otasukeInfo_00->SetVisible(false);

		// player has not taken control of super guide luigi
		if (!m_OtasukeAfter) {
			playerIsControllingSuperGuide = false;
			N_otasukeChu_00->SetVisible(false);
			N_left_00->SetVisible(true);
		} else {
			playerIsControllingSuperGuide = true;
			N_otasukeChu_00->SetVisible(true);
			N_left_00->SetVisible(false);
			layout.resetAnim(ANIM_OUT_EXIT);
		}

		// cannon stage
		if (m_startGameInfo.level1 == STAGE_WARP) {
			N_collection_00->SetVisible(false);
			N_score_00->SetVisible(false);
			N_time_00->SetVisible(false);
		} else {
			N_collection_00->SetVisible(true);
			N_score_00->SetVisible(true);
			N_time_00->SetVisible(true);
		}

		// coin battle mode
		if ((GameFlag & CoinBattle) != 0)
			N_score_00->SetVisible(false);

		SaveBlock *save = GetSaveFile()->GetBlock(-1);
		int world = m_startGameInfo.world1;
		int level = m_startGameInfo.level1;

		/*if (world > 9)
			world = 0;
		if (level > 41)
			level = 0;*/

		bool normalStage = CheckConditionShit(world, level, IsRegularLevel);
		if ((((GameFlag & ExtraMode) == 0) && ((GameFlag & UnkFlag) == 0)) && (!normalStage))
			N_collection_00->SetVisible(false);


		// setup star coins
		for (int coin = 0; coin < 3; coin++) {
			int paneOffset = 0;
			relatedToStarCoins = 3;
			u32 isCollect = save->CheckIfCoinCollected(CurrentWorld, CurrentLevel, coin);

			if ((StarCoinStatusArray[coin] != 4) || ((isCollect & 0xFF) != 0)) {
				// might not work
				(P_collectOff_00 + paneOffset)->SetVisible(false);
				(P_collection_00 + paneOffset)->SetVisible(true);
				relatedToStarCoins = 2;
			}

			paneOffset += 8;
		}

		// adjust scales
		Vec2 adjustedScale;
		nw4r::lyt::Pane *N_proportionC_00 = layout.findPaneByName("N_proportionC_00");

		if (IsWideScreen()) {
			adjustedScale.x = 1.0f;
			adjustedScale.y = 1.0f;
		}
		else {
			adjustedScale.x = 0.85f;
			adjustedScale.y = 0.85f;
		}

		N_proportionL_00->scale = adjustedScale;
		N_proportionC_00->scale = adjustedScale;
		N_proportionR_00->scale = adjustedScale;

		// put all of the pane positions into class Vecs
		leftProportionTrans.x = N_proportionL_00->trans.x;
		leftProportionTrans.y = N_proportionL_00->trans.y;
		leftProportionTrans.z = N_proportionL_00->trans.z;

		rightProportionTrans.x = N_proportionR_00->trans.x;
		rightProportionTrans.y = N_proportionR_00->trans.y;
		rightProportionTrans.z = N_proportionR_00->trans.z;

		otasukeChuTrans.x = N_otasukeChu_00->trans.x;
		otasukeChuTrans.y = N_otasukeChu_00->trans.y;
		otasukeChuTrans.z = N_otasukeChu_00->trans.z;

		areaZankiTrans.x = N_areaZanki_00->trans.x;
		areaZankiTrans.y = N_areaZanki_00->trans.y;
		areaZankiTrans.z = N_areaZanki_00->trans.z;

		otasukeInfoTrans.x = N_otasukeInfo_00->trans.x;
		otasukeInfoTrans.y = N_otasukeInfo_00->trans.y;
		otasukeInfoTrans.z = N_otasukeInfo_00->trans.z;

		areaCoinTrans.x = N_areaCoin_00->trans.x;
		areaCoinTrans.y = N_areaCoin_00->trans.y;
		areaCoinTrans.z = N_areaCoin_00->trans.z;

		areaScoreTrans.x = N_areaScore_00->trans.x;
		areaScoreTrans.y = N_areaScore_00->trans.y;
		areaScoreTrans.z = N_areaScore_00->trans.z;

		timeTrans.x = N_time_00->trans.x;
		timeTrans.y = N_time_00->trans.y;
		timeTrans.z = N_time_00->trans.z;

		otehonPosChange();

		// why are there MORE???
		_449 = 0;
		_44A = 0;
		layoutCreated = true;
		completedStage_maybe = false;
		_418 = 0xFF;
		_44D = 0;
		_41C = 0xFF;
		_44E = 0;
		_420 = 0xFF;
		_44F = 0;
		_450 = 0;
		_451 = 0;

		layout.update();

		if (shouldHideGameSceneClock)
			layout.findPaneByName("N_clock_00")->SetVisible(true);
		else
			layout.findPaneByName("N_clock_00")->SetVisible(false);

		// start custom stuff here
		setupLayoutOverrides();
	}

	return true;
}

bool dGameDisplay_c::createLayout() {
	bool gotFile = layout.loadArc("gameScene.arc", false);
	if (!gotFile)
		return false;

	layout.build("gameScene_37.brlyt");

	static const char *brlanName[] = {"gameScene_37_inMarioCoin.brlan"};
	layout.loadAnimations(brlanName, 1);

	static const char *groupName[] = {"C00_inMarioCoin"};
	static const int groupID[] = {0};
	layout.loadGroups(groupName, groupID, 1);

	rootPane = layout.getRootPane();

	static const char *paneNames[] = {
		"N_otasukeInfo_00", "N_otasukeChu_00", "N_left_00", "N_coin_00",
		"N_collection_00", "N_score_00", "N_areaZanki_00", "N_areaCoin_00",
		"N_areaScore_00", "N_marioIcon_00", "N_luigiIcon_00", "N_kinoB_00",
		"N_kinoY_00", "N_coin_01", "N_time_00", "N_proportionL_00",
		"N_proportionR_00", "N_coin1st_00", "N_coin2nd_00", "N_coin3rd_00"
	};
	layout.getPanes(paneNames, &N_otasukeInfo_00, 20);

	static const char *pictureNames[] = {
		"P_collectOff_00", "P_collection_00",
		"P_collectOff_01", "P_collection_01",
		"P_collectOff_02", "P_collection_02",
		"P_marioIcon_00", "P_luijiIcon_00",
		"P_kinoB_00", "P_kinoY_00"
	};
	layout.getPictures(pictureNames, &P_collectOff_00, 10);

	P_collectOff_00->SetVisible(true);
	P_collection_00->SetVisible(false);
	P_collectOff_01->SetVisible(true);
	P_collection_01->SetVisible(false);
	P_collectOff_02->SetVisible(true);
	P_collection_02->SetVisible(false);
	P_marioIcon_00->SetVisible(false);

	static const char *textBoxNames[] = {
		"T_left_00", "T_x_01",
		"T_left_01", "T_x_02",
		"T_left_02", "T_x_03",
		"T_left_03", "T_x_04",
		"T_coin_00", "T_time_00",
		"T_score_00"
	};
	layout.getTextBoxes(textBoxNames, &T_left_00, 11);

	static const char *bmgTextBoxes[] = {
		"T_otaChuS_00", "T_otaChu_01",
		"T_InfoS_00", "T_Info_00"
	};
	static const int messageIDs[] = {14, 14, 13, 13};
	layout.setLangStrings(bmgTextBoxes, messageIDs, 1, 4);

	return true;
}

int dGameDisplay_c::onExecute() {
	if (layoutCreated) {
		restCoinAnimeCheck();
		stateMgr.execute();

		bool animPlaying = layout.isAnyAnimOn();
		if (((animPlaying) || (_450 != 0)) || (_451 != 0)) {
			layout.execAnimations();
			layout.update();
		}

		areaCheck();
		relatedToPaneAlpha();

		if (_424 != 0) _424--;
		if (_428 != 0) _428--;
		if (_42C != 0) _42C--;
		if (_430 != 0) _430--;

		if (shouldHideGameSceneClock)
			updateAnalogClock();
	}

	doWaitCheck(); // boomeranghax.cpp

	return true;
}

// make the maximum visible score 99999999
void dGameDisplay_c::setScore(int newScore) {
	if (score == newScore)
		return;
	if (score > 99999998)
		return;

	mustAtLeast2ForScoreToCount += 1;
	if (mustAtLeast2ForScoreToCount < 2)
		return;

	mustAtLeast2ForScoreToCount = 0;
	score = newScore;

	if (newScore > 99999998)
		score = 99999999;

	const int fieldLength = 8;
	WriteNumberToTextBox(&score, &fieldLength, T_score_00, 1);
}



// start of truly custom stuff

void dGameDisplay_c::setupLayoutOverrides() {
	// also filehandle and filename orders
	static const char *paletteNames[] = {
		"mario.tpl",
		"luigi.tpl",
		"kinoY.tpl",
		"kinoB.tpl",
		"timer.tpl"
	};

	nw4r::lyt::Picture *P_timer_00 = layout.findPictureByName("P_timer_00");

	static File TPLHandles[5];
	char filename[5][64];

	// per-world insertion
	for (int i = 0; i < 5; i++) {
		sprintf(filename[i], "/Layout/tecks tore/stagehud/W%d/%s", CurrentWorld+1, paletteNames[i]);
		TPLHandles[i].open(filename[i]);
	}

	// isOpen returns false if no file is open
	if (TPLHandles[0].isOpen()) P_marioIcon_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[0].ptr(), 0);
	if (TPLHandles[1].isOpen()) P_luijiIcon_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[1].ptr(), 0);
	if (TPLHandles[2].isOpen()) P_kinoY_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[2].ptr(), 0);
	if (TPLHandles[3].isOpen()) P_kinoB_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[3].ptr(), 0);
	if (TPLHandles[4].isOpen()) P_timer_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[4].ptr(), 0);


	// per-level insertion, takes priority over world-based overrides
	for (int i = 0; i < 5; i++) {
		sprintf(filename[i], "/Layout/tecks tore/stagehud/%02d-%02d/%s", CurrentWorld+1, CurrentLevel+1, paletteNames[i]);
		TPLHandles[i].open(filename[i]);
	}

	if (TPLHandles[0].isOpen()) P_marioIcon_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[0].ptr(), 0);
	if (TPLHandles[1].isOpen()) P_luijiIcon_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[1].ptr(), 0);
	if (TPLHandles[2].isOpen()) P_kinoY_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[2].ptr(), 0);
	if (TPLHandles[3].isOpen()) P_kinoB_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[3].ptr(), 0);
	if (TPLHandles[4].isOpen()) P_timer_00->material->texMaps[0].ReplaceImage((TPLPalette*)TPLHandles[4].ptr(), 0);
}

extern int MessageBoxIsShowing;

void dGameDisplay_c::updateAnalogClock() {
	nw4r::lyt::Pane *N_hour_00 = layout.findPaneByName("N_hour_00");
	nw4r::lyt::Pane *N_minute_00 = layout.findPaneByName("N_minute_00");

	/* values for clock incrementation:
	  * hours increment by -30 degrees, and minutes increment by -6 degrees
	  * a minute is equal to a real life second (aka: 60 frames)
	  * so an "hour" is really a minute
	*/

	// don't tick if the game is paused for whatever reason
	if ((MessageBoxIsShowing) || (gamePaused))
		return;

	numFrames++;
	N_minute_00->rotate.z += -0.1f;

	// 1 second has passed
	if (numFrames == 60) {
		numFrames = 0;

		//N_minute_00->rotate.z += -6.0f;

		// smooth rotation
		N_hour_00->rotate.z += -0.5f;
	}
}
