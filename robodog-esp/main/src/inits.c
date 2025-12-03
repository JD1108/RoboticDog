#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "inits.h"
#include "globals.h"
#include "cal.h"

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
void servoInit(){

    hipFL=servoCreate(LOW0,HIGH0,0);
    kneeFL=servoCreate(LOW1,HIGH1,1);
    hipRL=servoCreate(LOW2,HIGH2,2);
    kneeRL=servoCreate(LOW3,HIGH3,3);
    hipRR=servoCreate(LOW4,HIGH4,4);
    kneeRR=servoCreate(LOW5,HIGH5,5);
    hipFR=servoCreate(LOW6,HIGH6,6);
    kneeFR=servoCreate(LOW7,HIGH7,7);
    
}