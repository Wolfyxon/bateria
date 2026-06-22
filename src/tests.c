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

void run_tests() {
    printf("Running tests\n");
    
    run_test(test_strtrimbounds);
    run_test(test_strslice);
    run_test(test_parse_int_single);
    run_test(test_parse_int_multi);
    run_test(test_parse_int_untrimmed);

    printf("All tests successful\n");
    exit(0);
}

#endif
