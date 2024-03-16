/**
 * @file tft.cpp
 * @author jake-is-ESD-protected
 * @date 2024-03-16
 * @brief TFT display abstraction (works over UART)
 *
 * 
 */

#include "tft.h"
#include "pin_defs.h"
#include "driver/uart.h"

static QueueHandle_t uart_queue;

e_err_t tft_init(void){
    uart_config_t uart_config = {
        .baud_rate = BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    if(uart_param_config(UART_NUM, &uart_config) != ESP_OK) { return e_syserr_install; }
    if(uart_set_pin(UART_NUM, PIN_UART_TX, PIN_UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK) { return e_syserr_install; }
    if(uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 10, &uart_queue, 0) != ESP_OK) { return e_syserr_install; }
    return e_syserr_no_err;
}

void tft_server(void *p){
    char raw_str[BUF_SIZE] = {0};
    uart_event_t event;

    while(1) {
        if (xQueueReceive(uart_queue, (void *)&event, (TickType_t)portMAX_DELAY)) {
            switch (event.type) {
            case UART_DATA:
                uart_read_bytes(UART_NUM, raw_str, event.size, portMAX_DELAY);
                break;
            case UART_FIFO_OVF:
                uart_flush_input(UART_NUM);
                xQueueReset(uart_queue);
                break;
            case UART_BUFFER_FULL:
                uart_flush_input(UART_NUM);
                // handle err
                xQueueReset(uart_queue);
                break;
            case UART_BREAK:
                break;
            case UART_PARITY_ERR:
                break;
            case UART_FRAME_ERR:
                break;
            default:
                break;
            }
            
        }
    }
}