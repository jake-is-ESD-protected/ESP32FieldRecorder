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
    if(ui_init() != e_syserr_no_err) { sys_err_handler(); }
    if(fsm_init() != e_syserr_no_err){ sys_err_handler(); }
    if(i2s_init() != e_syserr_no_err){ sys_err_handler(); }

    while(1);
}

void loop(){

}