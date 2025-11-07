#pragma once
#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

esp_err_t osdp_uart_init(void);
int osdp_uart_send(const uint8_t *data, int len);
int osdp_uart_recv(uint8_t *data, int max_len);
