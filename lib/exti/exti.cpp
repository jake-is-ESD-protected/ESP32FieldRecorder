/**
 * @file exti.cpp
 * @author jake-is-ESD-protected
 * @date 2024-03-13
 * @brief External interrupt handling (user buttons)
 *
 * 
 */

#include "exti.h"
#include "pin_defs.h"
#include <jescore.h>

uint32_t last_interrupt_time_rec = 0;
uint32_t last_interrupt_time_select = 0;

TickType_t pin_button_press_ts = 0;
TickType_t pin_button_release_ts = 0;
TickType_t pin_rotary_switch_press_ts = 0;
TickType_t pin_rotary_switch_release_ts = 0;
TickType_t pin_rotary_A_ts = 0;
TickType_t pin_rotary_B_ts = 0;

e_err_t __exti_init_pin(gpio_int_type_t edge, gpio_num_t pin, void(*handler)(void*),gpio_pullup_t pullup){
    gpio_config_t io_conf;
    io_conf.intr_type = edge;
    io_conf.pin_bit_mask = 1ULL << (uint32_t)pin;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = pullup;
    if(gpio_config(&io_conf) != ESP_OK) { return e_syserr_param; }
    if(gpio_isr_handler_add(pin, handler, (void*)pin) != ESP_OK) { return e_syserr_install; }
    return e_syserr_no_err;
}


e_err_t exti_init(void){
    e_err_t stat;
    if(gpio_install_isr_service(0) != ESP_OK) { return e_syserr_install; }
    if((stat = __exti_init_pin(GPIO_INTR_ANYEDGE, PIN_BUTTON, __exti_handler, GPIO_PULLUP_DISABLE)) != e_syserr_no_err) { return stat; }
    if((stat = __exti_init_pin(GPIO_INTR_ANYEDGE, PIN_ROTARY_SWITCH, __exti_handler, GPIO_PULLUP_DISABLE)) != e_syserr_no_err) { return stat; }
    if((stat = __exti_init_pin(GPIO_INTR_NEGEDGE, PIN_ROTARY_A, __exti_handler, GPIO_PULLUP_DISABLE)) != e_syserr_no_err) { return e_syserr_install; }
    if((stat = __exti_init_pin(GPIO_INTR_NEGEDGE, PIN_ROTARY_B, __exti_handler, GPIO_PULLUP_DISABLE)) != e_syserr_no_err) { return e_syserr_install; }
    
    return e_syserr_no_err;
}


void IRAM_ATTR __exti_handler(void* p){
    uint32_t pin = (uint32_t)p;
    TickType_t now = xTaskGetTickCountFromISR();
    switch((gpio_num_t)pin){
        case PIN_BUTTON:
        break;
        case PIN_ROTARY_SWITCH:
        break;
        case PIN_ROTARY_A:
        break;
        case PIN_ROTARY_B:
        break;
        default:
        break;
    }
}
