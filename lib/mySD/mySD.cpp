//Contains function for file-naming and SD-card init

#include "mySD.h"
#include "general.h"
#include "string.h"

using namespace std;

#define SD_CS 5

//algorithm for naming recordings on SD-card. Tries to find "/recxxx.wav" and adds the chronologically next filename
void findName(char* filename) {
    int a = 0;
    string outs;
    while (SD.exists(filename)) { 
        a++;
        stringstream outss;
        stringstream conv; 
        conv << a;
        string convstr = conv.str();

        if (a < 10) {
            outss << "/rec" << "00" << convstr << ".wav";
        }
        else if (a < 100) {
            outss << "/rec" << "0" << convstr << ".wav";
        }
        else {
            outss << "/rec" << convstr << ".wav";
        }
        outs = outss.str();
        for (int i = 0; i < FILENAME_LEN; i++) {
          filename[i] = outs[i];
        }
    }
}

//scan files on SD-card and return all found names
string* scanFiles(void){
    if(myFlags->errorSD){
        //error handler
    }
    uint32_t i = 0;
    string* pNameList;
    myFlags->fileCount = 0;
    File root = SD.open("/");
    File entry = root.openNextFile();
    while(entry){
        myFlags->fileCount++;
        entry = root.openNextFile();
    }
    root.rewindDirectory();
    pNameList = new string[(myFlags->fileCount)];
    entry = root.openNextFile();
    while(entry && (i < (myFlags->fileCount))){
        pNameList[i] = entry.name();
        entry = root.openNextFile();
        i++;
    }
    return pNameList;
}