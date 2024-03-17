/**
 * @file ui.h
 * @author jake-is-ESD-protected
 * @date 2024-03-14
 * @brief User interface server
 *
 * 
 */

#ifndef _UI_H_
#define _UI_H_

#include "sys_err.h"
#include <inttypes.h>

#define UI_SERVER_JOB_NAME  "uiserver"
#define UI_BOOT_DELAY       2000

typedef enum {
    ui_act_none,
    ui_act_gpio,
    ui_act_new_bitmap,
    ui_act_update_bmp_db,
    ui_act_update_bmp_bat,
    ui_act_update_bmp_file,
    ui_act_update_bmp_time,
    NUM_UI_ACTIONS
} ui_act_t;

typedef union {
    struct {
        uint8_t action;     // this will hold a (uint8_t)ui_act_t
        int16_t data;
    } field;
    void* raw;
} ui_cmd_t;

e_err_t ui_init(void);

void ui_server(void* p);

void ui_button_pressed(uint32_t ts);

void ui_button_released(uint32_t ts);

void ui_rotary_switch_pressed(uint32_t ts);

void ui_rotary_switch_released(uint32_t ts);

#endif // _UI_H_