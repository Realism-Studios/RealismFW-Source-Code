#ifndef POWERUPHAX_H
#define POWERUPHAX_H 

class dShellRenderer_c {
public:
	mHeapAllocator_c allocator;

	m3d::mdl_c
		hammerHelmet, hammerShell,
		lemmyHelmet, lemmyShell,
		bombHelmet, bombShell,
		boomerangHelmet, boomerangShell;
	
	m3d::mdl_c helmets[4], shells[4];
	m3d::mdl_c helmet, shell; // maybe try just one helmet and shell model for everything, it should work probably?
	m3d::anmTexPat_c helmetTexPat, shellTexPat;

	m3d::mdl_c *victimModel;
	u32 headNodeID, rootNodeID;

	dPlayerModel_c *victim;

	dShellRenderer_c();
	~dShellRenderer_c();

	void setup(dPlayerModelHandler_c *handler, int sceneID);
	int chooseShellType();
	void draw();

	static dShellRenderer_c *build();
};

#endif /* POWERUPHAX_H */
