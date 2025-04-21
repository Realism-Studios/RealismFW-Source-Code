#include <game.hpp>

bool dCourseClear_c::newLoadLayout() {
    bool gotFile = layout.loadArc("corseClear.arc", false);
    if (!gotFile)
        return false;

    layout.build("corseClear_00.brlyt");

    const char *brlanNames[] = {
        "corseClear_00_inText.brlan",
        "corseClear_00_loopText.brlan",
        "corseClear_00_outText.brlan"
    };
    const char *groupNames[] = {
        "A00_inText",
        "A02_loopText",
        "A01_outText"
    };
    int groupIDs[] = {0, 1, 2};

    layout.loadAnimations(brlanNames, 3);
    layout.loadGroups(groupNames, groupIDs, 3);

    rootPane = layout.getRootPane();
    N_marioClear_00 = layout.findPaneByName("N_marioClear_00");

    return true;
}