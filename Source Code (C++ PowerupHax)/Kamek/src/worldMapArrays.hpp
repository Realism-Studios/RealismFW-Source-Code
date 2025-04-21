static char *commonRes[] = {
	"RouteInfo",
	"CollectionCoinCourseSort",
	"cobCastle",
	"cobCourse",
	"cobKinokoRed",
	"cobKinoko1up",
	"cobKinokoStar",
	"cobStart",
	"cobAnchor",
	"cobToride",
	"cobGhost",
	"cobCannon",
	"cobKoopaShip",
	"character_SV",
	"cobSurrender",
	"cobKinopio",
};

static char *stockItemModels[] = {
	"teresa",
	"I_kinoko",
	"I_fireflower",
	"I_propeller",
	"I_iceflower",
	"I_penguin",
	"I_star",
	// new
	"I_hammer",
	"I_bubble",
	"I_lemmy",
};

static char *stockItemAnims[] = {
	"rcha", // pulling a nintendo move here to keep the array count correct
	"SI_kinoko",
	"SI_fireflower",
	"SI_propeller",
	"SI_iceflower",
	"SI_penguin",
	"SI_star",
	"SI_hammer",
	"SI_bubble",
	"SI_lemmy",
};


// map-specific resources
static int mapResCounts[] = {
	1,
	1,
	1,
	2,
	0,
	3,
	1,
	2,
	1
};
static char *mapRes[9][4] = {
	{ // w1
		"cobPeachCastle"
	},
	{ // w2
		"cobSandpillar"
	},
	{ // w3
		"cobHatenaSwitch"
	},
	{ // w4
		"CS_W4_Palm"
	},
	{}, // w5
	{ // w6
		"cobKiller",
		"cobRotary",
		"cobDokanRoute"
	},
	{ // w7
		"cobTsuta"
	},
	{ // w8
		"cobKoopaCastle",
		"cobfirepillar"
	},
	{ // w9
		"cobBoard"
	}
};

static int enemyResCounts[] = {
	2,
	2,
	2,
	3,
	4,
	4,
	2,
	3,
	1
};
static char *enemyRes[9][4] = {
	{ // w1
		"larry",
		"kuribo"
	},
	{ // w2
		"roy",
		"togezo"
	},
	{ // w3
		"lemmy",
		"bros"
	},
	{ // w4
		"wendy",
		"fugu",
		"koopaJr"
	},
	{ // w5
		"iggy",
		"pakkun_walk",
		"hanachan_body",
		"hanachan_head"
	},
	{ // w6
		"morton",
		"killer",
		"koopaJr",
		"pakkun"
	},
	{ // w7
		"ludwig",
		"jugem"
	},
	{ // w8
		"kameck",
		"peach",
		"bubble"
	},
	{} // w9
};
