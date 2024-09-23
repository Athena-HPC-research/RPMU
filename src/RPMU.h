#ifndef rpmuh
#define rpmuh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "buffer.h"

#define RPMU_READINGS_DIGIT_ACCURACY 4 // DO NOT INCREASE THIS BEYOND 8
#define RPMU_BUFFER_SIZE 8
#define RPMU_GENCMD_BUFFER_SIZE 1024

/// @brief Gets the ampers consumed by the CPU
/// @param genc_result The PMIC readings output
/// @return The ampers consumed by the CPU
double GetAmpers(char* genc_result);

/// @brief Gets the voltage the CPU runs at
/// @param genc_result The PMIC readings output
/// @return The voltage the CPU runs at
double GetVoltage(char* genc_result);

/// @brief Read the readings from the PMIC
/// @param result WORKS AS AN OUTPUT: The readings of the PMIC without any processing done to it
void gencmd(char* result);

/// @brief The terminal mode of the RPMU
void TerminalMode();

/// @brief The daemon mode of the RPMU, generates a snapshot of the consumption every minute the daemon mode runs
/// @param filename The snapshot file to be created with all the readings
/// @param minutes_to_run The minutes to run in daemon mode
void DaemonMode(char* filename, int minutes_to_run);

#endif