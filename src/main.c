#include "main.h"

int main(int argc, char **argv) {
    #ifdef ENABLE_TESTS
    run_tests();
    #endif

    AppState state = get_default_state();

    process_args(&state, argc, argv);

    if(!battery_dir_exists()) {
        fprintf(stderr, "error: Battery directory '%s' not found. What the heck?\n", BATTERY_DIR);
        exit(1);
    }

    if(!battery_exists(state.battery_name)) {
        fprintf(stderr, "error: Battery '%s' not found. Use --battery to change it.\n", state.battery_name);
        exit(1);
    }
    
    if(state.battery_targets_len == 0) {
        fprintf(stderr, "error: No battery discharge targets configured.\n");
        exit(1);
    }
    
    print_init(&state);

    #ifdef BUILD_WARNING
    fprintf(stderr, "warning: %s\n", BUILD_WARNING);
    #endif

    puts("\nBattery guard started\n");

    while(1) {
        main_loop(&state);
    }
}

void process_args(AppState *state, int argc, char **argv) {
    for(size_t i = 0; i < argc; i++) {
        char *arg = argv[i];

        if(streq(arg, "--help")) {
            print_help();
            exit(0);
        }
    }

    load_state_from_args(state, argc, argv);
}

void print_help() {
    puts("no help yet eh");
}

void print_init(AppState *state) {
    printf("Registered targets %ld:\n", state->battery_targets_len);
    print_targets(state);

    for(int i = 0; i < state->battery_targets_len; i++) {
        BatteryTarget *tg = &state->battery_targets[i];

        if(tg->command == NULL) {
            fprintf(
                stderr, 
                "warning: Some targets have no command configured. You will only get stdout messages and NO notifications!\n"
            );
            break;
        }
    }
}

void print_targets(AppState *state) {
    printf("---------------------------\n");

    for(size_t i = 0; i < state->battery_targets_len; i++) {
        BatteryTarget target = state->battery_targets[i];
        
        printf("Percent: %d%%\n", target.percent);
        printf("Message: %s\n", target.message);
        printf("Command: %s\n", target.command);
        
        printf("---------------------------\n");
    }
}

bool should_loop(AppState *state) {
    bool discharging;
    BatteryError err = battery_is_discharging(&discharging, state->battery_name);

    if(err != BATTERY_OK) {
        char err_str[64] = {0};
        battery_error_str_errno(err_str, sizeof(err_str), err);

        fprintf(stderr, "error: Failed to check battery status: %s\n", err_str);
        exit(1);
    }

    return discharging;
}

BatteryTarget *get_active_battery_target(AppState *state) {
    double percent;
    BatteryError err = battery_get_percentage(&percent, state->battery_name);

    if(err != BATTERY_OK) {
        char err_str[64] = {0};
        battery_error_str_errno(err_str, sizeof(err_str), err);
        
        fprintf(stderr, "error: Failed to get battery percentage: %s\n", err_str);
        return NULL;
    }

    BatteryTarget *current_target = NULL;

    for(size_t i = 0; i < state->battery_targets_len; i++) {
        BatteryTarget *t = &state->battery_targets[i];
        
        if(percent <= t->percent) {
            current_target = t;
        }
    }

    return current_target;
}

void main_loop(AppState *state) {
    wait(state->interval);

    if(!should_loop(state)) {
        state->last_battery_target = NULL;
        return;
    }

    BatteryTarget *current_target = get_active_battery_target(state);

    if(current_target != NULL && current_target != state->last_battery_target) {
        state->last_battery_target = current_target;
        battery_target_exec(state, current_target);
    }
}

void battery_target_exec(AppState *state, BatteryTarget *target) {
    if(target->message != NULL) {
        printf("%s\n", target->message);
    }
    
    if(target->command != NULL) {
        system(target->command);
    }
}
