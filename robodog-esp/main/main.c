#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "legWrapper.h"
#include "pca9685Wrapper.h"

bool walking=false;
LegHandle legFL;
LegHandle legRR;
LegHandle legFR;
LegHandle legRL;

void init(){
    legFL=legCreate(0.75f);
    legRR=legCreate(0.5f);
    legFR=legCreate(0.25f);
    legRL=legCreate(0.0f);
}
void walk(void *pvParameters){
    walking=true;
    float time=0.0f;
    while(walking){
        float an[8];
        an[0]=getHipAngle(legFL,time);
        an[1]=getKneeAngle(legFL,time);
        an[2]=getHipAngle(legRR,time);
        an[3]=getKneeAngle(legRR,time);
        an[4]=getHipAngle(legFR,time);
        an[5]=getKneeAngle(legFR,time);
        an[6]=getHipAngle(legRL,time);
        an[7]=getKneeAngle(legRL,time);

        printf("<start>");

        for(int i=0;i<8;i++){
            printf("%.2f",an[i]);
            if(i<7){
                printf(";");
            }
            
        }

        printf("<end>\n");

        time+=50.0f;
        vTaskDelay(50/portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    init();
    xTaskCreate(walk,"walking",16384,NULL,5,NULL);
}

