# ESP32FieldRecorder

Stereo field recorder with FS-Meter on the ESP32, I2S MEMS-mics. Handheld with 3 AA batteries. If you are interested in audio with ESP32, check out this repo.

This field recorder uses multithreading to achieve clean stereo sampling via the ESP32 HAL's i2s_read() function.