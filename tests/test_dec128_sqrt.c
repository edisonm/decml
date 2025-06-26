#include <stdio.h>
#include <string.h>
#include "dec128.h"
#include "test_dec128.h"

int main(void) {
    int failed = 0;
    printf("dec128: Testing sqrt\n");
    failed += test_dec128_1(dec128_sqrt, "0", "0", 0);
    failed += test_dec128_1(dec128_sqrt, "1", "1", 0);
    failed += test_dec128_1(dec128_sqrt, "4", "2", 0);
    failed += test_dec128_1(dec128_sqrt, "2", "1.414213562373095048801688724209698", 0);
    failed += test_dec128_1(dec128_sqrt, "3", "1.732050807568877293527446341505872", 0);
    failed += test_dec128_1(dec128_sqrt, "123.21", "11.1", 0);
    failed += test_dec128_1(dec128_sqrt, "8100000000000000", "90000000",  0); // big exact
    failed += test_dec128_1(dec128_sqrt, "8112963841460666", "90071992.54740990795641775453986985", 0); // approximate
    failed += test_dec128_1(dec128_sqrt, "8112963841460667", "90071992.54740991350753287766565313", 0); // approximate
    failed += test_dec128_1(dec128_sqrt,  "999999999999999", "31622776.60168377750860063460242657", 0); // approximate
    failed += test_dec128_1(dec128_sqrt, "-1", "nan", 0); // nan expected

    report_failed(failed);

    return failed != 0;
}
