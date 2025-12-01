#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "string.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_netif.h"

#include "legWrapper.h"
#include "servoPCAWrapper.h"
#include "globals.h"
#include "passwords.h"
#include "web.h"






void init(){
    legFL=legCreate(0.75f);
    legRR=legCreate(0.5f);
    legFR=legCreate(0.25f);
    legRL=legCreate(0.0f);
}


void app_main(void)
{
    wlanInit();
   // httpInit();
    //calibration();
}

