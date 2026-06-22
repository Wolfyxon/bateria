#include "tests.h"

#ifdef ENABLE_TESTS

void test_strtrimbounds() {
    size_t start, end = 0;

    char *str = " hello there\n ";
    strtrimbounds(str, &start, &end);

    if(start != 1) {
        fprintf(stderr, "start should be 1, got %ld\n", start);
        abort();
    }

    if(end != 12) {
        fprintf(stderr, "end should be 12, got %ld\n", end);
        abort();
    }
}

void test_strslice() {
    char *str = "hi there haha lol this is so sigma";

    char res[strlen(str) + 1];
    strslice(res, str, 3, 8);

    if(strcmp(res, "there") != 0) {
        fprintf(stderr, "Expected 'there' got: '%s'\n", res);
        abort();
    }
}

void test_parse_int_single() {
    char *str = "8";
    int res = 0;

    if(!parse_int(&res, str)) {
        fprintf(stderr, "parse_int returned fail\n");
        abort();
    }

    if(res != 8) {
        fprintf(stderr, "Expected 8, got %d\n", res);
        abort();
    }
}

void test_parse_int_multi() {
    char *str = "8809";
    int res = 0;

    if(!parse_int(&res, str)) {
        fprintf(stderr, "parse_int returned fail\n");
        abort();
    }

    if(res != 8809) {
        fprintf(stderr, "Expected 8809, got %d\n", res);
        abort();
    }
}

void test_parse_int_untrimmed() {
    char *str = "\n 1254   \n";
    int res = 0;

    if(!parse_int(&res, str)) {
        fprintf(stderr, "parse_int returned fail\n");
        abort();
    }

    if(res != 1254) {
        fprintf(stderr, "Expected 1254, got %d\n", res);
        abort();
    }
}

void test_bat_init() {
    if(streq(BATTERY_DIR, "/sys/class/power_supply")) {
        fprintf(stderr, "BATTERY_DIR is normal. Please compile the binary in mock mode to test battery checks\n");
        abort();
    }

    if(!battery_dir_exists()) {
        fprintf(stderr, "Battery directory doesn't exist\n");
        abort();
    }

    if(!battery_exists(TEST_BAT)) {
        fprintf(stderr, "Battery '%s' not found\n", TEST_BAT);
        abort();
    }
}

// Values as in the Makefile prepare-mock target
void test_bat_percent() {
    double perc = -1;
    BatteryError err = battery_get_percentage(&perc, TEST_BAT);

    if(err != BATTERY_OK) {
        fprintf(stderr, "battery_get_percentage() failed: %s\n", battery_error_str(err));
        abort();
    }

    if(perc != 50) {
        fprintf(stderr, "Expected 50%% got %f%%\n", perc);
        abort();
    }
}

void test_bat_state() {
    bool discharging = true;
    BatteryError err = battery_is_discharging(&discharging, TEST_BAT);

    if(err != BATTERY_OK) {
        fprintf(stderr, "battery_is_discharging() failed: %s\n", battery_error_str(err));
        abort();
    }

    if(discharging != false) {
        fprintf(stderr, "Battery should be charging\n");
        abort();
    }
}

void run_tests() {
    printf("Running tests\n");
    
    run_test(test_strtrimbounds);
    run_test(test_strslice);
    run_test(test_parse_int_single);
    run_test(test_parse_int_multi);
    run_test(test_parse_int_untrimmed);

    run_test(test_bat_init);
    run_test(test_bat_percent);

    printf("All tests successful\n");
    exit(0);
}

#endif
