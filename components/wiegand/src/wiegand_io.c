#include "wiegand_io.h"
#include "esp_rom_sys.h"
#include "time.h"
#include "rom/ets_sys.h"
static const char *TAG = "WG_IO";

const WiegandInterfaceConfig default_wiegand_config = {
    .parity_mode = PARITY_BOTH,
    .pulse_width_us = 25,
    .pulse_gap_us = 205,
    .pulse_gap_after_send = 2000
};

esp_err_t wiegand_interface_init(const WiegandInterface *iface)
{
    esp_err_t err;

    if (!iface) {
        ESP_LOGE(TAG, "Invalid Wiegand interface configuration");
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGD(TAG, "Initializing Wiegand interface ID: %d", iface->id);

    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << iface->data0_gpio) | (1ULL << iface->data1_gpio)
    };

    err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure GPIOs for Wiegand interface ID: %d, error: %s", iface->id, esp_err_to_name(err));
        return err;

    }
    // Выставляем в HIGH — неактивное состояние
    err = gpio_set_level(iface->data0_gpio, 1);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Wiegand interface ID: %d. Failed to set level for D0 GPIO %d, error: %s", iface->id, iface->data0_gpio, esp_err_to_name(err));
        return err;
    }
    
    err = gpio_set_level(iface->data1_gpio, 1);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Wiegand interface ID: %d. Failed to set level for D0 GPIO %d, error: %s", iface->id, iface->data0_gpio, esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}


esp_err_t send_one_bit(const WiegandInterface *iface, bool bit)
{
    esp_err_t err;
    if (bit)
    {
        // Логическая 1 — импульс на D1
        err = gpio_set_level(iface->data1_gpio, 0);
        if (err != ESP_OK)
            {        
                ESP_LOGE(TAG, "Wiegand interface ID: %d. Failed to set level for D1 GPIO %d, error: %s", iface->id, iface->data1_gpio, esp_err_to_name(err));
                return err;
            }
        ets_delay_us(25);
        err = gpio_set_level(iface->data1_gpio, 1);
        if (err != ESP_OK)
            {
            ESP_LOGE(TAG, "Wiegand interface ID: %d. Failed to set level for D1 GPIO %d, error: %s", iface->id, iface->data1_gpio, esp_err_to_name(err));
            return err;
            }
        return ESP_OK;
    } 
    else
    {
        err = gpio_set_level(iface->data0_gpio, 0);
        if (err != ESP_OK)
            {    
            ESP_LOGE(TAG, "Wiegand interface ID: %d. Failed to set level for D0 GPIO %d, error: %s", iface->id, iface->data0_gpio, esp_err_to_name(err));
            return err;
            }
        ets_delay_us(25);
        err = gpio_set_level(iface->data0_gpio, 1);
        if (err != ESP_OK)
            {
            ESP_LOGE(TAG, "Wiegand interface ID: %d. Failed to set level for D0 GPIO %d, error: %s", iface->id, iface->data0_gpio, esp_err_to_name(err));
            return err;
            }
        return ESP_OK;
    }
}


esp_err_t wiegand_interface_send_data(const WiegandInterface *io, const WiegandData *frame) {
    ESP_LOGD(TAG, "Wiegand interface send data start");

    if (!io || !frame) {
        ESP_LOGE(TAG, "Invalid Wiegand interface or frame data");
        return ESP_ERR_INVALID_ARG;
    }
    
    uint8_t total_bits = wiegand_data_total_bits(frame);
    uint8_t parity = wiegand_data_calculate_parity(frame);

    if (io->config.parity_mode == PARITY_BOTH || io->config.parity_mode == PARITY_LEADING) {
        // Установка ведущего бита чётности, если это предполагает конфигурация интерфейса
        send_one_bit(io, (parity >> 1) & 0x01);
        ets_delay_us(205);    
    }
    
    for (uint8_t i = 0; i < total_bits; ++i) {
        // Получаем бит из кадра
        send_one_bit(io, get_bit(frame->data, i));
        ets_delay_us(205);
    }
    if (io->config.parity_mode == PARITY_TRAILING || io->config.parity_mode == PARITY_BOTH) {
        // Установка завершающего бита чётности, если это предполагает конфигурация интерфейса
        send_one_bit(io, parity & 0x01);
        ets_delay_us(205);
    }

    return ESP_OK;
}
