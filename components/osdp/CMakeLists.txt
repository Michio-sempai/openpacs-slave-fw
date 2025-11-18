idf_component_register(
    SRCS
        "src/libosdp_wrapper.c"
        "port/osdp_channel_uart.c"
    INCLUDE_DIRS
        "include"
        "port"
    REQUIRES driver
)

# Добавляем исходники оригинальной библиотеки
set(LIBOSDP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libosdp)

file(
    GLOB LIBOSDP_SOURCES
        ${LIBOSDP_DIR}/src/*.c
        ${LIBOSDP_DIR}/utils/*.c
    )
target_sources(
    ${COMPONENT_LIB} PRIVATE
    ${LIBOSDP_SOURCES}
    )

target_include_directories(
    ${COMPONENT_LIB} PUBLIC
    ${LIBOSDP_DIR}/include
    ${LIBOSDP_DIR}/utils/include    
    )

# ⚙️ Переназначаем внутренние макросы логирования libosdp на ESP_LOGx
target_compile_definitions(
    ${COMPONENT_LIB} PRIVATE
    osdp_log_info=ESP_LOGI
    osdp_log_error=ESP_LOGE
    osdp_log_debug=ESP_LOGD
    osdp_log_warn=ESP_LOGW
    )
