#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

typedef struct {
    int percent;
    char *message;
    char *command;
} BatteryTarget;

typedef struct {
    float interval;
    char *battery_name;
    BatteryTarget *battery_targets;
    BatteryTarget *last_battery_target;
    size_t battery_targets_len;
} AppState;

void load_state_from_args(AppState *state, int argc, char **argv);

void add_target(AppState *state, BatteryTarget target);
void clear_targets(AppState *state);

AppState get_default_state();
BatteryTarget battery_target_new_default(int percent, char *message);
