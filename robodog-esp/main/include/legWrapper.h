#pragma once

#ifdef __cplusplus
extern "C"{
#endif

typedef void* LegHandle;

//create and destroy
LegHandle legCreate(float phase);
void legDestroy(LegHandle leg);

//gets
float getHipAngle(LegHandle leg,float time);
float getKneeAngle(LegHandle leg, float time);

//sets
void setStepTime(LegHandle leg,float time); // in ms, equal for all legs and should be bigger than 1000ms
void setStepHeight(LegHandle leg,float height);  // in mm, should be positiv and in good relation to stepLength
void setStepLength(LegHandle leg,float length);  // in mm, should be between 0 and 40
void setHeight(LegHandle leg,float height); // in mm, should be negativ between -80 and -135

void setTurn(int fac);


#ifdef __cplusplus
}
#endif