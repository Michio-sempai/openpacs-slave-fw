#include "driver/uart.h"
#include "esp_log.h"
#include "osdp_port.h"

#define OSDP_UART_NUM UART_NUM_1
#define OSDP_TX_PIN 17
#define OSDP_RX_PIN 16
#define OSDP_BAUDRATE 9600
#define OSDP_TAG "OSDP_UART"


esp_err_t osdp_uart_init(void) {
    const uart_config_t cfg = {
        .baud_rate = OSDP_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        // .parity = UART_PARITY_NONE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    ESP_ERROR_CHECK(uart_param_config(OSDP_UART_NUM, &cfg));
    ESP_ERROR_CHECK(uart_set_pin(OSDP_UART_NUM, OSDP_TX_PIN, OSDP_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(OSDP_UART_NUM, 256, 256, 0, NULL, 0));
    ESP_LOGI(OSDP_TAG, "UART initialized for OSDP");
    return ESP_OK;
}

int osdp_uart_send(const uint8_t *data, int len) {
    int written = uart_write_bytes(OSDP_UART_NUM, (const char*)data, len);
    ESP_LOGD(OSDP_TAG, "TX %d bytes", written);
    return written;
}

int osdp_uart_recv(uint8_t *data, int max_len) {
    int read = uart_read_bytes(OSDP_UART_NUM, data, max_len, pdMS_TO_TICKS(10));
    if (read > 0)
        ESP_LOGD(OSDP_TAG, "RX %d bytes", read);
    return read;
}
