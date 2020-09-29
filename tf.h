#ifndef TF_H
#define TF_H

#include <stdio.h>
#include <unistd.h>

#define F_TEMP "/sys/devices/platform/thinkpad_hwmon/hwmon/hwmon1/temp1_input"
#define F_FAN "/proc/acpi/ibm/fan"

#define SLEEP_TIME 6

// Fan levels
enum {
    LEVEL_0, LEVEL_1, LEVEL_2, LEVEL_3, LEVEL_4,
    LEVEL_5, LEVEL_6, LEVEL_7
};

// Struct config items
typedef struct config_i {
    int min;
    int max;
} config_i;

// Config (Change to suit your needs)
config_i config[] = {
    {0,  46}, // Level 0
    {41, 51}, // Level 1
    {45, 56}, // Level 2
    {51, 61}, // Level 3
    {55, 64}, // Level 4
    {60, 66}, // Level 5
    {63, 68}, // Level 6
    {65, 74}  // Level 7
};

// Write to fan file
void fan_write(char *msg) {
    FILE *fan = fopen(F_FAN, "w");
    if (fan == NULL) {
        fprintf(stderr, "Failed to open %s.\n", F_FAN);
    }

    fprintf(fan, "%s\n", msg);
    fclose(fan);
}

// Get CPU temp
int get_temp() {
    FILE *temp = fopen(F_TEMP, "r");
    if (temp == NULL) {
        fprintf(stderr, "Failed to open %s.\n", F_TEMP);
    }

    int temp_c; 
    fscanf(temp, "%d", &temp_c);
    temp_c /= 1000;

    fclose(temp);

    return temp_c;
}

#endif