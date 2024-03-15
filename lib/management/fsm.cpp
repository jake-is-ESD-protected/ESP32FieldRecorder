/**
 * @file fsm.cpp
 * @author jake-is-ESD-protected
 * @date 2024-03-14
 * @brief Finite state machine describing the device at any given time
 *
 * 
 */

#include "fsm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <jescore.h>
#include "pin_defs.h"

static fsm_struct_t fsm;
static SemaphoreHandle_t fsm_mutex;

void fsm_init(void){
    fsm.mode = e_fsm_idle;
    fsm.prev_mode = fsm.mode;
    fsm_mutex = xSemaphoreCreateMutex();
}


void fsm_handle_button(void* p){
    static TickType_t last_ts = 0;
    char* args = get_args();
    TickType_t ts = *(TickType_t*)get_param();
    if(strcmp(args, FSM_HANDLE_BUTTON_SWITCH_ARG_PRESS) == 0){
        
    }
    else if(strcmp(args, FSM_HANDLE_BUTTON_SWITCH_ARG_RELEASE) == 0){
        if(fsm_get_mode() == e_fsm_record) { 
            fsm_set_mode(e_fsm_idle);
            gpio_set_level(PIN_LED_RED, 0); 
        }
        else { 
            fsm_set_mode(e_fsm_record); 
            gpio_set_level(PIN_LED_RED, 1);
        }
    }
    else{
        // unknown arg
    }
    last_ts = ts;
}

void fsm_handle_rotary_switch(void* p){

}

void fsm_handle_rotary_turn(void* p){

}

e_err_t fsm_set_mode(e_fsm_mode_t mode){
    if(xSemaphoreTake(fsm_mutex, portMAX_DELAY) == pdTRUE){
        fsm.prev_mode = fsm.mode;
        fsm.mode = mode;
        xSemaphoreGive(fsm_mutex);
        return e_syserr_no_err;
    }
    return e_syserr_unavailable;
}

e_fsm_mode_t fsm_get_mode(void){
    e_fsm_mode_t mode;
    if(xSemaphoreTake(fsm_mutex, portMAX_DELAY) == pdTRUE){
        mode = fsm.mode;
        xSemaphoreGive(fsm_mutex);
    }
    return mode;
}

e_fsm_mode_t fsm_get_prev_mode(void){
    e_fsm_mode_t prev_mode;
    if(xSemaphoreTake(fsm_mutex, portMAX_DELAY) == pdTRUE){
        prev_mode = fsm.prev_mode;
        xSemaphoreGive(fsm_mutex);
    }
    return prev_mode;
}
