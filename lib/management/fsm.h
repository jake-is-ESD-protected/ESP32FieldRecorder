/**
 * @file fsm.h
 * @author jake-is-ESD-protected
 * @date 2024-03-14
 * @brief Finite state machine describing the device at any given time
 *
 * 
 */

#ifndef _FSM_H_
#define _FSM_H_

#include "sys_err.h"

typedef enum {
    e_fsm_idle,       // device streams audio IN and level is shown, nothing is written
    e_fsm_record,     // device streams audio IN to SD card
    e_fsm_browse,     // user scrolls through saved files
    e_fsm_play,       // device streams saved audio OUT
    e_fsm_settings,   // user configures device
    FSM_MODE_NUM
} e_fsm_mode_t;

typedef enum {
    e_fsm_evt_none,
    e_fsm_evt_timeout,
    e_fsm_evt_data_rdy,
    e_fsm_evt_button_press,
    e_fsm_evt_button_release,
    e_fsm_evt_rotary_switch_press,
    e_fsm_evt_rotary_switch_release,
    e_fsm_evt_rotary_turn_l,
    e_fsm_evt_rotary_turn_r,
    FSM_EVT_NUM
} e_fsm_evt_t;

typedef struct {
    e_fsm_mode_t mode;
    e_fsm_mode_t prev_mode;
} fsm_struct_t;


#define FSM_HANDLE_BUTTON_JOB_NAME              "button"
#define FSM_HANDLE_ROTARY_SWITCH_JOB_NAME       "switch"
#define FSM_HANDLE_ROTARY_TURN_JOB_NAME         "turn"

#define FSM_HANDLE_BUTTON_SWITCH_ARG_PRESS      "press"
#define FSM_HANDLE_BUTTON_SWITCH_ARG_RELEASE    "release"
#define FSM_HANDLE_ROTARY_TURN_ARG_LEFT         "left"
#define FSM_HANDLE_ROTARY_TURN_ARG_RIGHT        "right"


e_err_t fsm_init(void);

void fsm_handle_button(void* p);

void fsm_handle_rotary_switch(void* p);

void fsm_handle_rotary_turn(void* p);

e_err_t fsm_set_mode(e_fsm_mode_t);

e_fsm_mode_t fsm_get_mode(void);

e_fsm_mode_t fsm_get_prev_mode(void);

#endif // _FSM_H_