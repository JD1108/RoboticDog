#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "string.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_netif.h"
#include "driver/i2c_master.h"

#include "legWrapper.h"
#include "servoPCAWrapper.h"
#include "globals.h" 
#include "web.h"

#include "inits.h"







void walk(void *pvParameters){
    while(1){
        if(walking){
            uint8_t data[5];
            data[1]=0x00;
            data[2]=0x00;

            float an;

            //FL
            //Hip
            an=getHipAngle(legFL,timeF);
            data[0]=getReg(hipFL);
            data[3]=getOffLow(hipFL,an);
            data[4]=getOffHigh(hipFL,an);
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 5, -1));
            //Knee
            an=getKneeAngle(legFL,timeF);
            data[0]=getReg(kneeFL);
            data[3]=getOffLow(kneeFL,an);
            data[4]=getOffHigh(kneeFL,an);
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 5, -1));
            //RL
            //Hip
            an=getHipAngle(legRL,timeF);
            data[0]=getReg(hipRL);
            data[3]=getOffLow(hipRL,an);
            data[4]=getOffHigh(hipRL,an);
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 5, -1));
            //Knee
            an=getKneeAngle(legRL,timeF);
            data[0]=getReg(kneeRL);
            data[3]=getOffLow(kneeRL,an);
            data[4]=getOffHigh(kneeRL,an);
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 5, -1));
            //RR
            //Hip
            an=getHipAngle(legRR,timeF);
            data[0]=getReg(hipRR);
            data[3]=getOffLow(hipRR,an);
            data[4]=getOffHigh(hipRR,an);
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 5, -1));
            //Knee
            an=getKneeAngle(legRR,timeF);
            data[0]=getReg(kneeRR);
            data[3]=getOffLow(kneeRR,an);
            data[4]=getOffHigh(kneeRR,an);
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 5, -1));
            //FR
            //Hip
            an=getHipAngle(legFR,timeF);
            data[0]=getReg(hipFR);
            data[3]=getOffLow(hipFR,an);
            data[4]=getOffHigh(hipFR,an);
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 5, -1));
            //Knee
            an=getKneeAngle(legFR,timeF);
            data[0]=getReg(kneeFR);
            data[3]=getOffLow(kneeFR,an);
            data[4]=getOffHigh(kneeFR,an);
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 5, -1));

            
            timeF+=50.0f;
            
        }
        vTaskDelay(pdMS_TO_TICKS(50));
        
    }
}



void app_main(void)
{
    
    legInit();
    i2cInit();
    servoInit();
    wlanInit();
    xTaskCreate(walk,"Walking",4096,NULL,5,NULL);
    

}
/*
    i2cInit();
    legFL=legCreate(0.75f);
    ServoPCAHandle hip=servoCreate(125,490,0);
    ServoPCAHandle knee=servoCreate(125,490,1);

    uint8_t pwm[9];
    pwm[1]=0x00;
    pwm[2]=0x00;
    pwm[5]=0x00;
    pwm[6]=0x00;
    
    float t=0.0f;
    float kA=10.0f;
    float hA=10.0f;
    walking=true;
    
    while(walking){

        hA=getHipAngle(legFL,t);
        kA=getKneeAngle(legFL,t);
        pwm[0]=0x06;
        pwm[3]=getOffLow(hip,hA);
        pwm[4]=getOffHigh(hip,hA);
        
        ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
        pwm[0]=0x0A;
        pwm[3]=getOffLow(knee,kA);
        pwm[4]=getOffHigh(knee,kA);
        ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
        t+=20;
        vTaskDelay(pdMS_TO_TICKS(20));
        if(t>20000){walking=false;}

    }

    pwm[0]=0x06;
    pwm[3]=307&0xFF;
    pwm[4]=307>>8;
    ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
   */

