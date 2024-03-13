/**
 * @file pin_defs.h
 * @author jake-is-ESD-protected
 * @date 2024-03-13
 * @brief Hardware pin definitions
 *
 * 
 */

#ifndef _PIN_DEFS_H_
#define _PIN_DEFS_H_


// SPI --------------------------------------------
#define PIN_SPI_CLK             18
#define PIN_SPI_MOSI            23
#define PIN_SPI_MISO            19
#define PIN_SPI_SD_CS           5
// SPI --------------------------------------------

// I2C --------------------------------------------
#define PIN_I2C_SCL             22
#define PIN_I2C_SDA             21
// I2C --------------------------------------------


// I2S input --------------------------------------
#define PIN_I2S_BCLK_IN         25
#define PIN_I2S_LRC_IN          33
#define PIN_I2S_SD_IN           26
// I2S input --------------------------------------


// I2S output -------------------------------------
#define PIN_I2S_BCLK_OUT        16
#define PIN_I2S_LRC_OUT         17
#define PIN_I2S_SD_OUT          2
// I2S output -------------------------------------


// User input -------------------------------------
#define PIN_BUTTON              35
#define PIN_ROTARY_SWITCH       34
#define PIN_ROTARY_A            4
#define PIN_ROTARY_B            32
// User input -------------------------------------


// User output -------------------------------------
#define PIN_LED_RED             27
// User output -------------------------------------


#endif // _PIN_DEFS_H_