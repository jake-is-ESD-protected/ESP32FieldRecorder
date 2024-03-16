/**
 * @file tft.h
 * @author jake-is-ESD-protected
 * @date 2024-03-16
 * @brief TFT display abstraction (works over UART)
 *
 * 
 */

#ifndef _TFT_H_
#define _TFT_H_

#include "sys_err.h"

#define UART_NUM UART_NUM_2
#define BAUDRATE 115200
#define BUF_SIZE (1024)

e_err_t tft_init(void);

void tft_server(void *p);


#endif // _TFT_H_