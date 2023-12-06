#include "i2s_io.h"
#include "driver/i2s.h"

static int32_t i2s_buf_audio[I2S_BUF_AUDIO_SIZE] = {0};
static uint32_t read_pointer = I2S_BUF_AUDIO_SIZE / 2;
static QueueHandle_t i2s_evt_queue_in = NULL;
static QueueHandle_t i2s_evt_queue_out = NULL;
static uint32_t write_pointer = 0;

void i2s_init(void* p){
    i2s_config_t i2s_config_input = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = I2S_SAMPLING_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = I2S_BUF_DMA_SIZE,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config_input = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = I2S_PIN_BCLK_IN,
        .ws_io_num = I2S_PIN_LRC_IN,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_PIN_SD_IN
    };

    i2s_config_t i2s_config_output = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = I2S_SAMPLING_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = I2S_BUF_DMA_SIZE,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config_output = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = I2S_PIN_BCLK_OUT,
        .ws_io_num = I2S_PIN_LRC_OUT,
        .data_out_num = I2S_PIN_SD_OUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config_input, 4, &i2s_evt_queue_in);
    i2s_set_pin(I2S_NUM_0, &pin_config_input);
    i2s_set_clk(I2S_NUM_0, I2S_SAMPLING_RATE, I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_STEREO);

    i2s_driver_install(I2S_NUM_1, &i2s_config_output, 4, &i2s_evt_queue_out);
    i2s_set_pin(I2S_NUM_1, &pin_config_output);
    i2s_set_clk(I2S_NUM_1, I2S_SAMPLING_RATE, I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_STEREO);
}

void i2s_run_in(void* p){
    while(1){
        i2s_event_t evt;
        if (xQueueReceive(i2s_evt_queue_in, &evt, portMAX_DELAY) == pdPASS){
            if (evt.type != I2S_EVENT_RX_DONE) continue;
            uint32_t n_read = 0;
            do{
                uint8_t i2s_dma_buf[I2S_BUF_DMA_SIZE];
                i2s_read(I2S_NUM_0, i2s_dma_buf, I2S_BUF_DMA_SIZE, &n_read, I2S_READ_TIMEOUT_TICKS);
                i2s_push_to_buf(i2s_dma_buf, i2s_buf_audio, n_read);
            }while (n_read > 0);
        }
    }
}

void i2s_run_out(void* p){
    while(1){
        i2s_event_t evt;
        if (xQueueReceive(i2s_evt_queue_out, &evt, portMAX_DELAY) == pdPASS){
            if (evt.type != I2S_EVENT_TX_DONE) continue;
            uint32_t n_write = 0;
            do{ // YOU ARE HERE
                uint8_t i2s_dma_buf[I2S_BUF_DMA_SIZE];
                i2s_pull_from_buf(i2s_dma_buf, i2s_buf_audio, n_write);
                i2s_write(I2S_NUM_0, i2s_dma_buf, I2S_BUF_DMA_SIZE, &n_write, portMAX_DELAY);
                i2s_push_to_buf(i2s_dma_buf, i2s_buf_audio, n_write);
            }while (n_write > 0);
        }
    }
}

void i2s_push_to_buf(uint8_t* data, int32_t* buf, uint32_t len){
    int32_t* data_signed = (int32_t*)data;
    uint32_t iterations = len / (I2S_RESOLUTION_BIT / 8);
    for(uint32_t i = 0; i < iterations; i++){
        buf[write_pointer++] = data_signed[i];
        if(write_pointer == I2S_BUF_AUDIO_SIZE) write_pointer = 0;
    }
}

void i2s_pull_from_buf(uint8_t* data, int32_t* buf, uint32_t len){
    int32_t* data_signed = (int32_t*)data;
    uint32_t iterations = len / (I2S_RESOLUTION_BIT / 8);
    for(uint32_t i = 0; i < iterations; i++){
        data_signed[i] = buf[read_pointer++];
        if(read_pointer == I2S_BUF_AUDIO_SIZE) read_pointer = 0;
    }
    data = (uint8_t*)data_signed;
}