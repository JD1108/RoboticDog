

#include "esp_wifi.h"
#include "string.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_netif.h"



#include "passwords.h"
#include "web.h"

void wlanInit(){
    esp_err_t ret = nvs_flash_init();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Default-AP-Netif erstellen
    esp_netif_create_default_wifi_ap();

    // Event-Handler registrieren
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_event_handler,
        NULL,
        NULL));

    // WiFi initialisieren
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    // AP-Konfiguration
    wifi_config_t apConfig = {
        .ap.ssid = SSID,
        .ap.password = WLAN_PASSWORD,
        .ap.authmode = WIFI_AUTH_WPA_WPA2_PSK,
        .ap.max_connection = 4
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &apConfig));

    // Statische IP setzen
    esp_netif_ip_info_t ip_info;
    IP4_ADDR(&ip_info.ip, 192,168,4,1);
    IP4_ADDR(&ip_info.gw, 192,168,4,1);
    IP4_ADDR(&ip_info.netmask, 255,255,255,0);

    esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    ESP_ERROR_CHECK(esp_netif_dhcps_stop(ap_netif));
    ESP_ERROR_CHECK(esp_netif_set_ip_info(ap_netif, &ip_info));
    ESP_ERROR_CHECK(esp_netif_dhcps_start(ap_netif));

    // AP starten
    ESP_ERROR_CHECK(esp_wifi_start());

    printf("AP IP: " IPSTR "\n", IP2STR(&ip_info.ip));
}

void httpInit(){
    httpd_handle_t server = NULL;
    httpd_config_t httpConv = HTTPD_DEFAULT_CONFIG();

    ESP_ERROR_CHECK(httpd_start(&server,&httpConv));
    httpd_uri_t uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = helloWorld,
        .user_ctx  = NULL
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server,&uri));


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

esp_err_t helloWorld(httpd_req_t *req){

    const char* resp="<h1>Hello</h1><h2>World</h2>";
    httpd_resp_send(req,resp,strlen(resp));

    return ESP_OK;
}