/*
	auth:			    Jake T.
	brief:			  init for fieldrecorder, includes config and start of tasks
	date: 			  feb. 16th, 21
	modified by: 	Jake T.
	notes:			  ---rotary encoder is default hardware pull-up
                ---
	guide:			  
														  
*/

#include "tasks.h"
#include "mySD.h"
#include "myoled.h"
#include "general.h"
//*****************************************************************************************************************************************
/*GLOBAL VAR:
*
*/
//pointer to sample object passed to various tasks
sampling* myMainSampler = NULL;

//pointer to flag object passed to various tasks
flags* myFlags = new flags();

TaskHandle_t _manageoledTask;
TaskHandle_t _sendToSDTask;
TaskHandle_t _calculateDBFSTask;
TaskHandle_t _readI2STask;

static unsigned long last_interrupt_time_rec = 0;
static unsigned long last_interrupt_time_select = 0;

//*****************************************************************************************************************************************
/*ISR:
*
*/
//rec-button pressed
void IRAM_ATTR recButton_ISR(void){
  
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time_rec > IR_DEBOUNCE_TIME){

    if(myFlags->record == false){
      myFlags->record = true;
      myFlags->direction = STOP;
      myFlags->select = false;
    }
    else{
      myFlags->record = false;
    }
  }
  last_interrupt_time_rec = interrupt_time;
}

//switch-button pressed
void IRAM_ATTR switchButton_ISR(void){

  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time_select > IR_DEBOUNCE_TIME){
    myFlags->select = true;
  }
  last_interrupt_time_select = interrupt_time;
}

//turning rotary encoder
void IRAM_ATTR re_ISR(){
  if(digitalRead(PIN_ROTENC_CLK) == digitalRead(PIN_ROTENC_B)){
    myFlags->direction = CW;
  } 
  else{
    myFlags->direction = CCW;
  }
}

//*****************************************************************************************************************************************
/*SETUP:
*
*/
void setup() {

  _DBFSQueueHandle = xQueueCreate(1, 4);

  //sets pin 5 (SD-ChipSelect) as slave and checks for SD
  pinMode(SD_CS, OUTPUT); 
  digitalWrite(SD_CS, HIGH);
  if(!(SD.begin())){
    myFlags->errorSD = true;
  }
  vTaskDelay(100);


  //pin modes for interrupts and LED
  pinMode(PIN_REC, INPUT);
  pinMode(PIN_ROTENC_CLK, INPUT);
  pinMode(PIN_ROTENC_B, INPUT);
  pinMode(PIN_ROTENC_SWITCH, INPUT);
  pinMode(PIN_RED_LED, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(PIN_REC), recButton_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_ROTENC_SWITCH), switchButton_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_ROTENC_CLK), re_ISR, RISING);

  myMainSampler = new sampling();
  xTaskCreate(manageoledTask, "State machine", 8192, NULL, 1, &_manageoledTask);
  xTaskCreate(sendToSDTask, "Record Wav-Files to SD task", 4096, myMainSampler, 1, &_sendToSDTask);
  xTaskCreate(calculateDBFSTask, "calculate dBFs", 4096, myMainSampler, 1, &_calculateDBFSTask);
  myMainSampler->init(_sendToSDTask, BIG_BUF_LEN);
  myFlags->devInit = false;
}

//never go here
  void loop() {

}




