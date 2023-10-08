/*
	auth:			    Jake T.
	brief:			  tasks and states for sampling and oled-display
	date: 			  feb. 16th, 21
	modified by: 	Jake T.
	notes:			  --- 
                ---
	guide:			  
														  
*/
#include "tasks.h"
#include "wav.h"
#include "mySD.h"
#include "myOLED.h"
#include "general.h"

//global vars
static void (*pNowF)(MicroOLED);
static void (*pNextF)(MicroOLED);
const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
QueueHandle_t _DBFSQueueHandle;

//*****************************************************************************************************************************************
/*STATES:
*
*/
//VU state
void stateVU(MicroOLED Mainoled){

  vTaskSuspend(_sendToSDTask);

  Mainoled.clear(ALL);
  Mainoled.clear(PAGE);
  myVUGrid(Mainoled);

  myFlags->VUmeter = true;

  //3 transitions:
  //idle = refresh level
  while((!(myFlags->select)) && (!(myFlags->record))){
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    char dBchar[2];
    if(ulNotificationValue > 0){
      int32_t dBvalue;
      int32_t displayValue = 64;
      xQueueReceive(_DBFSQueueHandle, &dBvalue, 10);
      displayValue = dBvalue;
      sprintf(dBchar, "%d", dBvalue);
      Mainoled.setCursor(12, 18);
      Mainoled.setFontType(2);
      Mainoled.print(dBchar);
      Mainoled.rectFill(1, 42, 64, 4, BLACK, NORM);
      Mainoled.rectFill(1, 42, (64 - displayValue), 4);
      Mainoled.rect(1, 41, 63, 7);
      Mainoled.display();
    }
  }
  //rec-button = record
  if(myFlags->record){
      pNextF = stateRec;
      vTaskResume(_sendToSDTask);
  }
  //select-button = select
  if(myFlags->select){
      pNextF = stateFile;
      myFlags->select = false;
  }
}

//file state
void stateFile(MicroOLED Mainoled){

  vTaskSuspend(_readI2STask);
  vTaskSuspend(_sendToSDTask);
  vTaskSuspend(_calculateDBFSTask);

  if(myFlags->errorSD){
    pNextF = stateNoSD;
    vTaskResume(_readI2STask);
    vTaskResume(_calculateDBFSTask);
    return;
  }

  myFlags->VUmeter = false;
  
  string* pFileList = scanFiles();
  const char* fileName;
  fileName = (pFileList[0]).c_str();
  myFlags->select = false;
  myFlags->fileIndex = 0;
  
  myFileGrid(Mainoled, fileName);
  
  //4 transitions:

  while(true){
  
    //T1
    //timeout
    uint32_t past = millis();
    uint32_t present = 0;
    uint32_t delta = 0;
    while((!(myFlags->select)) && ((myFlags->direction) == 0) && (delta < TIME_IDLE_RESET)){
      present = millis();
      delta = present - past;
    }
    if(delta == TIME_IDLE_RESET){
      pNextF = stateVU;
      vTaskResume(_readI2STask);
      vTaskResume(_calculateDBFSTask);
      return;
    }

    //T2
    if(myFlags->record){
      pNextF = stateRec;
      vTaskResume(_sendToSDTask);
      vTaskResume(_readI2STask);
      return;
    }

    //T3
    //select file
    if(myFlags->select){
      myFlags->select = false;
      pNextF = statePlay;
      return;
    }

    //T4
    //scroll = CW / CCW
    if((myFlags->direction) != STOP){
    
    Mainoled.setFontType(0);

      if(myFlags->direction == CW){ 
        (myFlags->fileIndex)++;
        if(((myFlags->fileIndex) < 0) || ((myFlags->fileIndex) > ((myFlags->fileCount) - 1))){
          myFlags->fileIndex = 0;
        }                
        fileName = (pFileList[(myFlags->fileIndex)]).c_str();
      }
      if(myFlags->direction == CCW){
        (myFlags->fileIndex)--;
        if(((myFlags->fileIndex) < 0) || ((myFlags->fileIndex) > ((myFlags->fileCount) - 1))){
          myFlags->fileIndex = (myFlags->fileCount) - 1;
        }       
        fileName = (pFileList[(myFlags->fileIndex)]).c_str();
      }
      Mainoled.setCursor(1, 12);
      Mainoled.print(fileName);
      Mainoled.setCursor(6, 20);
      Mainoled.print("                   ");
      Mainoled.display();
      
      myFlags->direction = STOP;   
    }
  }       
}

//playback state
void statePlay(MicroOLED Mainoled){
    
    myFlags->VUmeter = false;

    Mainoled.clear(ALL);
    Mainoled.print("wav-player");

    //2 transitions:

    //switch-button = select
    if(myFlags->select){
      pNextF = stateFile;
      myFlags->select = false;
    }
    //EOF
    //while(playing)
}

//recording state
void stateRec(MicroOLED Mainoled){

  vTaskSuspend(_calculateDBFSTask);

  if(myFlags->errorSD){
    myFlags->record = false;
    pNextF = stateNoSD;
    vTaskResume(_calculateDBFSTask);
    return;
  }

  myFlags->VUmeter = false;
  myRecGrid(Mainoled);

  uint8_t sec = 0;
  Mainoled.setFontType(2);

  while(myFlags->record){
    
    displayTime(Mainoled, sec);
    vTaskDelay(pdMS_TO_TICKS(1000));
    sec++;
  }

  //1 transition:

  //rec-button = off
  Mainoled.setCursor(2, 2);
  Mainoled.print("STOP      ");
  Mainoled.display();
  vTaskDelay(1000);
  pNextF = stateVU;
  vTaskResume(_calculateDBFSTask);
}

//service routine for any SD-related problems
void stateNoSD(MicroOLED Mainoled){

  pNextF = stateVU;

  if(SD.begin()){
    myFlags->errorSD = false;
    return;
  }
  Mainoled.rectFill(10, 12, 49, 22, BLACK, NORM);
  Mainoled.setFontType(1);
  Mainoled.setCursor(12, 14);
  Mainoled.print("NO SD");
  Mainoled.display();
  vTaskDelay(1000);
}

//*****************************************************************************************************************************************
/*TASKS:
*
*/
//read I2S and store in globally accessible array
void readI2STask(void* param){

  while(myFlags->devInit);

  //introduce pointer to sample-manager to this task
  sampling* mySampler = (sampling*)param;

  while(true){

    i2s_event_t evt;
    //poll for ready-flag in queue
    if (xQueueReceive(mySampler->c__i2sFlagQueue, &evt, portMAX_DELAY) == pdPASS){
      if (evt.type == I2S_EVENT_RX_DONE){
        size_t samplesRead = 0;

        do{
          uint8_t i2sData[SMALL_BUF_LEN];
          i2s_read(I2S_NUM_0, i2sData, SMALL_BUF_LEN, &samplesRead, 10);
          mySampler->dataFactory(i2sData, samplesRead);
        }while (samplesRead > 0);
      }
    }
  }
}

//send I2S data to SDcard
void sendToSDTask(void* param){

  while(true){

    //wait for flag to change
    while(!(myFlags->record));

    vTaskSuspend(_calculateDBFSTask);

    sampling* mySampler = (sampling*)param;
    static char filename[] = "/rec000.wav";
    const int headerSize = 512;
    const int waveDataSize = 2 * SAMPLE_RATE * 4; //default size = 1s
    byte header[headerSize];
    File file;

    //create RIFF-WAV-Header
    CreateWavHeader(header, waveDataSize, SAMPLE_RATE);

    //find valid name
    findName(filename);

    //open/make file on SD card with given name
    file = SD.open(filename, FILE_WRITE);
    if(!file){
      myFlags->errorSD = true;
      myFlags->record = false;
      vTaskResume(_calculateDBFSTask);
      pNextF = stateVU;
      break;
    }

    //induce wav-header into start of file
    file.write(header, headerSize);
    
    int j = 0;

    //red means recording!
    digitalWrite(PIN_RED_LED, HIGH);
    while (myFlags->record)
    {
      //wait for notification
      uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
      if(ulNotificationValue > 0){

        file.write((uint8_t*)mySampler->getReadyBuf(), (size_t)(BIG_BUF_LEN));
      }
      j++;
    }
    
    //calculate recording-lenght and save it to header at the start of the file
    digitalWrite(PIN_RED_LED, LOW);
    CreateWavHeader(header, (j * SD_BUF_LEN_32BIT * 4 * 2), SAMPLE_RATE);
    file.seek(0);
    file.write(header, headerSize);
    file.close();

    vTaskResume(_calculateDBFSTask);
  }
  
}

//centralized access to oled-display
void manageoledTask(void* param){

  MicroOLED oled(PIN_RESET, DC_JUMPER);
  vTaskDelay(100);
  Wire.begin();
  // vTaskDelay(100);
  oled.begin();
  oled.clear(ALL);
  oled.clear(PAGE);

  //display init-screen
  myInitScreen(oled);
  pNowF = stateVU;

  vTaskDelay(1000);

  while(true){

    pNowF(oled);
    pNowF = pNextF;
  }
}

//see name
void calculateDBFSTask(void* param){

  while(true){
    sampling* mySampler = (sampling*)param;

    while(myFlags->VUmeter){
      uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
      if(ulNotificationValue > 0){
        int32_t* toCalc = mySampler->getReadyBuf();

        double sum = 0;
        for (int i = 0; i < (BIG_BUF_LEN / 4); i = i + 2){
            double sample = ((double)toCalc[i]) / ((double)INT32_MAX);
            sum += (sample * sample);
        }
        double rms = sqrt(sum / ((BIG_BUF_LEN / 4) / 2));
        int32_t decibel = (int32_t)(20 * log10(rms)) * (-1);
        xQueueSend(_DBFSQueueHandle, &decibel, 10);
        xTaskNotify(_manageoledTask, 1, eIncrement);
      }      
    }
  }
}