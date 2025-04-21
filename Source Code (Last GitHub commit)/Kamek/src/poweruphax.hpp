#ifndef POWERUPHAX_H
#define POWERUPHAX_H 

class dHammerSuitRenderer_c {
	public:
		mHeapAllocator_c allocator;

		m3d::mdl_c hammerHelmet, hammerShell, lemmyHelmet, lemmyShell, bombHelmet, bombShell, boomerangHelmet, boomerangShell;

		m3d::mdl_c *victimModel;
		u32 headNodeID, rootNodeID;

		dPlayerModel_c *victim;

		dHammerSuitRenderer_c();
		~dHammerSuitRenderer_c();
		void setup(dPlayerModelHandler_c *handler);
		void setup(dPlayerModelHandler_c *handler, int sceneID);
		void draw();

		static dHammerSuitRenderer_c *build();
		void SetupHelmetAndShell(dPlayerModelHandler_c *handler, int sceneID, nw4r::g3d::ResFile &resFile,
			m3d::mdl_c &helmet, m3d::mdl_c &shell);
		void initializeHelmetAndShell(int powerupID);
};

#endif /* POWERUPHAX_H */
