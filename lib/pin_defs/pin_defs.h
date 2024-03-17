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

#include "hal/gpio_types.h"

#define DISPLAY_OLED

// SPI --------------------------------------------
#define PIN_SPI_CLK             GPIO_NUM_18
#define PIN_SPI_MOSI            GPIO_NUM_23
#define PIN_SPI_MISO            GPIO_NUM_19
#define PIN_SPI_SD_CS           GPIO_NUM_5
// SPI --------------------------------------------

#ifdef DISPLAY_OLED
// I2C --------------------------------------------
#define PIN_I2C_SCL             GPIO_NUM_22
#define PIN_I2C_SDA             GPIO_NUM_21
// I2C --------------------------------------------
#else
// UART -------------------------------------------
#define PIN_UART_RX             GPIO_NUM_22
#define PIN_UART_TX             GPIO_NUM_21
// UART -------------------------------------------
#endif

// I2S input --------------------------------------
#define PIN_I2S_BCLK_IN         GPIO_NUM_25
#define PIN_I2S_LRC_IN          GPIO_NUM_33
#define PIN_I2S_SD_IN           GPIO_NUM_26
// I2S input --------------------------------------


// I2S output -------------------------------------
#define PIN_I2S_BCLK_OUT        GPIO_NUM_16
#define PIN_I2S_LRC_OUT         GPIO_NUM_17
#define PIN_I2S_SD_OUT          GPIO_NUM_2
// I2S output -------------------------------------


// User input -------------------------------------
#define PIN_BUTTON              GPIO_NUM_35
#define PIN_ROTARY_SWITCH       GPIO_NUM_34
#define PIN_ROTARY_A            GPIO_NUM_4
#define PIN_ROTARY_B            GPIO_NUM_32
// User input -------------------------------------


// User output -------------------------------------
#define PIN_LED_RED             GPIO_NUM_27
// User output -------------------------------------


#endif // _PIN_DEFS_H_