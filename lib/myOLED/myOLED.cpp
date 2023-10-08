/*
	auth:			Jake T.
	brief:			oled-display
	date: 			mar. 7th, 21
	modified by: 	Jake T.
	notes:			--- font 0 is the 5*7 font. It contains symbols for all ASCII characters
                        font 1 is the 8*16 font. It only defines the standard printable characters (space and above)
                        font 2 is the large numbers. It only defines the basic numbers.
                        font 3 is the 7 segment font. It only defines The basic numbers and a decimal point.
	guide:			  
														  
*/

#include "myOLED.h"

//all used numbers were chosen by eye

void myInitScreen(MicroOLED Mainoled){
    Mainoled.setCursor(16, 14);
    Mainoled.setFontType(1);
    Mainoled.print("FR0");
    Mainoled.display();
}

void myVUGrid(MicroOLED Mainoled){

    //main separators
    Mainoled.lineH(1, 10, 64);
    Mainoled.lineH(1, 39, 64);

    //small text upper left
    Mainoled.setCursor(1, 1);
    Mainoled.setFontType(0);
    Mainoled.print("meas dBFS ");

    //big numbers center left
    Mainoled.setCursor(12, 18);
    Mainoled.setFontType(2);
    Mainoled.print("00");
    Mainoled.setCursor(1, 20);
    Mainoled.setFontType(1);
    Mainoled.print("-");

    //normal text add number
    Mainoled.setCursor(38, 23);
    Mainoled.setFontType(1);
    Mainoled.print("dB");

    //bar lower left
    Mainoled.rect(1, 41, 63, 7);

    Mainoled.display();
}

void myRecGrid(MicroOLED Mainoled){

    //small text upper left
    Mainoled.setCursor(1, 1);
    Mainoled.setFontType(0);
    Mainoled.print("recording ");

    //big numbers center left
    Mainoled.setCursor(1, 18);
    Mainoled.setFontType(2);
    Mainoled.print("00:00"); 

    //normal text add number
    Mainoled.setCursor(56, 23);
    Mainoled.setFontType(1);
    Mainoled.print("s");

    //small text lower left
    Mainoled.setCursor(1, 41);
    Mainoled.setFontType(0);
    Mainoled.print("000 GB/f  "); 

    Mainoled.display(); 
}

void myFileGrid(MicroOLED Mainoled, const char* firstFile){

    //small text upper left
    Mainoled.setCursor(1, 1);
    Mainoled.setFontType(0);
    Mainoled.print("my files: ");

    //small text center left
    Mainoled.setCursor(1, 12);
    Mainoled.setFontType(0);
    Mainoled.print(firstFile); 

    //small text lower left
    Mainoled.setCursor(1, 41);
    Mainoled.setFontType(0);
    Mainoled.print("000 MB       "); 

    Mainoled.setCursor(6, 20);
    Mainoled.print("                   ");

    Mainoled.display();
}

void displayTime(MicroOLED Mainoled, uint32_t sec){

    uint8_t min = sec / 60;
    sec = sec % 60;
    Mainoled.setFontType(2);

    if(sec < 10){
        Mainoled.setCursor(45, 18);
        Mainoled.print((char)(sec + '0'));
        Mainoled.setCursor(34, 18);
        Mainoled.print("0");
    }
    else{
        char buf[2];
        sprintf(buf, "%d", sec);
        Mainoled.setCursor(34, 18);
        Mainoled.print(buf);
    }

    if(min < 10){
        Mainoled.setCursor(12, 18);
        Mainoled.print((char)(min + '0'));
        Mainoled.setCursor(1, 18);
        Mainoled.print("0");        
    }
    else{
        char buf[2];
        sprintf(buf, "%d", min);
        Mainoled.setCursor(1, 18);
        Mainoled.print(buf);
    }

    Mainoled.display();
}