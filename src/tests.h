#pragma once

#ifdef ENABLE_TESTS

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "battery.h"

#define run_test(function) do { \
    printf("-- Running '%s' -- \n", #function); \
    function(); \
} while(0);

void run_tests();

#endif
