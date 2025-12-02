

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

const char html[]= "<!DOCTYPE html><html><body>"
                "<label for=\"servo0\">Servo 0:</label>"
                "<input type=\"range\" id=\"servo0\" min=\"-10\" max=\"10\"value=\"0\" oninput=\"turn(this.value)\">"
                "<button onclick=\"start()\">Start</button>"
                "<button onclick=\"stop()\">Stop</button>"
                "<script>function turn(factor) {"
                "fetch(`/turn?turn=${factor}`)"
                ".then(response => response.text())"
                ".then(data => console.log(data))"
                ".catch(err => console.error(err));}"
                "function stop(){"
                "fetch(`/stop`)"
                ".then(response => response.text())"
                ".then(data => console.log(data))"
                ".catch(err => console.error(err));}"
                "function start(){"
                "fetch(`/go`)"
                ".then(response => response.text())"
                ".then(data => console.log(data))"
                ".catch(err => console.error(err));}"
                "</script>"
                "</body></html>";