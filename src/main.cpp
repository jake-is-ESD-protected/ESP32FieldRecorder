#include <Arduino.h>
#include "jescore.h"
#include "i2s_io.h"
#include "exti.h"
#include "sys_err.h"
#include "fsm.h"
#include "ui.h"

void setup(){
    if(jes_init() != e_err_no_err) { sys_err_handler(); }

    if(exti_init() != e_syserr_no_err) { sys_err_handler(); }
    fsm_init();
    if(ui_init() != e_syserr_no_err) { sys_err_handler(); }

    if(register_job(UI_SERVER_JOB_NAME, 4096, 1, ui_server, 1) != e_err_no_err) { sys_err_handler(); }
    launch_job(UI_SERVER_JOB_NAME);

    if(register_job(FSM_HANDLE_BUTTON_JOB_NAME, 2048, 1, fsm_handle_button, 0) != e_err_no_err) { sys_err_handler(); }
    if(register_job(FSM_HANDLE_ROTARY_SWITCH_JOB_NAME, 2048, 1, fsm_handle_rotary_switch, 0) != e_err_no_err) { sys_err_handler(); }
    if(register_job(FSM_HANDLE_ROTARY_TURN_JOB_NAME, 2048, 1, fsm_handle_rotary_turn, 0) != e_err_no_err) { sys_err_handler(); }

    if(register_job("i2sinit", I2S_TASK_INIT_MEM_SIZE, I2S_TASK_INIT_PRIORITY, i2s_init, false) != e_err_no_err) { sys_err_handler(); }
    if(register_job("i2srunin", I2S_TASK_RUN_MEM_SIZE, I2S_TASK_RUN_PRIORITY, i2s_run_in, true) != e_err_no_err) { sys_err_handler(); }
    if(register_job("i2srunout", I2S_TASK_RUN_MEM_SIZE, I2S_TASK_RUN_PRIORITY, i2s_run_out, true) != e_err_no_err) { sys_err_handler(); }

    launch_job("i2sinit");
    delay(100);
    launch_job("i2srunin");
    launch_job("i2srunout");
    while(1);
}

void loop(){

}