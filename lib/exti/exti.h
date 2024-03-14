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

e_err_t __exti_init_pin(gpio_int_type_t edge, gpio_num_t pin, void(*handler)(void*),gpio_pullup_t pullup);

e_err_t exti_init(void);

void IRAM_ATTR __exti_handler(void* p);

#endif // _EXTI_H_