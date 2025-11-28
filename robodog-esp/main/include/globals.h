#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "legWrapper.h"

extern bool walking;
extern float time;
extern LegHandle legFL;
extern LegHandle legRR;
extern LegHandle legFR;
extern LegHandle legRL;

#ifdef __cplusplus
}
#endif