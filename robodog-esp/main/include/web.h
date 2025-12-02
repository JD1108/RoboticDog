#pragma once



void wlanInit();
void httpInit();
void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id,void* event_data);
esp_err_t indexHandler(httpd_req_t *req);
esp_err_t turnHandler(httpd_req_t *req);
esp_err_t stopHandler(httpd_req_t *req);
esp_err_t goHandler(httpd_req_t *req);

extern const char html[];