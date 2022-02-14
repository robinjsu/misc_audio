#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

void fcheck(int ret, int count) {
    if (ret != count)
        perror("error parsing header");
    return;
}

int main(int argc, char *argv[]) {
    FILE* wavfile;
    int headerLen = 44;
    int ret;
    char header[headerLen];

    char riff[4]; // "RIFF"
    uint32_t *file_sz; // file size, 4 bytes
    char wave[4];
    char fmt[4];
    uint16_t fmtlen[2];
    uint8_t audiofmt[2];
    uint8_t numChan[2];
    uint8_t sampRate[4];
    uint8_t byteRate[4];
    uint8_t blckAlign[2];
    uint8_t bps[2];
    char data[4];
    uint8_t data_sz[4];
    char fmtType[4];

    if (argc != 2)
        perror ("incorrect arguments passed");
    
    if (strnlen(argv[1], sizeof(char)*100) == 100 || strnlen(argv[1], sizeof(char)*100) == 0)
        perror ("filename error, or exceeds maximum length");

    wavfile = fopen(argv[1], "r");
    if (wavfile == NULL) 
        perror ("error opening file");


    fread(riff, sizeof(char), 4, wavfile);
    if (strncmp(riff, "RIFF", 4) != 0)
        perror ("not in RIFF format!");
    
    ret = fread(file_sz, sizeof(uint32_t), 1, wavfile);
    fcheck(ret, 1);
    
    ret = fread(wave, sizeof(char), 4, wavfile);
    if (strncmp(wave, "WAVE", 4) != 0)
        perror ("not a WAVE file!");
    fcheck(ret, 4);

    ret = fread(fmt, sizeof(char), 4, wavfile);
    fcheck(ret, 4);
    if (strncmp(fmt, "fmt ", 4) != 0)
        perror ("fmt string is corrupted");

    ret = fread(fmtlen, sizeof(uint16_t), 2, wavfile);
    fcheck(ret, 2);

    ret = fread(audiofmt, sizeof(uint8_t), 2, wavfile);
    fcheck(ret, 2);
    if (*audiofmt == 1)
        strncpy(fmtType, "PCM\0", 4);


    ret = fread(numChan, sizeof(uint8_t), 2, wavfile);
    fcheck(ret, 2);

    ret = fread(sampRate, sizeof(uint8_t), 4, wavfile);
    fcheck(ret, 4);
    uint32_t rate = 0;
    for (int i = 3; i >=0; i--) {
        rate <<= 8;
        rate += sampRate[i];
    }
    

    ret = fread(byteRate, sizeof(uint8_t), 4, wavfile);
    fcheck(ret, 4);

    ret = fread(blckAlign, sizeof(uint8_t), 2, wavfile);
    fcheck(ret, 2);

    ret = fread(bps, sizeof(uint8_t), 2, wavfile);
    fcheck(ret, 2);

    ret = fread(data, sizeof(char), 4, wavfile);
    fcheck(ret, 4);
    if (int err = strncmp(data, "data", 4) != 0)
        perror ("data string corrupted");

    ret = fread(data_sz, sizeof(uint8_t), 4, wavfile);
    fcheck(ret, 4);

    printf("Wave File Info\nName: %s\nSize: %u frames\nSample Rate: %u sps\nFormat Type: %u-bit %s\nChannels: %u\n", argv[1], *file_sz, rate, *fmtlen, fmtType, *numChan);
    
    fclose (wavfile);

    return 0;
}