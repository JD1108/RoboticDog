

#include "esp_wifi.h"
#include "string.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_netif.h"



#include "passwords.h"
#include "web.h"
#include "globals.h"
#include "legWrapper.h"

void wlanInit(){

    //nvs
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //netif
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    //wifi init
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t apConv={
        .ap.ssid=SSID,
        .ap.password=WLAN_PASSWORD,
        .ap.authmode=WIFI_AUTH_WPA3_PSK,
        .ap.max_connection=4
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP,&apConv));
    ESP_ERROR_CHECK(esp_wifi_start());   
}

void httpInit(){
    httpd_handle_t server = NULL;
    httpd_config_t httpConv = HTTPD_DEFAULT_CONFIG();

    ESP_ERROR_CHECK(httpd_start(&server,&httpConv));

    httpd_uri_t uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = indexHandler,
        .user_ctx  = NULL
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server,&uri));
    httpd_uri_t uriTurn = {
        .uri       = "/turn",
        .method    = HTTP_GET,
        .handler   = turnHandler,
        .user_ctx  = NULL
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server,&uriTurn));
    httpd_uri_t uriGo = {
        .uri       = "/go",
        .method    = HTTP_GET,
        .handler   = goHandler,
        .user_ctx  = NULL
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server,&uriGo));
    httpd_uri_t uriStop = {
        .uri       = "/stop",
        .method    = HTTP_GET,
        .handler   = stopHandler,
        .user_ctx  = NULL
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server,&uriStop));
    httpd_uri_t uriCal = {
        .uri       = "/setAngle",
        .method    = HTTP_GET,
        .handler   = calHandler,
        .user_ctx  = NULL
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server,&uriCal));


}

void wifi_event_handler(void* arg,esp_event_base_t event_base,int32_t event_id, void* event_data) 
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_START) {
        esp_netif_ip_info_t ip_info;
        esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
        esp_netif_get_ip_info(ap_netif, &ip_info);
        printf("AP IP: " IPSTR "\n", IP2STR(&ip_info.ip));
        httpInit();
    }
}

esp_err_t indexHandler(httpd_req_t *req){
    httpd_resp_send(req,html,strlen(html));
    return ESP_OK;
}
esp_err_t turnHandler(httpd_req_t *req){
    char buff[24];
    int fac=0;
    int len=httpd_req_get_url_query_len(req);
    if(len>sizeof(buff)){
        len=sizeof(buff);
    }
    if(httpd_req_get_url_query_str(req,buff,len)==ESP_OK)
    {
        char cFac[8];
        
        if(httpd_query_key_value(buff,"turn",cFac,sizeof(cFac))==ESP_OK){
            fac=atoi(cFac);
        }
    }
    setTurn(fac);
    char resp[]="OK";
    httpd_resp_send(req,resp,strlen(resp));

    return ESP_OK;
}
esp_err_t stopHandler(httpd_req_t *req){
    char resp[]="OK";
    walking=false;
    httpd_resp_send(req,resp,strlen(resp));

    return ESP_OK;
}
esp_err_t goHandler(httpd_req_t *req){
    char resp[]="OK";
    walking=true;
    httpd_resp_send(req,resp,strlen(resp));
    return ESP_OK;
}
esp_err_t calHandler(httpd_req_t *req){ // here Debug
    
    walking=false;
    char buff[24];
    int ang=90;
    int len=httpd_req_get_url_query_len(req);
    if(len>sizeof(buff)){
        len=sizeof(buff);
    }
    if(httpd_req_get_url_query_str(req,buff,len)==ESP_OK)
    {
        char cAng[8];
        
        if(httpd_query_key_value(buff,"turn",cAng,sizeof(cAng))==ESP_OK){
            ang=atoi(cAng);
            printf("jo\n");
        }
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
    calibration(ang);
    char resp[]="OK";
    httpd_resp_send(req,resp,strlen(resp));
    printf("jo\n");
    return ESP_OK;
}

const char html[]= "<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"    <meta charset=\"UTF-8\">\n"
"    <title>Robot Control</title>\n"
"    <style>\n"
"        body {\n"
"            font-family: Arial, sans-serif;\n"
"            padding: 20px;\n"
"        }\n"
"        input[type=\"range\"] {\n"
"            width: 100%;\n"
"        }\n"
"        .slider::-webkit-slider-runnable-track {\n"
"            height: 40px;\n"
"            background: #00028f;\n"
"            border-radius: 10px;\n"
"        }\n"
"        .slider::-webkit-slider-thumb {\n"
"            -webkit-appearance: none;\n"
"            appearance: none;\n"
"            width: 5%;\n"
"            height: 120%;\n"
"            background: #007bff;\n"
"            border-radius: 10px;\n"
"            margin-top: -0.5%;\n"
"        }\n"
"        .scale {\n"
"            display: flex;\n"
"            justify-content: space-between;\n"
"            width: 100%;\n"
"            margin-top: 4px;\n"
"            font-size: 14px;\n"
"        }\n"
"        .startstop {\n"
"            width: 45%;\n"
"            height: 40px;\n"
"            margin-left: 2%;\n"
"            margin-right: 2%;\n"
"            font-size: larger;\n"
"            color: white;\n"
"            background: #00028f;\n"
"            border-radius: 10px;\n"
"        }\n"
"        .calB {\n"
"            width: 30%;\n"
"            height: 40px;\n"
"            margin-left: 1.5%;\n"
"            margin-right: 1.5%;\n"
"            font-size: larger;\n"
"            color: white;\n"
"            background: #00028f;\n"
"            border-radius: 10px;\n"
"        }\n"
"        input[type=\"checkbox\"] {\n"
"            height: 25px;\n"
"            width: 25px;\n"
"        }\n"
"        .hidden {\n"
"            display: none;\n"
"        }\n"
"        .visible {\n"
"            display: inline-block;\n"
"        }\n"
"    </style>\n"
"</head>\n"
"<body>\n"
"    <label for=\"turn\">Turn:</label><br>\n"
"    <input type=\"range\" class=\"slider\" id=\"turn\" min=\"-10\" max=\"10\" value=\"0\" oninput=\"turn(this.value)\">\n"
"    <div class=\"scale\">\n"
"        <span>left</span>\n"
"        <span>0</span>\n"
"        <span>right</span>\n"
"    </div>\n"
"    <br><br>\n"
"    <button class=\"startstop\" onclick=\"start()\">Start</button>\n"
"    <button class=\"startstop\" onclick=\"stop()\">Stop</button>\n"
"    <br><br>\n"
"    <label for=\"turn\">Calibration:</label><br>\n"
"    <input id=\"cal\" type=\"checkbox\" onclick=\"calStart()\"><br>\n"
"    <button id=\"a90\" class=\"calB hidden\" onclick=\"setAngle(90)\">All 90 Degree</button>\n"
"    <button id=\"a10\" class=\"calB hidden\" onclick=\"setAngle(10)\">All 10 Degree</button>\n"
"    <button id=\"a170\" class=\"calB hidden\" onclick=\"setAngle(170)\">All 170 Degres</button>\n"
"    <script>\n"
"        function turn(factor) {\n"
"            fetch(`/turn?turn=${factor}`)\n"
"                .then(response => response.text())\n"
"                .then(data => console.log(data))\n"
"                .catch(err => console.error(err));\n"
"        }\n"
"        function stop() {\n"
"            fetch(`/stop`)\n"
"                .then(response => response.text())\n"
"                .then(data => console.log(data))\n"
"                .catch(err => console.error(err));\n"
"        }\n"
"        function start() {\n"
"            fetch(`/go`)\n"
"                .then(response => response.text())\n"
"                .then(data => console.log(data))\n"
"                .catch(err => console.error(err));\n"
"        }\n"
"        function calStart() {\n"
"            const isChecked = document.getElementById(\"cal\").checked;\n"
"            const a90 = document.getElementById(\"a90\");\n"
"            const a10 = document.getElementById(\"a10\");\n"
"            const a170 = document.getElementById(\"a170\");\n"
"            if(isChecked){\n"
"                a90.classList.remove(\"hidden\"); a90.classList.add(\"visible\");\n"
"                a10.classList.remove(\"hidden\"); a10.classList.add(\"visible\");\n"
"                a170.classList.remove(\"hidden\"); a170.classList.add(\"visible\");\n"
"            } else {\n"
"                a90.classList.remove(\"visible\"); a90.classList.add(\"hidden\");\n"
"                a10.classList.remove(\"visible\"); a10.classList.add(\"hidden\");\n"
"                a170.classList.remove(\"visible\"); a170.classList.add(\"hidden\");\n"
"            }\n"
"        }\n"
"        function setAngle(factor) {\n"
"            fetch(`/setAngle?angle=${factor}`)\n"
"                .then(response => response.text())\n"
"                .then(data => console.log(data))\n"
"                .catch(err => console.error(err));\n"
"        }\n"
"    </script>\n"
"</body>\n"
"</html>\n";