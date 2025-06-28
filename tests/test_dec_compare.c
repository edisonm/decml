#include <stdio.h>
#include "dec64.h"
#include "dec128.h"
#include "test_dec.h"
#include "test_dec64.h"
#include "test_dec128.h"

int main(void) {
    int failed = 0;

    tests_i2_t tests_i2_equal[] = {
        {"1",   "1", 1},
        {"1.0", "1", 1},
        {"1",   "2", 0},
        {"0",   "-0", 1},
        {"nan", "nan", 0},
        {"nan", "3", 0},
        {"-0", "+0", 1},
        {"123", "1.23e2", 1},
        {"0.0001", "1e-4", 1}
    };
    
    tests_i2_t tests_i2_less[] = {
        {"1", "2", 1},
        {"1", "1", 0},
        {"2", "1", 0},
        {"-1", "0", 1},
        {"1", "10", 1},
        {"-inf", "0", 1},
        {"-inf", "-inf", 0},
        {"-inf", "inf", 1},
        {"nan", "3", 0},
        {"-0", "+0", 0}
    };
    
    tests_i2_t tests_i2_greater[] = {
        {"3", "1", 1},
        {"3", "3", 0},
        {"1", "3", 0},
        {"inf", "1e20", 1},
        {"inf", "inf", 0},
        {"nan", "3", 0},
        {"-0", "+0", 0}
    };
    
    tests_i2_t tests_i2_lessequal[] = {
        {"2", "2", 1},
        {"-2", "-2", 1},
        {"1", "2", 1},
        {"2", "1", 0}
    };
    
    tests_i2_t tests_i2_greaterequal[] = {
        {"2", "2", 1},
        {"3", "2", 1},
        {"2", "3", 0}
    };
    
    RUN_TESTS_i2(tests_i2, equal);
    RUN_TESTS_i2(tests_i2, less);
    RUN_TESTS_i2(tests_i2, greater);
    RUN_TESTS_i2(tests_i2, lessequal);
    RUN_TESTS_i2(tests_i2, greaterequal);

    report_failed(failed);
    return failed != 0;
}
