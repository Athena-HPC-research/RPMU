#ifndef pmch
#define pmch

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define PMC_BUFFER_SIZE 40

void GetVoltageAndAmperes(char result[PMC_BUFFER_SIZE]);
void gencmd(char result[]);

#endif
