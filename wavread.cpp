#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

void fcheck(int ret, int count) {
    if (ret != count)
        perror("error parsing header");
    return;
}

int main() {
    FILE* wavfile;
    int headerLen = 44;
    int ret;
    char header[headerLen];

    char riff[4]; // "RIFF"
    char file_sz[4]; // file size, 4 bytes
    char wave[4];
    char fmt[4];
    char fmtlen[4];
    char fmttype[2];
    char numChan[2];
    char sampRate[4];
    char byteRate[4];
    char blckAlign[4];
    char bps[2];
    char data[4];
    char data_sz[4];

    wavfile = fopen("bach_prelude_mono.wav", "r");
    if (wavfile == NULL) perror ("error opening file");

    else {
        fread(riff, sizeof(char), 4, wavfile);
        if (strncmp(riff, "RIFF", 4) != 0)
            perror ("not a WAV/RIFF file!");
        
        ret = fread(file_sz, sizeof(int32_t), 1, wavfile);
        fcheck(ret, 1);
        
        ret = fread(wave, sizeof(char), 4, wavfile);
        fcheck(ret, 4);

        ret = fread(fmt, sizeof(char), 4, wavfile);
        fcheck(ret, 4);

        ret = fread(fmtlen, sizeof(int32_t), 1, wavfile);
        fcheck(ret, 1);

        ret = fread(fmttype, sizeof(uint8_t), 1, wavfile);
        fcheck(ret, 1);

        fputs(fmtlen, stdout);

    fclose (wavfile);
   }

   return 0;
}