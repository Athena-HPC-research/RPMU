#include "PMC.h"

void GetVoltageAndAmperes(char result[PMC_BUFFER_SIZE]) {
    char genc_result[1024];
    int result_character_index = 0;
    int equal_character_counter = 0;
    char amperes_measured = 0;
    
    for (int i = 0; i < PMC_BUFFER_SIZE; i++)
        result[i] = ' ';

    result[PMC_BUFFER_SIZE - 1] = '\0';

    gencmd(genc_result);

    for (int i = 0; i < 1024; i++) {
        if (genc_result[i] == '=')
            equal_character_counter++;

        if (equal_character_counter == 8 && amperes_measured == 0) {
            amperes_measured = 1;

            for (int j = i + 1; j < i + PMC_BUFFER_SIZE + 1; j++) {
                if (genc_result[j] == 'A')
                    break;
                
                result[result_character_index] = genc_result[j];
                result_character_index++;
            }

            result_character_index++;
        }
        
        if (equal_character_counter == 20) {
            for (int j = i + 1; j < i + PMC_BUFFER_SIZE + 1; j++) {
                if (genc_result[j] == 'V')
                    break;
                
                result[result_character_index] = genc_result[j];
                result_character_index++;
            }

            break;
        }
    }
}

void gencmd(char result[])
{
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
