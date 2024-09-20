#ifndef rpmu
#define rpmu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "buffer.h"

#define RPMU_READINGS_DIGIT_ACCURACY 4 // DO NOT INCREASE THIS BEYOND 8
#define RPMU_BUFFER_SIZE 8
#define RPMU_GENCMD_BUFFER_SIZE 1024

void GetAmpers(char* genc_result, char* ampers);
void GetVoltage(char* genc_result, char* voltage);
void gencmd(char* result);

#endif