#include "battery.h"

char *battery_error_str(BatteryError error) {
    switch(error) {
        case BATTERY_OK:
            return NULL;
        case BATTERY_ERR_DIR_NOT_FOUND:
            return "battery directory not found";
        case BATTERY_ERR_NOT_FOUND:
            return "file not found";
        case BATTERY_ERR_CANT_OPEN:
            return "unable to open file";
        case BATTERY_ERR_FORMAT:
            return "invalid format";
        default:
            return "unknown error";
    }
}

void battery_error_str_errno(char *dest, size_t max_len, BatteryError error) {
    char *str = battery_error_str(error);

    if(error != BATTERY_ERR_CANT_OPEN) {
        strncpy(dest, str, max_len);
        dest[max_len - 1] = '\0';
        return;
    }

    char *errno_str = strerror(errno);

    snprintf(dest, max_len, "%s: %s", str, errno_str);
    dest[max_len - 1] = '\0';
}

bool battery_dir_exists() {
    return dir_exists(BATTERY_DIR);
}

size_t battery_get_path_len(char *battery_name) {
    return BATTERY_DIR_LEN + strlen(battery_name) + 1; // + 1 for the / separator
}

void battery_get_path(char *dest, char *battery_name) {
    sprintf(dest, "%s/%s", BATTERY_DIR, battery_name);
}

bool battery_exists(char *battery_name) {
    if(!battery_dir_exists()) {
        return false;
    }

    size_t path_len = battery_get_path_len(battery_name);
    char path[path_len + 1];
    battery_get_path(path, battery_name);

    return dir_exists(path);
}

BatteryError battery_get_property(char* dest, char *battery_name, char* property_path) {
    size_t path_len = BATTERY_DIR_LEN + 1 + strlen(battery_name) + 1 + strlen(property_path); 
    char path[path_len + 1];

    sprintf(path, "%s/%s/%s", BATTERY_DIR, battery_name, property_path);

    FILE *file = fopen(path, "r");
    
    if(file == NULL) {
        return BATTERY_ERR_CANT_OPEN;
    }
    
    fread(dest, sizeof(char), BATTERY_MAX_FILE_LEN - 1, file);
    fclose(file);
    
    return BATTERY_OK;
}

BatteryError battery_get_int_property(int* dest, char *battery_name, char* property_path) {
    char buf[BATTERY_MAX_FILE_LEN] = {0};
    
    BatteryError err = battery_get_property(buf, battery_name, property_path);

    if(err != BATTERY_OK) {
        return err;
    }
    
    bool success = parse_int(dest, buf);
    
    if(!success) {
        return BATTERY_ERR_FORMAT;
    }

    return BATTERY_OK;
}

BatteryError battery_get_energy_now(int *dest, char* battery_name) {
    int val;
    BatteryError err = battery_get_int_property(&val, battery_name, "energy_now");

    if(err != BATTERY_OK) {
        return err;
    }

    *dest = val;
    return BATTERY_OK;
}

BatteryError battery_get_energy_max(int *dest, char* battery_name) {
    int val;
    BatteryError err = battery_get_int_property(&val, battery_name, "energy_full");

    if(err != BATTERY_OK) {
        return err;
    }

    *dest = val;
    return BATTERY_OK;
}

BatteryError battery_get_percentage(double *dest, char* battery_name) {
    int now;
    int max;
    
    BatteryError now_err = battery_get_energy_now(&now, battery_name);
    BatteryError max_err = battery_get_energy_max(&max, battery_name);
    
    if(now_err != BATTERY_OK) {
        return now_err;
    }

    if(max_err != BATTERY_OK) {
        return max_err;
    }

    *dest = ((double)now / (double)max) * 100.0;
    return BATTERY_OK;
}

BatteryError battery_is_discharging(bool *dest, char *battery_name) {
    char buf[BATTERY_MAX_FILE_LEN] = {0};
    BatteryError err = battery_get_property(buf, battery_name, "status");

    if(err != BATTERY_OK) {
        return err;
    }

    *dest = strncmp(buf, BATTERY_STATUS_NAME_DISCHARGING, strlen(BATTERY_STATUS_NAME_DISCHARGING)) == 0;;
    
    return BATTERY_OK;
}
