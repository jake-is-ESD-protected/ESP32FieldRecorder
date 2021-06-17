//Contains size-adjustable wav-header with padding

#include "Wav.h"

void CreateWavHeader(byte* header, int waveDataSize, int samplingRate){
  header[0] = 'R';
  header[1] = 'I';
  header[2] = 'F';
  header[3] = 'F';
  unsigned int fileSizeMinus8 = waveDataSize + 512 - 8;
  header[4] = (byte)(fileSizeMinus8 & 0xFF);
  header[5] = (byte)((fileSizeMinus8 >> 8) & 0xFF);
  header[6] = (byte)((fileSizeMinus8 >> 16) & 0xFF);
  header[7] = (byte)((fileSizeMinus8 >> 24) & 0xFF);
  header[8] = 'W';
  header[9] = 'A';
  header[10] = 'V';
  header[11] = 'E';
  header[12] = 'f';
  header[13] = 'm';
  header[14] = 't';
  header[15] = ' ';
  header[16] = 0x10;  // fmt lenght = 16byte
  header[17] = 0x00;  // 
  header[18] = 0x00;  //
  header[19] = 0x00;  //
  header[20] = 0x01;  // format tag = 1 = PCM
  header[21] = 0x00;  //
  header[22] = 0x02;  // channels = 2 = stereo
  header[23] = 0x00;  //
  header[24] = (byte)(samplingRate & 0xFF);           //sampling rate
  header[25] = (byte)((samplingRate >> 8) & 0xFF);    //
  header[26] = (byte)((samplingRate >> 16) & 0xFF);   //
  header[27] = (byte)((samplingRate >> 24) & 0xFF);   //
  unsigned int bytePerSec = samplingRate * 8;
  header[28] = (byte)(bytePerSec & 0xFF);             //byte per second = sample rate * channels * byte per sample (4)
  header[29] = (byte)((bytePerSec >> 8) & 0xFF);      //
  header[30] = (byte)((bytePerSec >> 16) & 0xFF);     //
  header[31] = (byte)((bytePerSec >> 24) & 0xFF);     //
  header[32] = 0x08;  // block align (n channels = 2 * (bits/sample of one channel = 32 + 7)/8) = 8
  header[33] = 0x00;  //
  header[34] = 0x20;  // bits/sample = 32
  header[35] = 0x00;  //
  header[36] = 'p';   //new padding sub-chunk. Increases SD-performance since it writes in 512 byte sized blocks naturally
  header[37] = 'a';   //
  header[38] = 'd';   //
  header[39] = 'd';   //
  header[40] = 0xCC;  //padlen is rest of header - 512 = 460
  header[41] = 0x01;  //
  header[42] = 0x00;  //
  header[43] = 0x00;  //
  for(int i = 44; i < 504; i++){
    header[i] = 0x00;
  }
  header[504] = 'd';  //data sub-chunk
  header[505] = 'a';  //
  header[506] = 't';  // 
  header[507] = 'a';  //
  header[508] = (byte)(waveDataSize & 0xFF);
  header[509] = (byte)((waveDataSize >> 8) & 0xFF);
  header[510] = (byte)((waveDataSize >> 16) & 0xFF);
  header[511] = (byte)((waveDataSize >> 24) & 0xFF);
  //real samples get appended here
}

