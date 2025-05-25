#include <stdio.h>
#include "dec128.h"
#include "test_dec128.h"

int main(void) {
    int failed = 0;
    
    printf("testing compare dec128\n");

    // EQUALITY
    failed += test_dec128_i2(dec128_equal, "1",   "1", 1, 0);
    failed += test_dec128_i2(dec128_equal, "1.0", "1", 1, 0);
    failed += test_dec128_i2(dec128_equal, "1",   "2", 0, 0);
    failed += test_dec128_i2(dec128_equal, "0",   "-0", 1, 0);
    failed += test_dec128_i2(dec128_equal, "nan", "nan", 0, 0);

    // LESS
    failed += test_dec128_i2(dec128_less, "1", "2", 1, 0);
    failed += test_dec128_i2(dec128_less, "1", "1", 0, 0);
    failed += test_dec128_i2(dec128_less, "2", "1", 0, 0);
    failed += test_dec128_i2(dec128_less, "-1", "0", 1, 0);
    failed += test_dec128_i2(dec128_less, "1", "10", 1, 0);

    // GREATER
    failed += test_dec128_i2(dec128_greater, "3", "1", 1, 0);
    failed += test_dec128_i2(dec128_greater, "3", "3", 0, 0);
    failed += test_dec128_i2(dec128_greater, "1", "3", 0, 0);

    // LESS OR EQUAL
    failed += test_dec128_i2(dec128_lessequal, "2", "2", 1, 0);
    failed += test_dec128_i2(dec128_lessequal, "-2", "-2", 1, 0);
    failed += test_dec128_i2(dec128_lessequal, "1", "2", 1, 0);
    failed += test_dec128_i2(dec128_lessequal, "2", "1", 0, 0);

    // GREATER OR EQUAL
    failed += test_dec128_i2(dec128_greaterequal, "2", "2", 1, 0);
    failed += test_dec128_i2(dec128_greaterequal, "3", "2", 1, 0);
    failed += test_dec128_i2(dec128_greaterequal, "2", "3", 0, 0);

    // INFINITIES
    failed += test_dec128_i2(dec128_greater, "inf", "1e20", 1, 0);
    failed += test_dec128_i2(dec128_less, "-inf", "0", 1, 0);
    failed += test_dec128_i2(dec128_greater, "inf", "inf", 0, 0);
    failed += test_dec128_i2(dec128_less, "-inf", "-inf", 0, 0);
    failed += test_dec128_i2(dec128_less, "-inf", "inf", 1, 0);

    // NaN Handing
    failed += test_dec128_i2(dec128_equal, "nan", "3", 0, 0);
    failed += test_dec128_i2(dec128_greater, "nan", "3", 0, 0);
    failed += test_dec128_i2(dec128_less, "nan", "3", 0, 0);

    // Signed zero(s)
    failed += test_dec128_i2(dec128_equal, "-0", "+0", 1, 0);
    failed += test_dec128_i2(dec128_less, "-0", "+0", 0, 0);
    failed += test_dec128_i2(dec128_greater, "-0", "+0", 0, 0);

    // EXPONENT/scaling
    failed += test_dec128_i2(dec128_equal, "123", "1.23e2", 1, 0);
    failed += test_dec128_i2(dec128_equal, "0.0001", "1e-4", 1, 0);

    report_failed(failed);
    return failed != 0;
}
