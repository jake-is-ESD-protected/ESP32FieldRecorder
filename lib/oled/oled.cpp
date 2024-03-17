/**
 * @file oled.cpp
 * @author jake-is-ESD-protected
 * @date 2024-03-15
 * @brief OLED driver abstraction code
 *
 * 
 */

#include "oled.h"
#include "bitmaps.h"
#include "pin_defs.h"
#include "SparkFun_Qwiic_OLED.h"
#include "Wire.h"

QwiicMicroOLED oled;

void oled_init(void){
    Wire.begin();
    if (!oled.begin()){
        // ret err
    }
    oled.display();
}

void oled_title_screen(void){
    oled.reset(true);
    oled.bitmap(0, 0, BMP_IDLE_SCREEN);
    oled.display();
}

void oled_refresh_test(void){
    unsigned long start_time, end_time, refresh_time;
    oled.reset(true);
    // start_time = millis();
    for(int i=0; i<48; i++){
        oled.pixel(i, i);
        oled.display();
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
    // end_time = millis();
    // refresh_time = end_time - start_time;
    // oled.reset(true);
    // oled.setCursor(0,0);
    // oled.print("Refresh time: ");
    // oled.print(refresh_time);
    // oled.print(" ms");
    // oled.display();
}