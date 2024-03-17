/**
 * @file ui.cpp
 * @author jake-is-ESD-protected
 * @date 2024-03-14
 * @brief User interface server
 *
 * 
 */

#include "ui.h"
#include <jescore.h>
#include "fsm.h"
#include "pin_defs.h"
#include "oled.h"


e_err_t ui_init(void){
    esp_err_t stat;
    gpio_pad_select_gpio(PIN_LED_RED);
    if((stat = gpio_set_direction(PIN_LED_RED, GPIO_MODE_OUTPUT)) != ESP_OK) { return e_syserr_install; }

    oled_init();
    oled_title_screen();
    if(register_and_launch_job(UI_SERVER_JOB_NAME, 4096, 1, ui_server, 1) != e_err_no_err) { return e_syserr_jes; }
    return e_syserr_no_err;
}


void ui_server(void* p){
    vTaskDelay(UI_BOOT_DELAY / portTICK_PERIOD_MS);
    oled_idle_screen();
    while(1){
        ui_cmd_t ui_cmd;
        ui_cmd.raw = wait_for_notification();
        ui_act_t action = (ui_act_t)ui_cmd.field.action;
        int16_t data = ui_cmd.field.data;
        switch(action){
            case ui_act_none:
            // something went wrong on a lower level
            break;
            case ui_act_gpio:
                gpio_set_level(PIN_LED_RED, data);
            break;
            case ui_act_new_bitmap:
            break;
            case ui_act_update_bmp_db:
            break;
            case ui_act_update_bmp_bat:
            break;
            case ui_act_update_bmp_file:
            break;
            case ui_act_update_bmp_time:
            break;
            default:
            break;
        }
    }
}


void ui_button_pressed(uint32_t ts){
    
}


void ui_button_released(uint32_t ts){

}


void ui_rotary_switch_pressed(uint32_t ts){

}


void ui_rotary_switch_released(uint32_t ts){

}