#pragma once
// arrays, data, tables, etc for powerup stuff

static char *crsinLoadTable[] = {
	"Mario",
	"Luigi",
	"Kinopio",
	"Yoshi",
	"P_rcha",
	"L_rcha",
	"K_rcha",
	"Y_rcha",
	"Y_TexGreen",
	"Y_TexRed",
	"Y_TexYellow",
	"Y_TexBlue",
	"obj_coin",
	"balloon",
	"I_kinoko",
	"I_fireflower",
	"I_propeller",
	"I_iceflower",
	"I_penguin",
	"I_star",
	"I_yoshi_egg",
	"block_tsuta",
	"teresa",
	"jump_step",
	"ice",
	"ice_piece",
	"obj_dokan",
	"obj_door",
	"obj_kusa",
	"obj_hana",
	"obj_hana_daishizen",
	"block_jump",
	"obj_chikuwa_block",
	"lift_rakka_ashiba",
	"Mask",
	// new
	"suit_shell",
	"I_hammer",
	"hammer",
	"bros",
	"I_bubble",
	"gabon_rock",
	"lemmy_ball",
	"I_lemmy",
	//"lemmyM",
	"kanibo_rock",
	"I_mandy",
	"mandy",
	"I_bomb",
	//"bombM",
	"bros_bombhei",
	"I_mega",
	"I_boomerang",
	"boomerang",
};

// 0 = no limit, -1 = doesn't shoot projectile
static int projectileLimits[TOTAL_POWERUP_COUNT] = {
	0,0,0,0,0,0,0, // padding
	2,		// hammer
	32767,	// bubble
	5,		// lemmy
	5,		// mandy
	2,		// bomb
	-1,		// mega, doesn't throw projectile
	0,		// boomerang, uses special behavior
};
static int projectileNames[TOTAL_POWERUP_COUNT] = {
	0,0,0,0,0,0,0, // padding
	ProfileId::HAMMER,
	ProfileId::BubbleSuit,
	ProfileId::EN_BOUNCE_BALL,
	ProfileId::MandyIGuess,
	ProfileId::BrosBomb,
	0,
	ProfileId::BoomerangHax,
};

static int projectileCounts[TOTAL_POWERUP_COUNT][4];
static u8 boomerangDoWait[4] = {0,0,0,0};