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
#include "ui.h"

static fsm_struct_t fsm;
static SemaphoreHandle_t fsm_mutex;

e_err_t fsm_init(void){
    fsm.mode = e_fsm_idle;
    fsm.prev_mode = fsm.mode;
    fsm_mutex = xSemaphoreCreateMutex();
    if(register_job(FSM_HANDLE_BUTTON_JOB_NAME, 2048, 1, fsm_handle_button, 0) != e_err_no_err) { return e_syserr_jes; }
    if(register_job(FSM_HANDLE_ROTARY_SWITCH_JOB_NAME, 2048, 1, fsm_handle_rotary_switch, 0) != e_err_no_err) { return e_syserr_jes; }
    if(register_job(FSM_HANDLE_ROTARY_TURN_JOB_NAME, 2048, 1, fsm_handle_rotary_turn, 0) != e_err_no_err) { return e_syserr_jes; }
    ui_cmd_t ui_cmd;
    ui_cmd.field.action = (uint8_t)ui_act_new_bitmap;
    ui_cmd.field.data = (int16_t)e_bmp_idle;
    notify_job(UI_SERVER_JOB_NAME, ui_cmd.raw);
    return e_syserr_no_err;
}


void fsm_handle_button(void* p){
    static TickType_t last_ts = 0;
    char* args = get_args();
    TickType_t ts = *(TickType_t*)get_param();
    if(strcmp(args, FSM_HANDLE_BUTTON_SWITCH_ARG_PRESS) == 0){
        
    }
    else if(strcmp(args, FSM_HANDLE_BUTTON_SWITCH_ARG_RELEASE) == 0){
        ui_cmd_t ui_cmd;
        ui_cmd.field.action = (uint8_t)ui_act_gpio;
        if(fsm_get_mode() == e_fsm_record) { 
            fsm_set_mode(e_fsm_idle);
            ui_cmd.field.data = 0;
        }
        else { 
            fsm_set_mode(e_fsm_record); 
            ui_cmd.field.data = 1;
        }
        notify_job(UI_SERVER_JOB_NAME, ui_cmd.raw);
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
