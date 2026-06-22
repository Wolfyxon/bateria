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
    
    check_duplicates(&state);
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

        if(streq(arg, "--version")) {
            printf("bateria v. %s\n", VERSION);
            exit(0);
        }

        if(streq(arg, "--help")) {
            print_help();
            exit(0);
        }
    }

    load_state_from_args(state, argc, argv);
}

void print_help() {
    puts("Usage: bateria  [--help] [--version] | [--interval <seconds>] [--target <percent> [--command <command>]]...");
    puts("");
    puts("General options:");
    puts("--help              : Shows this help");
    puts("--version           : Shows program version");
    puts("--battery  <name>   : Specifies which battery should be monitoreds");
    puts("--interval <seconds>: Specifies the interval between checks. Floats are supported");
    puts("");
    puts("Discharge target options:");
    puts("--target   <percent>: Initializes a target and specifies on what battery percent it should run");
    puts("--command  <command>: System command to run when the target is reached");
    puts("");
    puts("For bugs, help & source code visit:");
    puts("https://github.com/Wolfyxon/bateria");
}

void print_init(AppState *state) {
    print_targets(state);
    printf("Registered targets %ld\n", state->battery_targets_len);
    printf("Check interval: %fs\n", state->interval);
    printf("Used battery: %s\n", state->battery_name);
    
    double test_perc = -1;
    battery_get_percentage(&test_perc, state->battery_name);

    printf("Starting percentage: %f%%\n", test_perc);

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

void check_duplicates(AppState *state) {
    bool ok = true;

    for(size_t i = 0; i < state->battery_targets_len; i++) {
        for(size_t j = i; j < state->battery_targets_len; j++) {
            BatteryTarget *a = &state->battery_targets[i];
            BatteryTarget *b = &state->battery_targets[j];

            if(a == b) {
                continue;
            }

            if(a->percent == b->percent) {
                fprintf(stderr, "error: Duplicate targets for value %d%%\n", a->percent);
                ok = false;
            }
        }
    }

    if(!ok) {
        exit(1);
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

    percent = round(percent);

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
