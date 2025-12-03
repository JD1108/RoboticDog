#include <cstdint>
#include <iostream>
#include <math.h>

extern "C"{
    #include "driver/i2c_master.h"
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
}


#include "servoPCA.hpp"
#include "globals.h"

ServoPCA::ServoPCA(int low, int high, int chanel){
    if(low>124&&low<300){
        cal10=low;
    }else{
        cal10=125;
    }
    if(high<491&&high>300){
        cal170=high;
    }
    else{
        cal170=490;
    }
    if(chanel>15 || chanel<0) //impossible chanel will route to the last chanel which is not used in this project
    {
        chanel=15;
    }
    reg=0x06+4*chanel;
}
//private
uint16_t ServoPCA::getWidth(float angle)const{
    uint16_t width;
    if(angle<=170&&angle>=10){
        angle-=10;
        width=cal10+(angle/160)*(cal170-cal10);
    }
    else{
        width=cal10+0.5*(cal170-cal10);
    }
    return width;
}
//public
uint8_t ServoPCA::getOffLow(float angle)const{
    uint16_t width=getWidth(angle);
    return width & 0xFF;
}
uint8_t ServoPCA::getOffHigh(float angle)const{
    uint16_t width=getWidth(angle);
    return width>>8;
}
uint8_t ServoPCA::getReg()const{
    return reg;
}

void ServoPCA::calibration(int angle){

    int pwm=103+(int)round(angle*(409.0f/180.0f));

    uint8_t data[5];
    data[1]=0x00;
    data[2]=0x00;
    data[3]=pwm&0xFF;
    data[4]=pwm>>8;

    for(int i=0;i<8;i++){
        data[0]=0x06+4*i;
        ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 5, -1));
    }
}

/*
    i2c_master_bus_config_t i2cConf={};
    i2cConf.clk_source = I2C_CLK_SRC_DEFAULT;
    i2cConf.i2c_port = I2C_NUM_0;
    i2cConf.scl_io_num = static_cast<gpio_num_t>(22);
    i2cConf.sda_io_num = static_cast<gpio_num_t>(21);
    i2cConf.glitch_ignore_cnt = 7;
    i2cConf.flags.enable_internal_pullup = true;

    i2c_master_bus_handle_t busHandle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2cConf,&busHandle));

    i2c_device_config_t devConf={};
    devConf.dev_addr_length=I2C_ADDR_BIT_LEN_7;
    devConf.device_address=0x40;
    devConf.scl_speed_hz=400000;

    i2c_master_dev_handle_t devHandle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(busHandle,&devConf,&devHandle));

    uint8_t data[2];  

    data[0] = 0x00;   
    data[1] = 0x10;   //sleep

    ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 2, -1));

    data[0] = 0xFE;   
    data[1] = static_cast<uint8_t>((25000000 / (4096 * 50)) - 1);
    ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 2, -1));

    data[0] = 0x00;   
    data[1] = 0x20;   //unsleep and autoincremment

    ESP_ERROR_CHECK(i2c_master_transmit(devHandle, data, 2, -1));
    vTaskDelay(pdMS_TO_TICKS(500));

    int i=0;
    uint16_t low[16];
    uint16_t high[16];

    uint8_t pwm[5];
    pwm[0]=0x06;
    pwm[1]=0x00;
    pwm[2]=0x00;
    pwm[3]=307&0xFF;
    pwm[4]=307>>8;
    ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
    

    for(int i=100;i>0;i--){
        int val=215+0.92*i;
        pwm[3]=val&0xFF;
        pwm[4]=val>>8;
        ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    for(int i=0;i<200;i++){
        int val=215+0.965*i;
        pwm[3]=val&0xFF;
        pwm[4]=val>>8;
        ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    for(int i=100;i>0;i--){
        int val=307+0.93*i;
        pwm[3]=val&0xFF;
        pwm[4]=val>>8;
        ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    /*
    while(i<16){
        std::cout<<"Connect servo to channel 0 the PCA9685."<<std::endl
        <<"Press a ENTER to start calibration...";
        dummy = readLineUART();
        std::string input;
        int inputI=0;
        uint16_t tempLow=250;
        uint16_t tempHigh=350;
        uint8_t pwm[5];
        pwm[0]=0x06;
        pwm[1]=0x00;
        pwm[2]=0x00;
        pwm[3]=307&0xFF;
        pwm[4]=307>>8;
        ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
        std::cout<<i+1<<". Servo at 90 degree (pwm-value:307)"<<std::endl<<"Press Enter to continue...";
        dummy = readLineUART();
        while(1){
            pwm[3]=tempLow&0xFF;
            pwm[4]=tempLow>>8;
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
            std::cout<<"This is pwm-value"<<tempLow<< ". How much do you want to lower?"<<std::endl
                <<"0 = end"<<std::endl
                <<"Plese enter number:";
            input = readLineUART();
            inputI=std::atoi(input.c_str());
            if(inputI==0){break;}
            tempLow-=inputI;
        }
        low[i]=tempLow;
        std::cout<<"Press Enter to continue with upper bound...";
        dummy = readLineUART();
        while(1){
            pwm[3]=tempHigh&0xFF;
            pwm[4]=tempHigh>>8;
            ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));
            std::cout<<"This is pwm-value"<<tempHigh<< ". How much do you want to higher?"<<std::endl
                <<"0 = end"<<std::endl
                <<"Plese enter number:";
            input = readLineUART();
            inputI=std::atoi(input.c_str());
            if(inputI==0){break;}
            tempHigh+=inputI;
        }
        high[i]=tempHigh;
        
        do{
            std::cout<<"Calibrate another servo?"<<std::endl<<"y/n"<<std::endl;
            input = readLineUART();

        }while(input[0]!='y'&&input[0]!='Y'&&input[0]!='n'&&input[0]!='N');
        
        if(input[0]=='n'||input[0]=='N'){break;}
        i++;
        pwm[3]=0x00;
        pwm[4]=0x00;
        ESP_ERROR_CHECK(i2c_master_transmit(devHandle, pwm, 5, -1));

    }
    std::cout<<"Servo \t"<<"Low\t"<<"High"<<std::endl;
    for(int j=0;j<=i;j++){
        std::cout<<j+1<<"\t"<<low[j]<<"\t"<<high[j]<<std::endl;
    }
    std::cout<<std::endl<<"Finish with ENTER...";
    dummy = readLineUART();
    */
