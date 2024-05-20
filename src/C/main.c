#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "PMC.h"

#define FILE_MODE_LIMIT_MINUTES 1

void TerminalMode();
void FileMode(const char* filename);

int main(int argc, char** argv) {
    if (argc > 2) {
        printf("Invalid amount of arguments\n");
	    printf("[file]: OPTIONAL, specifies the file the output should be stored at\n");
	    exit(-1);
    }

    if (argc == 1)
        TerminalMode();
    else
        FileMode(argv[1]);

    return 0;
}

void TerminalMode() {
    char result[PMC_BUFFER_SIZE];
    
    while (1) {
        GetVoltageAndAmperes(result);
        printf("%s\n", result);
        sleep(1);
    }
}

void FileMode(const char* filename) {
    char result[PMC_BUFFER_SIZE];
    time_t current_time;
    FILE* output_file = fopen(filename, "w");

    if (output_file == NULL) {
        printf("Error creating output file");
        exit(-1);
    }

    for (int i = 0; i < FILE_MODE_LIMIT_MINUTES * 60; i++) {
        GetVoltageAndAmperes(result);
        time(&current_time);
        fprintf(output_file, "%s %s\n\n", ctime(&current_time), result);
        printf("TEST\n");
        sleep(1);
    }

    fclose(output_file);
}
