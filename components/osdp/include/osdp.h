#pragma once
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t libosdp_init(void);
void libosdp_task(void *arg);

#ifdef __cplusplus
}
#endif
