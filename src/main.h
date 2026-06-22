#pragma once

#include <stdio.h>

#include "config.h"
#include "battery.h"
#include "tests.h"

int main(int argc, char **argv);
void process_args(AppState *state, int argc, char **argv);
void print_help();
void print_init(AppState *state);

void battery_target_exec(AppState *state, BatteryTarget *target);
bool should_loop(AppState *state);
BatteryTarget *get_active_battery_target(AppState *state);
void main_loop(AppState *state);
