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
#include "passwords.h"
#include "web.h"
#include "debug.h"






void legInit(){
    legFL=legCreate(0.75f);
    legRR=legCreate(0.5f);
    legFR=legCreate(0.25f);
    legRL=legCreate(0.0f);
}
void i2cInit(){
    i2cConf.clk_source = I2C_CLK_SRC_DEFAULT;
    i2cConf.i2c_port = I2C_NUM_0;
    i2cConf.scl_io_num = 22;
    i2cConf.sda_io_num = 21;
    i2cConf.glitch_ignore_cnt = 7;
    i2cConf.flags.enable_internal_pullup =false;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2cConf,&busHandle));
    
    devConf.dev_addr_length=I2C_ADDR_BIT_LEN_7;
    devConf.device_address=0x40;
    devConf.scl_speed_hz=10000;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(busHandle,&devConf,&devHandle));

    vTaskDelay(pdMS_TO_TICKS(50));
    uint8_t data[2];
    data[0] = 0x00;   
    data[1] = 0x10;   //sleep
    ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 2, -1));
    
    printf("e\n");//IIIIIIIIIIIIIIIIIIII
    uint8_t pre=0x79;
    data[0] = 0xFE;   
    data[1] = pre; //set prescale
    ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 2, -1));

    printf("h\n");//IIIIIIIIIIIIIIIIIIII
    data[0] = 0x00;   
    data[1] = 0x20;   //unsleep and autoincremment
    ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 2, -1));
    printf("i\n");//IIIIIIIIIIIIIIIIIIII
    vTaskDelay(pdMS_TO_TICKS(500));
}


void app_main(void)
{
    
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

    /*
    pwm[0]=0x06;
    pwm[3]=307&0xFF;
    pwm[4]=307>>8;
    ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
   
   legInit();
   wlanInit();
   xTaskCreate(walkDebug,"Walking",4096,NULL,5,NULL);
    */

}

