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

e_err_t ui_init(void);

void ui_server(void* p);

void ui_button_pressed(uint32_t ts);

void ui_button_released(uint32_t ts);

void ui_rotary_switch_pressed(uint32_t ts);

void ui_rotary_switch_released(uint32_t ts);

#endif // _UI_H_