#include <new>

#include "legWrapper.h"
#include "leg.hpp"
#include "globals.h"


extern "C"{

    LegHandle legCreate(float phase){
        return reinterpret_cast<LegHandle>(new Leg(phase));
    }
    void legDestroy(LegHandle leg){
        Leg* p=reinterpret_cast<Leg*>(leg);
        delete p;
    }

    //gets
    float getHipAngle(LegHandle leg,float time){
        Leg* p=reinterpret_cast<Leg*>(leg);
        return p->getHipAngle(time);

    }
    float getKneeAngle(LegHandle leg, float time){
        Leg* p=reinterpret_cast<Leg*>(leg);
        return p->getKneeAngle(time);
    }

    //sets
    void setStepTime(LegHandle leg,float time){
        Leg* p=reinterpret_cast<Leg*>(leg);
        p->setStepTime(time);

    }
    void setStepHeight(LegHandle leg,float height){
        Leg* p=reinterpret_cast<Leg*>(leg);
        p->setStepHeight(height);

    }
    void setStepLength(LegHandle leg,float length){
        Leg* p=reinterpret_cast<Leg*>(leg);
        p->setStepLength(length);

    }
    void setHeight(LegHandle leg,float height){
        Leg* p=reinterpret_cast<Leg*>(leg);
        p->setHeight(height);

    }
    void setTurn(int fac){
        float LengthN=40.0f;
        float c=0.05f;
        float lengthR=LengthN;
        float lengthL=LengthN;

        if(fac>0)
        {
            lengthR-=fac*c*LengthN;
        }
        else if(fac<0)
        {
            lengthL+=fac*c*LengthN;
        }
        setStepLength(legFR,lengthR);
        setStepLength(legRR,lengthR);
        setStepLength(legFL,lengthL);
        setStepLength(legRL,lengthL);

    }

}