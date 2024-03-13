/**
 * @file sdcard.h
 * @author jake-is-ESD-protected
 * @date 2024-03-13
 * @brief SD card interfacing and tasks.
 *
 * 
 */

#ifndef _SDCARD_H_
#define _SDCARD_H_

#include "sys_err.h"

#define SDCARD_BASE_PATH    "/"

e_err_t sd_init(void);

esp_err_t sd_mount(void);

esp_err_t sd_unmount(void);

e_err_t sd_write(void* data, uint8_t type_size, uint32_t len, const char* fname);

e_err_t sd_read(void* data, uint8_t type_size, uint32_t len, const char* fname);

#endif // _SDCARD_H_
