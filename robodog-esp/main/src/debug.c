#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "globals.h"
#include "debug.h"

void walkDebug(void *pvParameters){
    while(1){
        if(walking){
            float an[8];
            an[0]=getHipAngle(legFL,timeF);
            an[1]=getKneeAngle(legFL,timeF);
            an[2]=getHipAngle(legRR,timeF);
            an[3]=getKneeAngle(legRR,timeF);
            an[4]=getHipAngle(legFR,timeF);
            an[5]=getKneeAngle(legFR,timeF);
            an[6]=getHipAngle(legRL,timeF);
            an[7]=getKneeAngle(legRL,timeF);

            printf("<start>");

            for(int i=0;i<8;i++){
                printf("%.2f",an[i]);
                if(i<7){
                    printf(";");
                }
                
            }

            printf("<end>\n");

            timeF+=50.0f;
            
        }
        vTaskDelay(pdMS_TO_TICKS(50));
        
    }
}