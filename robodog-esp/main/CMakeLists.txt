set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
idf_component_register(
    SRCS 
        "main.c"
        "src/leg.cpp"
        "src/pca9685.cpp" 
        "src/legWrapper.cpp"
        "src/globals.c"
        "src/pca9685Wrapper.cpp"
        "src/debug.c"

    INCLUDE_DIRS 
        "include"
    REQUIRES 
        driver 
        esp_timer 
        esp_event
    )
