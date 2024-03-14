/**
 * @file sys_err.h
 * @author jake-is-ESD-protected
 * @date 2024-03-13
 * @brief System wide error types.
 *
 * 
 */

#include "sys_err.h"
#include "Arduino.h"
#include "pin_defs.h"

void sys_err_handler(void){
    // this is BS
    // fix later
    pinMode(PIN_LED_RED, OUTPUT);
    while(1){
        digitalWrite(PIN_LED_RED, HIGH);
        vTaskDelay(300 / portTICK_PERIOD_MS);
        digitalWrite(PIN_LED_RED, LOW);
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}