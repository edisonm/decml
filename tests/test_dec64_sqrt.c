#include <stdio.h>
#include <string.h>
#include "dec64.h"
#include "test_dec64.h"

int main(void) {
    int failed = 0;
    
    printf("testing sqrt dec64\n");
    failed += test_dec64_1(dec64_sqrt, "0", "0", 0);
    failed += test_dec64_1(dec64_sqrt, "1", "1", 0);
    failed += test_dec64_1(dec64_sqrt, "4", "2", 0);
    failed += test_dec64_1(dec64_sqrt, "2", "1.414213562373095", 0);
    failed += test_dec64_1(dec64_sqrt, "3", "1.732050807568877", 0);
    failed += test_dec64_1(dec64_sqrt, "123.21", "11.1", 0);
    failed += test_dec64_1(dec64_sqrt, "8100000000000000", "90000000",  0); // big exact
    failed += test_dec64_1(dec64_sqrt, "8112963841460666", "90071992.54740991", 0); // approximate
    failed += test_dec64_1(dec64_sqrt, "8112963841460667", "90071992.54740991", 0); // approximate
    failed += test_dec64_1(dec64_sqrt,  "999999999999999", "31622776.60168378", 0); // approximate
    failed += test_dec64_1(dec64_sqrt, "-1", "nan", 0); // nan expected

    report_failed(failed);

    return failed != 0;
}
