#ifndef _TASKS_H_
#define _TASKS_H_

#include "Arduino.h"
#include "math.h"

//sample DMA into buffer
void readI2STask(void* param);

//store data on SD
void sendToSDTask(void* param);

//manage oled-usage by different tasks
void manageoledTask(void* param);

//see name
void calculateDBFSTask(void* param);

//ISRs
void IRAM_ATTR recButton_ISR(void);
void IRAM_ATTR switchButton_ISR(void);
void IRAM_ATTR reCLK_ISR(void);
void IRAM_ATTR reB_ISR(void);

extern TaskHandle_t _manageoledTask;
extern QueueHandle_t _DBFSQueueHandle;

#endif //TASKS_H