/**
 * @file bitmaps.h
 * @author jake-is-ESD-protected
 * @date 2024-03-17
 * @brief Singleton objects for bitmaps
 * @note    After messing around with the non-qwiic lib and u8g2 for about 10h
 *          I decided to stick with whatever Sparkfun gives me, I am too tired
 *          for this shit. For this reason, the code, drivers and all that stuff
 *          related to the OLED is in heavy C++.
 *          
 *          Bitmap data is stored as static const unsigned char arrays in their
 *          seperate own header files always prefixed with data_. The file here 
 *          imports the raw data and turns it into a bitmap object. This
 *          object is then exported as macro.
 */


#ifndef _BITMAPS_H_
#define _BITMAPS_H_

#include <SparkFun_Qwiic_OLED.h>

// ------------------------------------------------------------------------------------
// Title screen object
// ------------------------------------------------------------------------------------
class bmp_title_screen final : public bmpSingleton<bmp_title_screen> {
    public:
    const uint8_t* data(void){
        #include "data_title_screen.h"
        return title_screen;
    }
    bmp_title_screen() : bmpSingleton<bmp_title_screen>(TITLE_SCREEN_WIDTH, TITLE_SCREEN_HEIGHT){}
};
#define BMP_TITLE_SCREEN bmp_title_screen::instance()
// ------------------------------------------------------------------------------------
// Title screen object
// ------------------------------------------------------------------------------------



#endif // _BITMAPS_H_