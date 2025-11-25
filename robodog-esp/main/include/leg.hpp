#pragma once

class Leg {
    private:
        const float thighLength=77.0;
        const float lowerLegLength=64;
        float stepHeight=10.0;
        float stepLength=40.0;
        float phaseOffset=0.0;

        float invKinX(float time);

    public:
        float hipAngle(float time);
        float kneeAngle(float time);
        void setStepHeight(float height);
        void setStepLength(float length);

};
