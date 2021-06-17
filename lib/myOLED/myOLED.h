#ifndef _MYOLED_H_
#define _MYOLED_H_

#include "Wire.h"
#include "SFE_MicroOLED.h"

#define OLED_ADDR 0x3D
#define PIN_RESET -1
#define DC_JUMPER 1

//oled-states
void stateVU(MicroOLED Mainoled);
void stateFile(MicroOLED Mainoled);
void statePlay(MicroOLED Mainoled);
void stateRec(MicroOLED Mainoled);
void stateNoSD(MicroOLED Mainoled);

//draw-functions
void myVUGrid(MicroOLED Mainoled);
void myInitScreen(MicroOLED Mainoled);
void myRecGrid(MicroOLED Mainoled);
void myFileGrid(MicroOLED Mainoled, const char* firstFile);
void displayTime(MicroOLED, uint32_t sec);

#endif //MYOLED_H