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
#include "fsm.h"

TickType_t pin_button_ts = 0;
TickType_t pin_rotary_switch_ts = 0;
TickType_t pin_rotary_A_ts = 0;
TickType_t pin_rotary_B_ts = 0;

e_err_t __exti_init_pin(gpio_int_type_t edge, gpio_num_t pin, void(*handler)(void*),gpio_pullup_t pullup){
    gpio_config_t io_conf;
    io_conf.intr_type = edge;
    io_conf.pin_bit_mask = 1ULL << (uint32_t)pin;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = pullup;
    if(gpio_config(&io_conf) != ESP_OK) { return e_syserr_param; }
    exti_context_t ctxt;
    ctxt.context.pin = (uint16_t)pin;
    ctxt.context.edge = (uint16_t)edge;
    if(gpio_isr_handler_add(pin, handler, (void*)ctxt.raw) != ESP_OK) { return e_syserr_install; }
    return e_syserr_no_err;
}


e_err_t exti_init(void){
    e_err_t stat;
    if(gpio_install_isr_service(0) != ESP_OK) { return e_syserr_install; }
    if((stat = __exti_init_pin(GPIO_INTR_NEGEDGE, PIN_BUTTON, __exti_handler, GPIO_PULLUP_DISABLE)) != e_syserr_no_err) { return stat; }
    if((stat = __exti_init_pin(GPIO_INTR_POSEDGE, PIN_BUTTON, __exti_handler, GPIO_PULLUP_DISABLE)) != e_syserr_no_err) { return stat; }
    if((stat = __exti_init_pin(GPIO_INTR_NEGEDGE, PIN_ROTARY_SWITCH, __exti_handler, GPIO_PULLUP_DISABLE)) != e_syserr_no_err) { return stat; }
    if((stat = __exti_init_pin(GPIO_INTR_POSEDGE, PIN_ROTARY_SWITCH, __exti_handler, GPIO_PULLUP_DISABLE)) != e_syserr_no_err) { return stat; }
    if((stat = __exti_init_pin(GPIO_INTR_NEGEDGE, PIN_ROTARY_A, __exti_handler, GPIO_PULLUP_ENABLE)) != e_syserr_no_err) { return stat; }
    if((stat = __exti_init_pin(GPIO_INTR_NEGEDGE, PIN_ROTARY_B, __exti_handler, GPIO_PULLUP_ENABLE)) != e_syserr_no_err) { return stat; }
    return e_syserr_no_err;
}


void IRAM_ATTR __exti_handler(void* p){
    exti_context_t ctxt;
    ctxt.raw = p;
    gpio_num_t pin = (gpio_num_t)ctxt.context.pin;
    gpio_int_type_t edge = (gpio_int_type_t)ctxt.context.edge;
    TickType_t now = xTaskGetTickCountFromISR();
    const char* handler_name = NULL;
    const char* args = NULL;
    switch(pin){
        case PIN_BUTTON:
            if(now - pin_button_ts > EXTI_BUTTON_DEBOUNCE_TICKS){
                handler_name = FSM_HANDLE_BUTTON_JOB_NAME;
                if(ctxt.context.edge == GPIO_INTR_NEGEDGE){
                    args = FSM_HANDLE_BUTTON_SWITCH_ARG_PRESS;
                }
                else{
                    args = FSM_HANDLE_BUTTON_SWITCH_ARG_RELEASE;
                }
                pin_button_ts = now;
            }
            
        break;
        case PIN_ROTARY_SWITCH:
            if(now - pin_rotary_switch_ts > EXTI_BUTTON_DEBOUNCE_TICKS){
                handler_name = FSM_HANDLE_ROTARY_SWITCH_JOB_NAME;
                if(ctxt.context.edge == GPIO_INTR_NEGEDGE){
                    args = FSM_HANDLE_BUTTON_SWITCH_ARG_PRESS;
                }
                else{
                    args = FSM_HANDLE_BUTTON_SWITCH_ARG_RELEASE;
                }
                pin_rotary_switch_ts = now;
            }
            
        break;
        case PIN_ROTARY_A:
            handler_name = FSM_HANDLE_ROTARY_TURN_JOB_NAME;
            args = FSM_HANDLE_ROTARY_TURN_ARG_LEFT;
        break;
        case PIN_ROTARY_B:
            handler_name = FSM_HANDLE_ROTARY_TURN_JOB_NAME;
            args = FSM_HANDLE_ROTARY_TURN_ARG_RIGHT;
        break;
        default:
        break;
    }
    if(handler_name != NULL) { 
        job_struct_t* pj_to_to = __job_get_job_by_name(handler_name);
        job_struct_t* pj_core = __job_get_job_by_name("core"); // this is a bit wonky
        __job_set_args((char*)args, pj_to_to);
        __job_set_param(&now, pj_to_to);
        __job_notify_with_job(pj_core, pj_to_to, 1);
    }
}
