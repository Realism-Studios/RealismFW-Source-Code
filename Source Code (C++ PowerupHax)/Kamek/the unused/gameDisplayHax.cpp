#include <game.hpp>

int dGameDisplay_c::newOnExecute() {
	int orig_val = this->onExecute_orig();
	
	return orig_val;
}
