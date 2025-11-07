#include "wiegand.h"
#include "esp_log.h"
// Получение бита по его индексу
// static inline uint8_t get_bit(const uint8_t *data, uint8_t bit_index) {
//     return (data[bit_index / 8] >> (7 - (bit_index % 8))) & 0x01;
// }
static const char *TAG = "WG";

// Получение количества чистых битов для заданного типа Wiegand
uint8_t wiegand_data_payload_count(const WiegandData *wiegand) {
    ESP_LOGD(TAG, "Start wiegand_data_payload_count()");
    switch (wiegand->type) {
        case WIEGAND_TYPE_W8: return 8;
        case WIEGAND_TYPE_W26: return 24;
        case WIEGAND_TYPE_W34: return 32;
        case WIEGAND_TYPE_W58: return 56;
        default: return 0; // Неподдерживаемый тип
    }
}
// Получение общего количества битов в Wiegand-кадре (включая биты чётности)
uint8_t wiegand_data_total_bits(const WiegandData *wiegand) {
    ESP_LOGD(TAG, "Start wiegand_data_total_bits()");
    return wiegand_data_payload_count(wiegand) + 2; // +2 для битов чётности
}

// Вычисление битов чётности для заданного Wiegand-кадра
// Возвращает 2 бита: старший — leading, младший — trailing
uint8_t wiegand_data_calculate_parity(const WiegandData *wiegand) {
    ESP_LOGD(TAG, "Start wiegand_data_calculate_parity()");

    uint8_t half = wiegand_data_payload_count(wiegand) / 2; // Находим половину количества битов в payload

    uint8_t even = 0;
    uint8_t odd = 1;

    // Leading parity: even parity по первой половине payload
    for (uint8_t i = 1; i <= half; ++i) {
        even ^= get_bit(wiegand->data, i);
    }

    // Trailing parity: odd parity по второй половине payload
    for (uint8_t i = 1 + half; i < wiegand_data_total_bits(wiegand) - 1; ++i) {
        odd ^= get_bit(wiegand->data, i);
    }

    return (even << 1) | odd;
}