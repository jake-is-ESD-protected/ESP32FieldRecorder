#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_system.h"
#include "soc/i2s_reg.h"

#define SAMPLE_RATE         44100
#define MIC_GAIN            1
#define PIN_I2S_BCLK        25
#define PIN_I2S_LRC         33
#define PIN_I2S_DIN         26

#define PIN_REC             35
#define PIN_RED_LED         27

#define PIN_ROTENC_CLK      4
#define PIN_ROTENC_B        32
#define PIN_ROTENC_SWITCH   34

#define SMALL_BUF_LEN       1024
#define BIG_BUF_LEN         (32768 * 2)

#define STOP                0
#define CW                  1
#define CCW                 2

#define TIME_IDLE_RESET     4000 //ms
#define IR_DEBOUNCE_TIME    2000

class sampling {

private:
    int32_t* c_bigBuf1;
    int32_t* c_bigBuf2;
    uint32_t c_bufSizeBytes;
    int32_t c_bigBufIndex = 0;
    int32_t* c_sendThisBuffer;
    int32_t* c_fillThisBuffer;

    TaskHandle_t c__readI2STask;
    TaskHandle_t c__sendToSDTask;
    QueueHandle_t c__i2sFlagQueue;

protected:
    void dataFactory(uint8_t* smallBuf, uint32_t bufLen);

public:
    sampling(void);

    void init(TaskHandle_t _writeToSDTaskHandle, uint32_t bufSizeBytes);

    int32_t* getReadyBuf(){
        return c_sendThisBuffer;
    }

    friend void readI2STask(void* param);

};

class flags {
    public:
        //flags for rotary encoder
        bool select;                //saves a request for action after pressing the RE
        uint8_t direction;          //turning direction of rotary encoder: CW, CCW, STOP
        uint16_t fileCount;         //number of displayed files from SD-card
        uint16_t fileIndex;         //nth place of displayed file, used for back and forth scrolling capability

        //flag for other peripherals
        bool brownOut;              //flag for external interrupt by voltage supervisor, low battery
        bool record;                //true after red button press, false after next button press
        bool errorSD;               //see name.
        uint8_t err_gen = 0;
        bool VUmeter;

        //general flags
        bool devInit;               //active while booting

        flags(void);                //constructor   
};

//make flag-object visible for every directory
extern flags* myFlags;

extern TaskHandle_t _calculateDBFSTask;
extern TaskHandle_t _sendToSDTask;
extern TaskHandle_t _readI2STask;


#endif