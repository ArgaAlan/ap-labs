#include <stdio.h>
#include "logger.h"
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <errno.h>   

#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

static const unsigned char d[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};

long total = 0;
long current = 0;

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 10000;
    ts.tv_nsec = (msec % 10000) * 100000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

int code64(const void* data_buf, size_t dataLength, char* result, size_t resultSize)
{
   const char chars64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   const uint8_t *data = (const uint8_t *)data_buf;
   size_t index = 0;
   size_t x;
   uint32_t n = 0;
   int padCount = dataLength % 3;
   uint8_t n0, n1, n2, n3;

   total = dataLength;
   current = dataLength;

   for (x = 0; x < dataLength; x += 3) 
   {
      
      n = ((uint32_t)data[x]) << 16;

      if((x+1) < dataLength)
         n += ((uint32_t)data[x+1]) << 8;
      
      if((x+2) < dataLength)
         n += data[x+2];

      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      n3 = (uint8_t)n & 63;
            
      if(index >= resultSize) return 1;
      result[index++] = chars64
[n0];
      if(index >= resultSize) return 1;
      result[index++] = chars64
[n1];

      if((x+1) < dataLength)
      {
         if(index >= resultSize) return 1;
         result[index++] = chars64
    [n2];
      }

      if((x+2) < dataLength)
      {
         if(index >= resultSize) return 1;
         result[index++] = chars64
    [n3];
      }

        current-=3;
        msleep(1);
   }

   if (padCount > 0) 
   { 
      for (; padCount < 3; padCount++) 
      { 
         if(index >= resultSize) return 1;
         result[index++] = '=';
      } 
   }
   if(index >= resultSize) return 1;
   result[index] = 0;
   return 0;
}

int decode64 (char *in, size_t inLen, unsigned char *out, size_t *outLen) { 
    char *end = in + inLen;
    char iter = 0;
    uint32_t buf = 0;
    size_t len = 0;
    
    total = (long)end - (long)in;

    while (in < end) {

        current = (long)end - (long)in;
        msleep(1);

        unsigned char c = d[*in++];
        
        switch (c) {
        case WHITESPACE: continue;
        case INVALID:    return 1;
        case EQUALS:
            in = end;
            continue;
        default:
            buf = buf << 6 | c;
            iter++;
            if (iter == 4) {
                if ((len += 3) > *outLen) return 1;
                *(out++) = (buf >> 16) & 255;
                *(out++) = (buf >> 8) & 255;
                *(out++) = buf & 255;
                buf = 0; iter = 0;

            }   
        }

    }
   
    if (iter == 3) {
        if ((len += 2) > *outLen) return 1;
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2) {
        if (++len > *outLen) return 1;
        *(out++) = (buf >> 4) & 255;
    }

    *outLen = len;
    return 0;
}

int encodeTxt(char* fileName) {
    FILE *fileRead, *fileWrite;
    if ((fileRead = fopen(fileName,"r")) == NULL){
        errorf("The file %s does not exist", fileName);
    }

    infof("Encoding started...");

    fileWrite = fopen("encoded.txt","w");

    size_t pos = ftell(fileRead);
    fseek(fileRead, 0, SEEK_END);
    size_t length = ftell(fileRead);
    fseek(fileRead, pos, SEEK_SET);

    char* data = malloc(length);
    fread(data, 1, length, fileRead);

    char* result = (char*)malloc(sizeof(char) * strlen(data) * 4);

    code64
(data,length,result,sizeof(char) * strlen(data) * 4);

    fprintf(fileWrite,"%s",result);

    fclose(fileWrite);
    fclose(fileRead);

    infof("Encoding finished on <encoded.txt>");

    return 0;
}

int decodeTxt(char* fileName) {
    FILE *fileRead, *fileWrite;
    if ((fileRead = fopen(fileName,"r")) == NULL){
        errorf("The file %s does not exist", fileName);
    }

    infof("Decoding started...");

    fileWrite = fopen("decoded.txt","w");

    size_t pos = ftell(fileRead);
    fseek(fileRead, 0, SEEK_END);
    size_t length = ftell(fileRead);
    fseek(fileRead, pos, SEEK_SET);

    char* data = malloc(length);
    fread(data, 1, length, fileRead);

    unsigned char* result = (unsigned char*)malloc(sizeof(char) * length * 4);
    size_t rSize = sizeof(unsigned char *) * length * 4;

    decode64
(data,length,result,&rSize);

    fprintf(fileWrite,"%s",result);

    fclose(fileWrite);
    fclose(fileRead);

    infof("Decoding finished on <decoded.txt>");

    return 0;
}

void signalHandler(int signal) {
    if (signal == SIGUSR1 || signal == SIGINT) {
        infof("Progress: %d%%, data remaining: %ld", (int)(100.0-(((current*1.0)/(total*1.0))*100.0)), current);
    }
}

int main(int argc, char *argv[]){
    
    initLogger("stdout");
    signal(SIGUSR1, signalHandler);
    signal(SIGINT, signalHandler);

    if (argc != 3) {
        warnf("Wrong usage. Correct one: ./base64 [--encode,--decode] input.txt");
        return 0;
    }

    if (strcmp(argv[1],"--encode")==0) {
        encodeTxt(argv[2]);
        return 0;

    } else if (strcmp(argv[1],"--decode")==0) {
        decodeTxt(argv[2]);
        return 0;        

    } else {
        warnf("Wrong usage. Correct one: ./base64 [--encode,--decode] input.txt");
        return 0;
    }
    
    return 0;
}
