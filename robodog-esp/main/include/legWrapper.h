#pragma once

#ifdef __cplusplus
extern "C"{
#endif

typedef void* LegHandle;

//create and destroy
LegHandle legCreate(float phase);
void legDestroy(LegHandle leg);

//gets
float hipAngle(LegHandle leg,float x, float y);
float kneeAngle(LegHandle leg, float x, float y);

//sets
void setStepTime(float time); // in ms, equal for all legs and should be bigger than 1000ms
void setStepHeight(float height);  // in mm, should be positiv and in good relation to stepLength
void setStepLength(float length);  // in mm, should be between 0 and 40
void setHeight(float height); // in mm, should be negativ between -80 and -135




#ifdef __cplusplus
}
#endif