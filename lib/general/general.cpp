#include "general.h"
#include "tasks.h"
#include "myOLED.h"

//constructor for sampling-class
sampling::sampling(){
    //constructor instructions
}

void sampling::init(TaskHandle_t _writeToSDTaskHandle, uint32_t bufSizeBytes){

    c__sendToSDTask = _writeToSDTaskHandle;
    c_bufSizeBytes = bufSizeBytes;
    c_bigBuf1 = (int32_t*)malloc(bufSizeBytes);
    c_bigBuf2 = (int32_t*)malloc(bufSizeBytes);

    if((c_bigBuf1 == NULL) || (c_bigBuf2 == NULL)){
        myFlags->err_gen = 1;
        while(1);
    }

    c_fillThisBuffer = c_bigBuf1;
    c_sendThisBuffer = c_bigBuf2;

    //general config of I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = SMALL_BUF_LEN,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    //pin format, adjust in i2s.h if needed:
    i2s_pin_config_t pin_config = {
        .bck_io_num = PIN_I2S_BCLK,
        .ws_io_num = PIN_I2S_LRC,
    };
    //i tried putting this into statement above, threw an error (?)
    pin_config.data_in_num = PIN_I2S_DIN;

    //fix for SPH645-specific timing-error:
    // REG_SET_BIT(I2S_TIMING_REG(I2S_NUM_0), BIT(9));
    // REG_SET_BIT(I2S_CONF_REG(I2S_NUM_0), I2S_RX_MSB_SHIFT);

    //init I2S-driver/peripherals
    i2s_driver_install(I2S_NUM_0, &i2s_config, 4, &c__i2sFlagQueue);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_STEREO);

    xTaskCreate(readI2STask, "I2S store", 4096, this, 1, &_readI2STask);
}

void sampling::dataFactory(uint8_t* smallBuf, uint32_t bufLen){

    int32_t* samples = (int32_t*)smallBuf;
    for(int i = 0; i < (bufLen / 4); i++){

        c_fillThisBuffer[c_bigBufIndex] = samples[i] << MIC_GAIN; //work with << for gain
        c_bigBufIndex++;

        if(c_bigBufIndex == (c_bufSizeBytes / 4)){

            std::swap(c_fillThisBuffer, c_sendThisBuffer);
            c_bigBufIndex = 0;
            if(myFlags->record){
                xTaskNotify(c__sendToSDTask, 1, eIncrement);
            }
            if(myFlags->VUmeter){
                xTaskNotify(_calculateDBFSTask, 1, eIncrement);
            }
        }
    }
}

//constructor for flags-class
flags::flags(){

    //brownOut = digitalRead(PIN_SUPVIS_LOW_BAT)
    record = false;
    select = false;
    devInit = true;
    errorSD = false;
    VUmeter = true;
    direction = STOP;
    fileCount = 0;
    fileIndex = 0;
}

