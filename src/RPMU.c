#include "RPMU.h"

double GetAmpers(char* genc_result) {
    char* ampers = CreateBuffer(RPMU_BUFFER_SIZE);
    int equal_character_counter = 0;
    int start_index = 0;

    for (start_index = 0; start_index < RPMU_GENCMD_BUFFER_SIZE && equal_character_counter < 8; start_index++) {
        if (genc_result[start_index] == '=') {
            equal_character_counter++;
        }
    }

    for (int i = start_index; i < start_index + RPMU_BUFFER_SIZE + 1; i++) {
        if (genc_result[i] == '.') {
            ampers[i - start_index] = genc_result[i];

            for (int x = 0; x < RPMU_READINGS_DIGIT_ACCURACY; x++) {
                ampers[i - start_index + x + 1] = genc_result[i + x + 1];
            }

            break;
        }
        
        ampers[i - start_index] = genc_result[i];
    }

    return atof(ampers);
}

double GetVoltage(char* genc_result) {
    char* voltage = CreateBuffer(RPMU_BUFFER_SIZE);
    int equal_character_counter = 0;
    int start_index = 0;

    for (start_index = 0; start_index < RPMU_GENCMD_BUFFER_SIZE && equal_character_counter < 20; start_index++) {
        if (genc_result[start_index] == '=') {
            equal_character_counter++;
        }
    }

    for (int i = start_index; i < start_index + RPMU_BUFFER_SIZE + 1; i++) {
        if (genc_result[i] == '.') {
            voltage[i - start_index] = genc_result[i];

            for (int x = 0; x < RPMU_READINGS_DIGIT_ACCURACY; x++) {
                voltage[i - start_index + x + 1] = genc_result[i + x + 1];
            }

            break;
        }
        
        voltage[i - start_index] = genc_result[i];
    }

    return atof(voltage);
}

void gencmd(char result[]) {
    int file_desc = open("/dev/vcio", 0);
    unsigned packet[263];
    
    if (file_desc < 0) {
        printf("Error opening device file\n");
        exit(-1);
    }

    packet[1] = 0;
    packet[2] = 0x00030080;
    packet[3] = 1024;
    packet[4] = 0;
    packet[5] = 0;
    packet[262] = 0;
    packet[0] = 263 * sizeof *packet;
    result[0] = 0;

    memcpy(packet + 6, "pmic_read_adc", 14);

    if (ioctl(file_desc, _IOWR(100, 0, char *), packet) < 0) {
        printf("ioctl error: %i\n", 0);
        exit(-1);
    } 

    strncat(result, (const char *) (packet + 6), 1024);
    close(file_desc);
}

void TerminalMode() {
    char* gencmd_result = CreateBuffer(RPMU_GENCMD_BUFFER_SIZE);

    while (1) {
        gencmd(gencmd_result);
        printf("%fV %fA\n", GetVoltage(gencmd_result), GetAmpers(gencmd_result));
        sleep(1);
    }
}

void DaemonMode(char* filename, int minutes_to_run) {
    char* filenameBuffer = CreateBuffer(200);
    char* filenameCounterBuffer = CreateBuffer(200);
    int filenameCounter = 0;
    char* gencmd_result = CreateBuffer(RPMU_GENCMD_BUFFER_SIZE);
    char* voltage = CreateBuffer(RPMU_BUFFER_SIZE);
    char* ampers = CreateBuffer(RPMU_BUFFER_SIZE);

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
            fprintf(output_file, "%i %f %f\n", time(NULL), GetVoltage(gencmd_result), GetAmpers(gencmd_result));
            sleep(1);
        }

        fclose(output_file);
    }
}