#ifndef __OPENING_H
#define __OPENING_H

#include <game.hpp>
#include <sfx.hpp>
#include "fileload.hpp"

void OpeningDrawFunc();

TPLPalette *lastLeft, *lastRight, *currLeft, *currRight;

class OpeningInfo {
public:
	u32 lineCount;
	u32 chrCount;
	u16 musicID;
	u16 sfxID;
	u32 textOffset;
};

class OpeningBinary {
public:
	u32 pagesCount;
	OpeningInfo openingInfo[];
};

class dScOpening_c : public dScene_c {
	public:
		dScOpening_c();

		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		FunctionChain initChain;
   		FileHandle fileHandle;
   		OpeningBinary *openingFile;

   		u16 currentMusicID;
   		int musicHandleID;

		u32 currentStateID;
		u32 textTimer;
		u32 currentChar;
		u8 currentFunc;

		int currentPicture;

		nw4r::snd::SoundHandle handles[40];

		m2d::EmbedLayout_c layout;
		u8 layoutLoaded, isVisible, waitingTime;

		void *selectCursor;
		dYesNoWindow_c *yesNoWindow;
		int yesNoStage;

		nw4r::lyt::Pane *N_flipbook_00;

		nw4r::lyt::Picture
			*P_rightPage_00, *P_leftPage_00,
			*P_orightPage_00, *P_oleftPage_00,
			*P_nrightPage_00, *P_nleftPage_00;

		nw4r::lyt::TextBox *T_textS_00, *T_text_00;

		static dScOpening_c *build();
		static dScOpening_c *instance;

		wchar_t *getTextForPage(int page);
		void loadIdlePic();
		void loadPrevPic();
		void loadNextPic();
		void finish();

		void state0_startsPage();
		void state1_writesDialog();
		void state2_showsButton();
		void state3_idlesButton();
		void state4_waitsForInput();
		void state5_hidesButton();
		void state6_flipsPageNext();
		void state7_flipsPageBack();
		void state8_endsOpening();
};

#endif