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
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    oled_title_screen();
    // oled_refresh_test();

    return e_syserr_no_err;
}


void ui_server(void* p){
    while(1){
        uint32_t* pmode = (uint32_t*)wait_for_notification();
        switch(*pmode){
            case e_fsm_idle:
                gpio_set_level(PIN_LED_RED, 0);
            break;
            case e_fsm_record:
                gpio_set_level(PIN_LED_RED, 1);
            break;
            case e_fsm_browse:
            break;
            case e_fsm_play:
            break;
            case e_fsm_settings:
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