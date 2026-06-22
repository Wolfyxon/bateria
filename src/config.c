#include "config.h"

void load_state_from_args(AppState *state, int argc, char **argv) {
    BatteryTarget new_target = {0};
    bool has_new_target = false;
    bool targets_added = false;

    for(size_t i = 1; i < argc; i++) {
        char *arg = argv[i];

        if(streq(arg, "--battery")) {
            if(argc < i + 2) {
                fprintf(stderr, "error: Usage: --battery <battery sys name>\n");
                exit(1);
            }

            char *bat_arg = argv[i + 1];

            size_t len = strlen(bat_arg);
            char *buf = malloc(len + 1);

            if(buf == NULL) {
                fprintf(stderr, "error: Unable to create battery name buffer\n");
                exit(1);
            }

            strncpy(buf, bat_arg, len);
            buf[len] = '\0';

            state->battery_name = buf;
            i++;
        }

        else if(streq(arg, "--target")) {
            if(!targets_added) {
                clear_targets(state);
                targets_added = true;
            }

            if(has_new_target) {
                add_target(state, new_target);
                has_new_target = false;
            }

            if(argc < i + 2) {
                fprintf(stderr, "error: Usage: --target <percent number>\n");
                exit(1);
            }

            memset(&new_target, 0, sizeof(BatteryTarget));
            has_new_target = true;
            
            char *percent_str = argv[i + 1];

            if(!parse_int(&new_target.percent, percent_str)) {
                fprintf(stderr, "error: --target: Invalid percent number '%s'\n", percent_str);
                exit(1);
            }

            char *prefix = "Battery at ";
            char *msg_buf = malloc(strlen(prefix) + strlen(percent_str) + 2);

            if(msg_buf == NULL) {
                fprintf(stderr, "error: Failed to create message buffer\n");
                exit(1);
            }

            sprintf(msg_buf, "%s%s%%", prefix, percent_str);

            new_target.message = msg_buf;
            i++;
        }

        else if(streq(arg, "--command")) {
            if(!has_new_target) {
                fprintf(stderr, "error: --command must be used after --target");
                exit(1);
            }

            if(argc < i + 2) {
                fprintf(stderr, "error: Usage: --command <command>\n");
                exit(1);
            }
            
            char *cmd_src = argv[i + 1];

            size_t len = strlen(cmd_src);
            char *buf = malloc(len + 1);

            if(buf == NULL) {
                fprintf(stderr, "error: Failed to create command buffer\n");
                exit(1);
            }

            strncpy(buf, cmd_src, len);
            buf[len] = '\0';

            new_target.command = buf;

            i++;
        }

        else {
            fprintf(stderr, "error: Unknown argument '%s'\n", arg);
            exit(1);
        }
    }

    if(has_new_target) {
        add_target(state, new_target);
    }
}

void add_target(AppState *state, BatteryTarget target) {
    BatteryTarget *new_array = realloc(state->battery_targets, (++state->battery_targets_len) * sizeof(BatteryTarget));

    if(new_array == NULL) {
        fprintf(stderr, "error: Failed to add target. Unable to extend buffer\n");
        exit(1);
    }
    
    new_array[state->battery_targets_len - 1] = target;

    state->battery_targets = new_array;
}

void clear_targets(AppState *state) {
    state->battery_targets_len = 0;

    /*BatteryTarget *new_array = realloc(state->battery_targets, 0);
    
    if(new_array == NULL) {
        fprintf(stderr, "error: Unable to shrink battery target array buffer\n");
        return;
    }

    state->battery_targets = new_array;*/
}

AppState get_default_state() {
    BatteryTarget *targets = malloc(sizeof(BatteryTarget) * 3);
    
    targets[0] = battery_target_new_default(25, "25%, Battery is running low");
    targets[1] = battery_target_new_default(10, "10%, battery critical");
    targets[2] = battery_target_new_default(5, "5%, CONNECT CHARGER NOW");
    
    if(targets == NULL) {
        fprintf(stderr, "error: Failed to initialize default battery targets");
        exit(1);
    }

    AppState state = {
        .battery_name = "BAT0",
        .interval = 0,
        .battery_targets = targets,
        .battery_targets_len = 3
    };

    return state;
}

BatteryTarget battery_target_new_default(int percent, char *message) {
    char *msg_buf = malloc(strlen(message) + 1);

    if(msg_buf == NULL) {
        fprintf(stderr, "error: Failed to allocate memory for message buffer\n");
        exit(1);
    }
    
    strcpy(msg_buf, message);

    BatteryTarget target = {
        .percent = percent,
        .message = msg_buf
    };

    return target;
}

