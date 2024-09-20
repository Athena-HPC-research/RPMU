#include "RPMU.h"

void GetAmpers(char* genc_result, char* ampers) {
    int equal_character_counter = 0;
    int start_index = 0;
    
    EmptyBuffer(ampers, RPMU_BUFFER_SIZE);

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
}

void GetVoltage(char* genc_result, char* voltage) {
    int equal_character_counter = 0;
    int start_index = 0;

    EmptyBuffer(voltage, RPMU_BUFFER_SIZE);

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