/**
 * @file exti.h
 * @author jake-is-ESD-protected
 * @date 2024-03-13
 * @brief External interrupt handling (user buttons)
 *
 * 
 */

#ifndef _EXTI_H_
#define _EXTI_H_

#include "driver/gpio.h"
#include "sys_err.h"

#define EXTI_BUTTON_DEBOUNCE_TICKS          500
#define EXTI_ROTARY_SWITCH_DEBOUNCE_TICKS   500

typedef union {
    struct {
        uint16_t pin;
        uint16_t edge;
    } context;
    void* raw;
} exti_context_t;


e_err_t __exti_init_pin(gpio_int_type_t edge, gpio_num_t pin, void(*handler)(void*),gpio_pullup_t pullup);

e_err_t exti_init(void);

void IRAM_ATTR __exti_handler(void* p);

#endif // _EXTI_H_