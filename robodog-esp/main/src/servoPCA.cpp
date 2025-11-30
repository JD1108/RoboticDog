#include <cstdint>
#include <iostream>

#include "servoPCA.hpp"

ServoPCA::ServoPCA(int low, int high, int chanel){
    if(low>210&&low<300){
        cal10=low;
    }else{
        cal10=215;
    }
    if(high<404&&high>300){
        cal170=high;
    }
    else{
        cal170=400;
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
        width=cal10+(angle/170)*(cal170-cal10);
    }
    else{
        width=cal10+0.5*(cal170-cal10);
    }
    return width;
}
//public
uint8_t ServoPCA::getOffLow(uint16_t width)const{
    return width & 0xFF;
}
uint8_t ServoPCA::getOffHigh(uint16_t width)const{
    return width>>8;
}
uint8_t ServoPCA::getReg()const{
    return reg;
}
void ServoPCA::calibration(){
    std::cout<<"Connect servo to channel 0 the PCA9685."
        <<"Press a ENTER to start calibration...";
    std::cin.get();

    int i=0;
    uint8_t low[16];
    uint8_t high[16];
    while(i<16){
        char input=' ';
        int inputI=0;
        uint8_t tempLow=250;
        uint8_t tempHigh=350;
        std::cout<<i+1<<". Servo at 90 degree (pwm-value:307)"<<std::endl<<"Press Enter to continue...";
        std::cin.get();
        while(1){
            std::cout<<"This is pwm-value"<<tempLow<< ". How much do you want to lower?"<<std::endl
                <<"0 = end"<<std::endl
                <<"Plese enter number:";
            std::cin>>inputI;
            if(inputI==0){break;}
            tempLow-=inputI;
        }
        low[i]=tempLow;
        std::cout<<"Press Enter to continue with upper bound...";
        std::cin.get();
        while(1){
            std::cout<<"This is pwm-value"<<tempHigh<< ". How much do you want to higher?"<<std::endl
                <<"0 = end"<<std::endl
                <<"Plese enter number:";
            std::cin>>inputI;
            if(inputI==0){break;}
            tempHigh+=inputI;
        }
        high[i]=tempHigh;
        
        do{
            std::cout<<"Calibrate another servo?"<<std::endl<<"y/n"<<std::endl;
            std::cin>>input;

        }while(input!='y'&&input!='Y'&&input!='n'&&input!='N');
        
        if(input=='N'||input=='N'){break;}
        i++;
    }
    std::cout<<"Servo \t"<<"Low\t"<<"High"<<std::endl;
    for(int j=0;j<=i;j++){
        std::cout<<j+1<<"\t"<<low[j]<<"\t"<<high[j]<<std::endl;
    }
    std::cout<<std::endl<<"Finish with ENTER...";
    std::cin.get();


}
