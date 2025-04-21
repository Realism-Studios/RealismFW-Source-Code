#include <game.hpp>

bool dGameOver_c::newLoadLayout() {
    bool gotFile = layout.loadArc("gameOver.arc", false);
    if (!gotFile)
        return false;

    layout.build("");
}