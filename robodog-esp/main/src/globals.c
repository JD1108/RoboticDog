#include <stddef.h>

#include "globals.h"

bool walking = false;
float time = 0.0f;
LegHandle legFL = NULL;
LegHandle legRR = NULL;
LegHandle legFR = NULL;
LegHandle legRL = NULL;

i2c_master_bus_config_t i2cConf={};
i2c_master_bus_handle_t busHandle;
i2c_device_config_t devConf={};
i2c_master_dev_handle_t devHandle;


