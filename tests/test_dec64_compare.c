#include <stdio.h>
#include "dec64.h"
#include "test_dec64.h"

int main(void) {
    int failed = 0;
    
    printf("testing compare dec64\n");

    // EQUALITY
    failed += test_dec64_i2(dec64_equal, "1",   "1", 1, 0);
    failed += test_dec64_i2(dec64_equal, "1.0", "1", 1, 0);
    failed += test_dec64_i2(dec64_equal, "1",   "2", 0, 0);
    failed += test_dec64_i2(dec64_equal, "0",   "-0", 1, 0);
    failed += test_dec64_i2(dec64_equal, "nan", "nan", 0, 0);

    // LESS
    failed += test_dec64_i2(dec64_less, "1", "2", 1, 0);
    failed += test_dec64_i2(dec64_less, "1", "1", 0, 0);
    failed += test_dec64_i2(dec64_less, "2", "1", 0, 0);
    failed += test_dec64_i2(dec64_less, "-1", "0", 1, 0);
    failed += test_dec64_i2(dec64_less, "1", "10", 1, 0);

    // GREATER
    failed += test_dec64_i2(dec64_greater, "3", "1", 1, 0);
    failed += test_dec64_i2(dec64_greater, "3", "3", 0, 0);
    failed += test_dec64_i2(dec64_greater, "1", "3", 0, 0);

    // LESS OR EQUAL
    failed += test_dec64_i2(dec64_lessequal, "2", "2", 1, 0);
    failed += test_dec64_i2(dec64_lessequal, "-2", "-2", 1, 0);
    failed += test_dec64_i2(dec64_lessequal, "1", "2", 1, 0);
    failed += test_dec64_i2(dec64_lessequal, "2", "1", 0, 0);

    // GREATER OR EQUAL
    failed += test_dec64_i2(dec64_greaterequal, "2", "2", 1, 0);
    failed += test_dec64_i2(dec64_greaterequal, "3", "2", 1, 0);
    failed += test_dec64_i2(dec64_greaterequal, "2", "3", 0, 0);

    // INFINITIES
    failed += test_dec64_i2(dec64_greater, "inf", "1e20", 1, 0);
    failed += test_dec64_i2(dec64_less, "-inf", "0", 1, 0);
    failed += test_dec64_i2(dec64_greater, "inf", "inf", 0, 0);
    failed += test_dec64_i2(dec64_less, "-inf", "-inf", 0, 0);
    failed += test_dec64_i2(dec64_less, "-inf", "inf", 1, 0);

    // NaN Handing
    failed += test_dec64_i2(dec64_equal, "nan", "3", 0, 0);
    failed += test_dec64_i2(dec64_greater, "nan", "3", 0, 0);
    failed += test_dec64_i2(dec64_less, "nan", "3", 0, 0);

    // Signed zero(s)
    failed += test_dec64_i2(dec64_equal, "-0", "+0", 1, 0);
    failed += test_dec64_i2(dec64_less, "-0", "+0", 0, 0);
    failed += test_dec64_i2(dec64_greater, "-0", "+0", 0, 0);

    // EXPONENT/scaling
    failed += test_dec64_i2(dec64_equal, "123", "1.23e2", 1, 0);
    failed += test_dec64_i2(dec64_equal, "0.0001", "1e-4", 1, 0);

    report_failed(failed);
    return failed != 0;
}
