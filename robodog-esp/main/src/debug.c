#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "globals.h"
#include "debug.h"

void walkDebug(void *pvParameters){
    while(1){
        if(walking){
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
            
        }
        vTaskDelay(pdMS_TO_TICKS(50));
        
    }
}