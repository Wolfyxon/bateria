#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"

#ifndef BATTERY_DIR
#define BATTERY_DIR "/sys/class/power_supply"
#endif

#define BATTERY_DIR_LEN strlen(BATTERY_DIR)
#define BATTERY_MAX_FILE_LEN 512

#define BATTERY_STATUS_NAME_DISCHARGING "Discharging"

typedef enum BatteryError {
    BATTERY_OK,
    BATTERY_ERR_GENERIC,
    BATTERY_ERR_DIR_NOT_FOUND,
    BATTERY_ERR_NOT_FOUND,
    BATTERY_ERR_CANT_OPEN,
    BATTERY_ERR_FORMAT
} BatteryError;

char *battery_error_str(BatteryError error);
void battery_error_str_errno(char *dest, size_t max_len, BatteryError error);

bool battery_dir_exists();
bool battery_exists(char *battery_name);

size_t battery_get_path_len(char *battery_name);
void battery_get_path(char *dest, char *battery_name);

BatteryError battery_get_property(char* dest, char *battery_name, char* property_path);
BatteryError battery_get_int_property(int* dest, char *battery_name, char* property_path);

BatteryError battery_get_energy_now(int *dest, char *battery_name);
BatteryError battery_get_energy_max(int *dest, char *battery_name);
BatteryError battery_get_percentage(double *dest, char *battery_name);

BatteryError battery_is_discharging(bool *dest, char *battery_name);
