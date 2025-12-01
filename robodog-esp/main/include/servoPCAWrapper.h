#pragma once



#ifdef __cplusplus
extern "C"{
#endif

typedef void* ServoPCAHandle;
ServoPCAHandle servoCreate(int low, int high, int chanel);
void servoDestroy(ServoPCAHandle servo);
uint8_t getOffLow(ServoPCAHandle servo, float angle);
uint8_t getOffHigh(ServoPCAHandle servo,float angle);
uint8_t getReg(ServoPCAHandle servo);
void calibration();


#ifdef __cplusplus
}
#endif