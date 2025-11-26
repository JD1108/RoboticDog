#include "leg.hpp"
#include <cmath>

float Leg::tranX(float time){ //returns the X value for every timestep
    float phase=fmodf((time/stepTime)+phaseOffset,1.0f); // phase from 0 to 1
    float x=0.0f;
    if(phase<0.75)
    {
        x=(0.25 * stepLength) - (phase / 0.75) * stepLength; // the standing part of the foot
    }
    else{
        x=bezier(bezierPointsX,phase); // the swinging part of the boot
    }
    
    return x;
}
float Leg::tranY(float time){ //returns the Y value for every timestep
    
    float phase=fmodf((time/stepTime)+phaseOffset,1.0f);
    float y=0.0f;
    if(phase<0.75)
    {
        y=height; // the standing part of the foot
    }
    else{
        y=bezier(bezierPointsY,phase); // the swinging part of the boot
    }
    return y;
}
float Leg::bezier(float p[5], float phase){ //returns the component of bezier curve which was calculated for the motion of a foot
        float t=(phase - 0.75f)/0.25f;
        float b;
        b= powf((1 - t),4.0f)* p[0] +
            4 * powf((1 - t),3.0f) * t * p[1] +
            6 * powf((1 - t),2.0f) * powf(t,2.0f) * p[2] +
            4 * (1 - t) * powf(t,3.0f) * p[3] +
            powf(t,4.0f)* p[4];
    return b;
}
float Leg::hipAngle(float x, float y)const{ //return the hip angle for a given set of x,y in degree
   float aH=90.0f;
   float d=sqrtf(powf(x,2.0f)+powf(y,2.0f));
    if(d<kThighLength+kLowerLegLength){
        aH=atan2f(y,x)-acosf((powf(kThighLength,2.0f)+powf(d,2.0f)-powf(kLowerLegLength,2.0f))/(2*d*kThighLength));
        aH *= 180.0f/M_PI;
        if(aH<180.0f&&aH>0.0f){
            return aH;
        }
        else{
            return NULL;
        }
        
    }
    else{
        return NULL;
    }
}
float Leg::kneeAngle(float x, float y)const{ //return the knee angle for a given set of x,y in degree
    float aK=90.0f;
    float d=sqrtf(powf(x,2.0f)+powf(y,2.0f));
    if(d<kThighLength+kLowerLegLength){
        aK=acosf((powf(kThighLength,2.0f)+powf(kLowerLegLength,2.0f)+powf(d,2.0f))/(2.0f*kLowerLegLength*kThighLength));
        aK *= 180.0f/M_PI;
        if(aK<180.0f&&aK>0.0f){
            return aK;
        }
        else{
            return NULL;
        }
        
    }
    else{
        return NULL;
    }
}
void Leg::setStepHeight(float height){ // in mm, should be positiv and in good relation to stepLength
    if(height<=40.0f && height<=0.5*stepLength && height<=0.3*stepHeight && height>0.0f){
        stepHeight = height;
    }
}
void Leg::setStepLength(float length){ // in mm, should be between 0 and 40
    if(length>=0.0f && length<=40.0f){
        stepLength = length;
        bezierPointsX[0]=-0.75f*stepLength;
        bezierPointsX[1]=-0.83f*stepLength;
        bezierPointsX[2]=0.0f*stepLength;
        bezierPointsX[3]=0.33f*stepLength;
        bezierPointsX[4]=0.25f*stepLength;
    }
}
void Leg::setHeight(float heightNew){ // in mm, should be negativ between -80 and -135
    if(heightNew<=-80.0f && heightNew>=-135.0f){
        height=heightNew;
        bezierPointsY[0]=height;
        bezierPointsY[1]=height+0.1f*stepHeight;
        bezierPointsY[2]=height+3.0f*stepHeight;
        bezierPointsY[3]=height+0.1f*stepHeight;
        bezierPointsY[4]=height;
    }
}   
void Leg::setStepTime(float time){ // in ms, time for one hole cycle, should be bigger than 1 sek
    if(time>1000){
        stepTime=time;
    }
}
Leg::Leg(float phase){
    phaseOffset = phase;
    setStepHeight(10.0f);
    setStepLength(40.0f);
    height=-135.0f;
    stepTime=2000.0; 
}


