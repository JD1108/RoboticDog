#include <new>
#include <cstdint>

#include "servoPCAWrapper.h"
#include "servoPCA.hpp"

extern "C"{
    ServoPCAHandle servoCreate(int low, int high, int chanel){
        return reinterpret_cast<ServoPCAHandle>(new ServoPCA(low,high,chanel));
    }
    void servoDestroy(ServoPCAHandle servo){
        ServoPCA* s=reinterpret_cast<ServoPCA*>(servo);
        delete s;

    }
    uint8_t getOffLow(ServoPCAHandle servo, float angle){
        ServoPCA* s=reinterpret_cast<ServoPCA*>(servo);
        return s->getOffLow(angle);

    }
    uint8_t getOffHigh(ServoPCAHandle servo,float angle){
        ServoPCA* s=reinterpret_cast<ServoPCA*>(servo);
        return s->getOffHigh(angle);
        }
    uint8_t getReg(ServoPCAHandle servo){
        ServoPCA* s=reinterpret_cast<ServoPCA*>(servo);
        return s->getReg();

    }
    void calibration(int angle){
        ServoPCA::calibration(angle);
    } 
}