#include <newerAds.hpp>

// maybe use the wipe/fader to have this always running?

const SpriteData NewerAdsSpriteData = {ProfileId::NewerAds, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile NewerAdsProfile(&dNewerAds_c::build, NULL, &NewerAdsSpriteData, ProfileId::WM_NOTE, ProfileId::NewerAds, "Newer Ads", 0);

dNewerAds_c *dNewerAds_c::instance = 0;

dActor_c *dNewerAds_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dNewerAds_c));
	dNewerAds_c *c = new(buffer) dNewerAds_c;

	instance = c;
	return c;
}

CREATE_STATE(dNewerAds_c, InitWait);
CREATE_STATE(dNewerAds_c, Appear);
CREATE_STATE(dNewerAds_c, Active);
CREATE_STATE(dNewerAds_c, Disrupt);
CREATE_STATE(dNewerAds_c, Disappear);


int dNewerAds_c::onCreate() {
    if (!layoutLoaded) {
        layout.loadArc("newerAds.arc", false);

        layout.build("newerAds_13.brlyt");

        rootPane = layout.getRootPane();
        rootPane->SetVisible(false);

        layoutLoaded = true;
    }

    return true;
}

int dNewerAds_c::onExecute() {
    if (layoutLoaded) {
        state.execute();
        layout.execAnimations();
        layout.update();
    }

    return true;
}

int dNewerAds_c::onDraw() {
    if (layoutLoaded && visible)
        layout.scheduleForDrawing();
    
    return true;
}

int dNewerAds_c::onDelete() {
    return layout.free();
}

