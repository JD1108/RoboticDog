#pragma once

class Leg {
    private:
        static constexpr float kThighLength=77.0; // mm
        static constexpr float kLowerLegLength=64; // mm
        static float stepTime; 
        float bezierPointsX[5];
        float bezierPointsY[5];
        float stepHeight; // mm
        float stepLength; // mm
        float height; // mm
        float phaseOffset; // from 0.0f to 1.0f for part of hole cycle lead -> highest value first

        float tranX(float time); // calculates the x position of the foot at a given time
        float tranY(float time); // calculates the y position of the foot at a given time
        float bezier(float p[5], float t); // calculates a point on a Bezier curve

    public:
        float hipAngle(float x, float y)const; // in degree
        float kneeAngle(float x, float y)const; // in degree
        void setStepHeight(float height); //in mm, will be used  in future versions 
        void setStepLength(float length); // in mm, is used for turning
        void setHeight(float height);    //in mm, will be used  in future versions with IMU feedback
        void setStepTime(float time);
        Leg(float phase);

};
