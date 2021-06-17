#ifndef _WAV_H_
#define _WAV_H_

#include <Arduino.h>

//size of header is 512 byte
void CreateWavHeader(byte* header, int waveDataSize, int sampleRate);

//optional
typedef struct{

    uint8_t riff[4];
    int32_t size;
    uint8_t wave[4];

    uint8_t fmt[4];
    int32_t fmtSize;
    int16_t format;
    int16_t channels;
    int32_t sampleRate;
    int32_t byteRate;
    int16_t alignment;
    int16_t bit_depth;

    uint8_t padd[4];
    int32_t paddSize;
    int32_t padding;

    uint8_t data[4];
    int32_t dataLenBytes; 

} wav_header_t;


#endif //WAV_H
