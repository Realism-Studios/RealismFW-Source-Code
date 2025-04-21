#include <common.hpp>

// requested by kirblue
extern "C" void PrologReport() {
    for (int i = 0; i != 700; i++) {
        OSReport(">L\n");
        OSReport("<OK\n");
    }
}