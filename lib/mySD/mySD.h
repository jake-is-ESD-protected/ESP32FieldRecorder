#ifndef _MYSD_H_
#define _MYSD_H_

#include <sstream>
#include <SD.h>

using namespace std;

#define FILENAME_LEN        12
#define SD_CS               5
#define SD_BUF_LEN_32BIT    8192 
#define SD_BUF_LEN_BYTES    (SD_BUF_LEN_32BIT * 4)

#define WAV_ERROR_NO_SD     0

//iterates from 0 to 999 to find an unused filename
void findName(char* filename);

string* scanFiles(void);



#endif //MYSD_H