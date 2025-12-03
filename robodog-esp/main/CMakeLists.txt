set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
idf_component_register(
    SRCS 
        "main.c"
        "src/leg.cpp"
        "src/servoPCA.cpp" 
        "src/legWrapper.cpp"
        "src/globals.c"
        "src/servoPCAWrapper.cpp"
        "src/web.c"
        "src/inits.c"
    INCLUDE_DIRS 
        "include"
    REQUIRES 
        driver 
        esp_timer 
        esp_event
        esp_wifi
        nvs_flash
        esp_http_server
    )
