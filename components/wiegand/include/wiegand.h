#ifndef WIEGAND_H
#define WIEGAND_H

#include <stdint.h>
#include "esp_err.h"
#define WIEGAND_MAX_BITS 128
#define WIEGAND_MAX_BYTES (WIEGAND_MAX_BITS / 8)

typedef enum {
    IN = 0,
    OUT = 1
} WIEGAND_INTERFACE_MODE

typedef struct wiegand
{
    /* data */
};



// Типы поддерживаемых форматов Wiegand
typedef enum {
    WIEGAND_TYPE_UNKNOWN = 0,
    WIEGAND_TYPE_W8,
    WIEGAND_TYPE_W26,
    WIEGAND_TYPE_W34,
    WIEGAND_TYPE_W58,
} WiegandType;

// Структура Wiegand-кадра
typedef struct {
    uint8_t data[WIEGAND_MAX_BYTES]; // общий буфер для хранения данных
    WiegandType type;
} WiegandData;

// Получение количества чистых битов данных
uint8_t wiegand_data_payload_count(const WiegandData *wiegand);

// Получение общего количества битов в Wiegand-кадре (включая биты чётности)
uint8_t wiegand_data_total_bits(const WiegandData *wiegand);

// Вычисление битов чётности для заданного Wiegand-кадра
// Возвращает 2 бита: старший — leading, младший — trailing
uint8_t wiegand_data_calculate_parity(const WiegandData *wiegand);

// Получение бита по его индексу
static inline uint8_t get_bit(const uint8_t *data, uint8_t bit_index) {
    return (data[bit_index / 8] >> (7 - (bit_index % 8))) & 0x01;
}


#endif // WIEGAND_H