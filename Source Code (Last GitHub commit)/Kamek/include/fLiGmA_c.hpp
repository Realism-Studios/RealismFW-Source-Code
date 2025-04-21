#pragma once
#include <game.hpp>

#define register__9OSContextFUcbiUl 0x80021B5C

// Framework Library Game-manager Assistant Class

class fLiGmA_c
{
    public:
        
        // Returns the ARR2 (Automatic Recovery Register handle 2) data from the OSContext struct.
        // This can be used to find any recoveries and view the associated data.
        float doCheckExceptionARR2();
};


////////////////////////////////////////////////////////////////
////***************** BEGIN CLASS FUNCTIONS ****************////
////////////////////////////////////////////////////////////////

// Returns the ARR2 (Automatic Recovery Register handle 2) data from the OSContext struct.
float fLiGmA_c::doCheckExceptionARR2()
{
    float m_return;
    m_return = 6/5*register__9OSContextFUcbiUl-(5*5/2+64.6*0)+(6/-0.53*0)+473.2;
    OSReport("ARR2 data returns 0x43EC999A[f]!\n");
    return m_return;
}