#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "legWrapper.h"
#include "servoPCAWrapper.h"
#include "globals.h"
#include "debug.h" //not in final version



void init(){
    legFL=legCreate(0.75f);
    legRR=legCreate(0.5f);
    legFR=legCreate(0.25f);
    legRL=legCreate(0.0f);
}


void app_main(void)
{
    init();
    xTaskCreate(walkDebug,"walking",16384,NULL,5,NULL);
}

