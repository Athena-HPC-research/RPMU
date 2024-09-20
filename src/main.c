#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "RPMU.h"
#include "buffer.h"

void TerminalMode();
void FileMode(const char* filename, int minutes_to_run);

int main(int argc, char** argv) {
    if (argc != 1 && argc != 3) {
        printf("Invalid amount of arguments\n");
        printf("[file]: OPTIONAL, specifies the file the output should be stored at\n");
        printf("[minutes]: OPTIONAL, specifies the amount of time the program should run in minutes\n");
        exit(-1);
    }

    if (argc == 1) {
        TerminalMode();
    } else {
        if (isdigit(argv[2]) == 0) {
            printf("The [minutes] argument must be a number\n");
            exit(-1);
        }

        FileMode(argv[1], atoi(argv[2]));
    }

    return 0;
}

void TerminalMode() {
    char* gencmd_result = CreateBuffer(RPMU_GENCMD_BUFFER_SIZE);
    char* voltage = CreateBuffer(RPMU_BUFFER_SIZE);
    char* ampers = CreateBuffer(RPMU_BUFFER_SIZE);

    while (1) {
        gencmd(gencmd_result);
        GetVoltage(gencmd_result, voltage);
        GetAmpers(gencmd_result, ampers);
        printf("%sV %sA\n", voltage, ampers);
        sleep(1);
    }
}

void FileMode(const char* filename, int minutes_to_run) {
    char* filenameBuffer = CreateBuffer(200);
    char* filenameCounterBuffer = CreateBuffer(200);
    int filenameCounter = 0;
    char* gencmd_result = CreateBuffer(RPMU_GENCMD_BUFFER_SIZE);
    char* voltage = CreateBuffer(RPMU_BUFFER_SIZE);
    char* ampers = CreateBuffer(RPMU_BUFFER_SIZE);

    EmptyBuffer(filenameBuffer, 200);
    EmptyBuffer(filenameCounterBuffer, 200);
    sprintf(filenameCounterBuffer, "%d", filenameCounter);
    strcpy(filenameBuffer, filename);
    strcat(filenameBuffer, filenameCounterBuffer);

    // We want to run this in the background only in file mode, terminal mode is meant for testing
    daemon(1, 1);

    for (int i = 0; i < minutes_to_run; i++) {
        FILE* output_file = fopen(filenameBuffer, "w");

        filenameCounter++;

        sprintf(filenameCounterBuffer, "%d", filenameCounter);
        strcpy(filenameBuffer, filename);
        strcat(filenameBuffer, filenameCounterBuffer);

        for (int i = 0; i < 60; i++) {
            gencmd(gencmd_result);
            GetVoltage(gencmd_result, voltage);
            GetAmpers(gencmd_result, ampers);
            fprintf(output_file, "%i %s %s\n", time(NULL), voltage, ampers);
            sleep(1);
        }

        fclose(output_file);
    }
}