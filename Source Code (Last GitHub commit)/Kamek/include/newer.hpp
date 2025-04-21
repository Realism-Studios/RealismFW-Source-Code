#ifndef NEWER_H
#define NEWER_H 

extern int lastLevelIDs[];

void WriteAsciiToTextBox(nw4r::lyt::TextBox *tb, const char *source);

int getUnspentStarCoinCount();

int getStarCoinCount();
int getStarCoinCountForWorld(u32 worldID);
int getTotalStarCoinCountForWorld(u32 worldID);

const wchar_t* getLevelNumber(int world, int level);
const wchar_t* getWorldNumber(int world);

#endif /* NEWER_H */
