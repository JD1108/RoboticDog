#include "legWrapper.h"
#include "leg.hpp"
#include <new>

extern "C"{

    LegHandle legCreate(float phase){
        return reinterpret_cast<LegHandle>(new(std::nothrow) Leg(phase));
       
    }

}