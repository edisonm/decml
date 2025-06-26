#include <stdio.h>
#include <string.h>
#include "dec64.h"
#include "test_dec64.h"

int main(void) {
    int failed = 0;
    printf("dec64: Testing exp10m1\n");
    failed += test_dec64_1(dec64_exp10m1, "1", "9", 0);
    failed += test_dec64_1(dec64_exp10m1, "10", "9999999999", 0);
    failed += test_dec64_1(dec64_exp10m1, "-10", "-0.9999999999", 0);
    failed += test_dec64_1(dec64_exp10m1, "100", "1e100", 0);
    failed += test_dec64_1(dec64_exp10m1, "-20", "-1", 0);
    failed += test_dec64_1(dec64_exp10m1, "-100", "-1", 0);
    failed += test_dec64_1(dec64_exp10m1, "-100e100", "-1", 0);
    failed += test_dec64_1(dec64_exp10m1, "1e-10", "2.302585093259141e-10", 0);
    failed += test_dec64_1(dec64_exp10m1, "3.333333333333333e-15", "7.675283643313514e-15", 0);
    failed += test_dec64_1(dec64_exp10m1, "1e-20", "2.302585092994046e-20", 0);
    failed += test_dec64_1(dec64_exp10m1, "1e-30", "2.302585092994046e-30", 0);
    failed += test_dec64_1(dec64_exp10m1, "0.95", "7.912509381337455", 0);
    failed += test_dec64_1(dec64_exp10m1, "0.10", "0.2589254117941672", 0);
    failed += test_dec64_1(dec64_exp10m1, "0.15", "0.4125375446227543", 0);
    failed += test_dec64_1(dec64_exp10m1, "-0.15", "-0.2920542156158621", 0);
    failed += test_dec64_1(dec64_exp10m1, "0.015", "0.03514216667934382", 0);
    failed += test_dec64_1(dec64_exp10m1, "0.3333333333333333", "1.154434690031884", 0);
    printf("dec64: Testing log10\n");
    failed += test_dec64_1(dec64_log10, "2",    "0.3010299956639812", 0); // approximate
    failed += test_dec64_1(dec64_log10, "0.5", "-0.3010299956639812", 0); // approximate
    failed += test_dec64_1(dec64_log10, "20",   "1.301029995663981", 0); // approximate
    failed += test_dec64_1(dec64_log10, "200",  "2.301029995663981", 0); // approximate
    failed += test_dec64_1(dec64_log10, "2000", "3.301029995663981", 0); // approximate
    failed += test_dec64_1(dec64_log10, "1e100",   "100", 0);
    failed += test_dec64_1(dec64_log10, "1e-100", "-100", 0);
    failed += test_dec64_1(dec64_log10, "1.84e100", "100.2648178230095", 0);
    failed += test_dec64_1(dec64_log10, "3", "0.4771212547196624", 0);
    failed += test_dec64_1(dec64_log10,  "0", "-inf", 0); // error expected
    failed += test_dec64_1(dec64_log10, "-1", "nan", 0); // error expected
    
    report_failed(failed);

    return failed != 0;
}
