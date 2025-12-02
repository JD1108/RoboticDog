#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "driver/i2c_master.h"
#include "legWrapper.h"

extern bool walking;
extern float time;
extern LegHandle legFL;
extern LegHandle legRR;
extern LegHandle legFR;
extern LegHandle legRL;

extern i2c_master_bus_config_t i2cConf;
extern i2c_master_bus_handle_t busHandle;
extern i2c_device_config_t devConf;
extern i2c_master_dev_handle_t devHandle;

#ifdef __cplusplus
}
#endif