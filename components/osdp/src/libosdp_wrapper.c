#include "libosdp.h"
#include "libosdp_component.h"
#include "osdp_port.h"
#include "esp_log.h"

static const char *TAG = "LIBOSDP";

static osdp_t *ctx = NULL;
static uint8_t scbk[16] = {0}; // Secure Channel key — пока не используется

static int channel_send(void *param, uint8_t *data, int len) {
    return osdp_uart_send(data, len);
}

static int channel_recv(void *param, uint8_t *data, int max_len) {
    return osdp_uart_recv(data, max_len);
}

esp_err_t libosdp_init(void) {
    osdp_uart_init();

    osdp_pd_info_t pd_info = {
        .address = 101,             // адрес PD
        .id = 0x1234,               // ID устройства
        .baud_rate = 9600,
        .flags = 0,                 // без Secure Channel
        .channel = {
            .send = channel_send,
            .recv = channel_recv,
            .flush = NULL,
            .data = NULL,
        },
    };

    ctx = osdp_pd_setup(&pd_info);
    if (!ctx) {
        ESP_LOGE(TAG, "Failed to init OSDP PD");
        return ESP_FAIL;
    }

    osdp_set_log_level(7); // максимум логов
    osdp_logger_init((osdp_logger_t)ESP_LOGI); // если библиотека позволяет переназначить

    ESP_LOGI(TAG, "OSDP PD initialized");
    return ESP_OK;
}

void libosdp_task(void *arg) {
    while (1) {
        osdp_pd_refresh(ctx);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
