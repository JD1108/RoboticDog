#pragma once
#include <cstdint>


class ServoPCA { // this class is made for MG90S with PCA9685
    private:
        int cal10; // dutytime for 10 degree in X/4096 *20ms
        int cal170; // dutytime for 170 degree in X/4096 *20ms
        uint8_t reg; //register for chanel on PCA
        uint16_t getWidth(float angle)const;

    public:
        ServoPCA(int low, int high, int chanel);
        uint8_t getOffLow(float angle)const;
        uint8_t getOffHigh(float angle)const;
        uint8_t getReg()const;
        static void calibration();
        

        
};