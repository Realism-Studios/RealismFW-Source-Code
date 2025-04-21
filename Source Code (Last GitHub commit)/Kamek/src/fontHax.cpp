#include <game.hpp>
#include "fileload.hpp"

static const char *brfntFilenames[] = {
	"/Font/mj2d01_marioFont_64_IA4.brfnt",
	"/Font/mj2d00_PictureFont_32_RGBA8.brfnt",
	"/Font/mj2d00_MessageFont_32_I4.brfnt",
	"/Font/mj2d00_numberFont_I4A.brfnt",
	// new fonts start here
	//"mj2d02_MessageFontNewer_32_I4.brfnt",
};

int dFontMng_c::create(EGG::Heap *heap) {
	BinaryFileHeader *brfnt[FONT_COUNT];
	//dDvdLoader_c *loader = fontLoaders;
	File fuckThisGame[FONT_COUNT];

	for (int i = 0; i < FONT_COUNT; i++) {
		OSReport("Setting up font %i\n", i);
		brfnt[i] = 0;
		fontDatas[i] = 0;

		//if (fontDatas[i] == 0) {
			/*while (true) {
				

				//void *data = loader->load(brfntFilenames[i], 0, heap);
				//brfnt = (BinaryFileHeader*)data;

				//brfnt = (BinaryFileHeader*)loader->load(brfntFilenames[i], 0, heap);

				if (brfnt != 0)
					break;
			}*/

			OSReport("Loading \"%s\"\n", brfntFilenames[i]);
				if (fuckThisGame[i].open(brfntFilenames[i]))
					brfnt[i] = (BinaryFileHeader*)fuckThisGame[i].ptr();
			OSReport("Loaded the font! File: %i\n", i);
		//}

		// failed
		if (brfnt == 0) {
			OSReport("oh, it failed :c\n");
			return false;
		}
		
		OSReport("gonna set fontdata %i now\n", i);
		fontDatas[i] = brfnt[i];
		OSReport("gonna set font res %i now\n", i);
		ResFontSetResource(&this->fonts[i], brfnt[i]);
		OSReport("Set ResFont resource!\n");

		if (fuckThisGame[i].isOpen()) {
			OSReport("freeing fileloader %i\n", i);
			fuckThisGame[i].close();
		}
	}

	OSReport("All done!!!\n");

	return true;
}


int dFontMng_c::getResFontIndex(const char *filename) {
	u32 returnedIndex = 0;

	for (int i = 0; i < FONT_COUNT; i++) {
		int result = strcmp(filename, brfntFilenames[i]);

		if (result != 0) {
			returnedIndex++;
		}

		if (returnedIndex > FONT_COUNT)
			return false;
	}

	return returnedIndex;
}