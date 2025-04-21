#ifndef __M2D_DATA_H
#define __M2D_DATA_H

#include <common.hpp>

class M2D_WorldData {
public:
    struct Header {
        char magic[4]; // 'MDAT'
    };
};

#endif