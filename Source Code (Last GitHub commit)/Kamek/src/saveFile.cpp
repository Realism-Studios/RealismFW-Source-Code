#include <game.hpp>
#include <levelnumber.hpp>

#define SAVE_HEADER_SIZE 0x6A0
#define SAVE_BLOCK_SIZE 0x980
#define SAVE_FILE_SIZE (SAVE_HEADER_SIZE + (6*SAVE_BLOCK_SIZE))

/* Actual savefile consists of:
 * 1 HEADER (st: 0x00)
 * 3 SAVES
 * 3 TEMP/QUICK SAVES
*/

int characterIDs[4] = {0, 1, 3, 2};

// initial data to set when making a new file
void SaveBlock::newInitialize() {
	memset(this, 0, 0x980);
	this->version[0] = 14;
	this->version[1] = 0;
	this->bitfield |= 1;

	int world = 0;
	do {
		SetSomeConditionShit(world, Stage_Home, COND_BOTH_EXITS);
		world++;
	} while (world < 9);

	int player = 0;
	do {
		setPlrID(player, characterIDs[player]);
		setPlrMode(player, 0);
		setRest(player, 5);
		setCreateItem(player, 0);
		player++;
	} while (player < 4);

	world = 0;
	do {
		setKinopioCourseNo(world, Stage_Invalid);
		int enemy = 0;
		do {
			setEnemyPathNode(world, enemy, 0xFF);
			setEnemyDirection(world, enemy, UP);
			enemy++;
		} while (enemy < 4);
		world++;
	} while (world < 10);

	setIbaraNow(2);

	// start of custom stuff
	this->difficulty = DIFFICULTY_NORMAL;
}