#include <game.hpp>
#include <sfx.hpp>

enum CSM_Animations {
    IN_WINDOW = 0,
    LOOP_WINDOW = 1,
    ON_BUTTON = 2, // 3, 4, 5, 6
    IDLE_BUTTON = 7, // 8, 9, 10, 11
    HIT_BUTTON = 12, // 13, 14, 15, 16
    OFF_BUTTON = 17, // 18, 19, 20, 21
    OUT_WINDOW = 22,
};

int dCourseSelectMenu_c::onCreate() {
    if (!layoutLoaded) {
        bool gotLyt = loadLayout();
        if (!gotLyt)
            return false;
        
        SaveBlock *save = GetSaveFile()->GetBlock(-1);
        int msgID = 7;

        // Uncomment this line if you want to print the Quick Save text like normal
        //if ((save->bitfield & SAVE_BIT_GAME_COMPLETE) != 0)
            msgID = 4;

        if (IsWideScreen()) {
            P_back->SetVisible(true);
            P_back->SetVisible(false);
        } else {
            P_back->SetVisible(false);
            P_back->SetVisible(true);
        }

        // write BMG to Save button
        WriteBMGToTextBox(T_corseSelectS03, GetBMG(), 2, msgID, 0);
        WriteBMGToTextBox(T_corseSelect_03, GetBMG(), 2, msgID, 0);

        rootPane->SetVisible(false);
        layout.drawOrder = 3;

        layoutLoaded = true;
        visible = false;
        choiceWasMade = false;
        _273 = false;

        //currentSelection = GenerateRandomNumber(4);
    }

    return true;
}

bool dCourseSelectMenu_c::loadLayout() {
    bool gotFile = layout.loadArc("corseSelectMenu.arc", false);
    if (!gotFile)
        return false;
    
    layout.build("corseSelectMenu_11.brlyt");

    static const char *brlanNames[] = {
        "corseSelectMenu_11_inWindow.brlan",
        "corseSelectMenu_11_loopWindow.brlan",
        "corseSelectMenu_11_onButton.brlan",
        "corseSelectMenu_11_idleButton.brlan",
        "corseSelectMenu_11_hitButton.brlan",
        "corseSelectMenu_11_offButon.brlan",
        "corseSelectMenu_11_outWindow.brlan"
    };
    layout.loadAnimations(brlanNames, 7);

    static const char *groupNames[] = {
        "A00_Window",
        "A00_Window",
        "B00_Button", "B01_Button", "B02_Button", "B03_Button", "B04_Button",
        "B00_Button", "B01_Button", "B02_Button", "B03_Button", "B04_Button",
        "B00_Button", "B01_Button", "B02_Button", "B03_Button", "B04_Button",
        "B00_Button", "B01_Button", "B02_Button", "B03_Button", "B04_Button",
        "A00_Window"
    };
    static const int groupIDs[] = {
        0,
        1,
        2, 2, 2, 2, 2,
        3, 3, 3, 3, 3,
        4, 4, 4, 4, 4,
        5, 5, 5, 5, 5,
        6
    };
    layout.loadGroups(groupNames, groupIDs, 23);

    rootPane = layout.getRootPane();

    static const char *pictureNames[] = {
        "P_SBBase_01", "P_SBBase_02", "P_SBBase_03",
        "P_SBBase_04", "P_back", "P_backWhite"
    };
    layout.getPictures(pictureNames, &P_SBBase_01, 6);

    P_SBBase_00 = layout.findPictureByName("P_SBBase_00");

    // grab these since the string written to them is different depending on the first save bit
    static const char *saveTextNames[] = {"T_corseSelectS03", "T_corseSelect_03"};
    layout.getTextBoxes(saveTextNames, &T_corseSelectS03, 2);

    static const char *textBoxNames[] = {
        "T_corseSelectS01", "T_corseSelect_01",
        "T_corseSelectS02", "T_corseSelect_02",
        "T_corseSelectS04", "T_corseSelect_05",
        "T_back"
    };
    static const int messageIDs[] = {2, 2, 3, 3, 5, 5, 6};
    layout.setLangStrings(textBoxNames, messageIDs, 2, 7);

    return true;
}

int dCourseSelectMenu_c::onExecute() {
    if (visible) {
        state.execute();

        layout.execAnimations();
        layout.update();
    }

    // not related to the menu at all, this saves the last P1 for the File Selection screen
    SaveBlock *saveBlock = GetSaveFile()->GetBlock(-1);

    if (saveBlock->lastP1Character != Player_ID[0]) {
		OSReport("Player 1 ID: %d\n", Player_ID[0]);
		saveBlock->lastP1Character = Player_ID[0];
	}

    // this'll make the menu very slowly rotate while it's open
    rootPane->rotate.z += 0.0001f;

    return true;
}

int dCourseSelectMenu_c::onDraw() {
    if (layoutLoaded && visible)
        layout.scheduleForDrawing();
    
    return true;
}

void dCourseSelectMenu_c::setupSelectCursor() {
    if (_274 == 0) // ???
        _274 = 0;
    
    // doesn't work with the first option, might also be broken with all others too, idk
    UpdateSelectCursor(P_SBBase_01 + (0x4 * currentSelection), 4, false);
}


// reset rotation on window open
void dCourseSelectMenu_c::beginState_InitWait() {
    rootPane->rotate.z = 0.0f;
}

// handles button functionality for the menu
void dCourseSelectMenu_c::executeState_PauseDisp() {
    int nowPressed = Remocon_GetPressed(GetActiveRemocon());

    if (_273 == 0) {
        //setupSelectCursor();

        // close the menu
        bool shouldClose = CheckIfMenuShouldBeCancelledForSpecifiedWiimote(0);
        if ((shouldClose) || (nowPressed & WPAD_PLUS)) {
            state.setState(&StateID_ClouseAnimeEndWait);
        }

        // press whatever button we're on
        if ((nowPressed & WPAD_TWO) || (nowPressed & WPAD_A)) {
            MapSoundPlayer(SoundRelatedClass, SFX_DS_DECIDE, 1);
            state.setState(&StateID_HitAnimeEndWait);
        }

        // next button
        /*if (nowPressed & WPAD_DOWN) {
            currentSelection++;
            if (currentSelection > 4) // go back to first button
                currentSelection = 0;

            state.setState(&StateID_ButtonChangeAnimeEndWait);
        }

        // previous button
        if (nowPressed & WPAD_UP) {
            currentSelection--;
            if (currentSelection < 0) // go to last button
                currentSelection = 4;
            
            state.setState(&StateID_ButtonChangeAnimeEndWait);
        }*/

        // chooses random button
        if ((nowPressed & WPAD_UP) || (nowPressed & WPAD_DOWN)) {
            rollAgain:
            int randomChoice = GenerateRandomNumber(5);

            // make sure we don't select the same button
            if (randomChoice != currentSelection)
                currentSelection = randomChoice;
            else
                goto rollAgain;
            
            state.setState(&StateID_ButtonChangeAnimeEndWait);
        }
    }
}