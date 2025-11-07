#ifndef WIEGAND_IO_H
#define WIEGAND_IO_H
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wiegand.h"
#include <stdint.h>

// Настройка отправки битов чётности при передаче данных по Wiegand
typedef enum {
    PARITY_BOTH = 0,        // Оба бита чётности
    PARITY_LEADING,         // Только ведущий бит чётности
    PARITY_TRAILING,        // Только завершающий бит чётности
    PARITY_NONE,            // Без битов чётности
} SentWiegandParityMode;

typedef struct {
    SentWiegandParityMode parity_mode; // Что делать с битами чётности
    uint16_t pulse_width_us;       // Длительность импульса (по умолчанию 50 мкс)
    uint16_t pulse_gap_us;         // Интервал между битами (по умолчанию 1000 мкс)
    uint16_t pulse_gap_after_send; // Пауза после отправки кадра (по умолчанию 2000 мкс)
} WiegandInterfaceConfig;


// Интерфейс для одного физического канала
typedef struct {
    uint8_t id;                                     // Идентификатор интерфейса, для отладки
    WiegandInterfaceConfig config;                  // Конфигурация интерфейса
    gpio_num_t data0_gpio;                          // GPIO для D0
    gpio_num_t data1_gpio;                          // GPIO для D1
    void *ctx;                                      // произвольный пользовательский контекст

} WiegandInterface;

extern const WiegandInterfaceConfig default_wiegand_config;


// Отправка кадра через конкретный IO-интерфейс
esp_err_t wiegand_interface_send_data(const WiegandInterface *io, const WiegandData *frame);

esp_err_t wiegand_interface_init(const WiegandInterface *iface);



#endif // WIEGAND_IO_H
