#pragma once

#include "management.h"
#include "inttypes.h"

#define I2S_BUF_AUDIO_SIZE      (1024 * 2)
#define I2S_BUF_DMA_SIZE        1024

#define I2S_SAMPLING_RATE       44100
#define I2S_TASK_INIT_MEM_SIZE  1024
#define I2S_TASK_RUN_MEM_SIZE   4096
#define I2S_TASK_INIT_PRIORITY  1
#define I2S_TASK_RUN_PRIORITY   1
#define I2S_READ_TIMEOUT_TICKS  10
#define I2S_RESOLUTION_BIT      32

void i2s_init(void* p);

void i2s_run_in(void* p);

void i2s_run_out(void* p);

void i2s_push_to_buf(uint8_t* data, int32_t* buf, uint32_t len);

void i2s_pull_from_buf(uint8_t* data, int32_t* buf, uint32_t len);