#include <stdio.h>
#include <string.h>
#include "dec128.h"
#include "test_dec128.h"

int main(void) {
    int failed = 0;
    printf("dec128: Testing exp10m1\n");
    failed += test_dec128_1(dec128_exp10m1, "1", "9", 0);
    failed += test_dec128_1(dec128_exp10m1, "10", "9999999999", 0);
    failed += test_dec128_1(dec128_exp10m1, "-10", "-0.9999999999", 0);
    failed += test_dec128_1(dec128_exp10m1, "100", "1e100", 0);
    failed += test_dec128_1(dec128_exp10m1, "-20", "-0.99999999999999999999", 0);
    failed += test_dec128_1(dec128_exp10m1, "-100", "-1", 0);
    failed += test_dec128_1(dec128_exp10m1, "-100e100", "-1", 0);
    failed += test_dec128_1(dec128_exp10m1, "1e-10", "2.302585093259140589562258141136554e-10", 0);
    failed += test_dec128_1(dec128_exp10m1, "3.33333333333333333333333333333333e-15", "7.675206890477116302765784608227366e-15", 0);
    failed += test_dec128_1(dec128_exp10m1, "1e-20", "2.302585092994045684017991454684364e-20", 0);
    failed += test_dec128_1(dec128_exp10m1, "1e-30", "2.302585092994045684017991454684364e-30", 0);
    failed += test_dec128_1(dec128_exp10m1, "0.95", "7.91250938133745529953108681078297", 0);
    failed += test_dec128_1(dec128_exp10m1, "0.10", "0.2589254117941672104239541063958006", 0);
    failed += test_dec128_1(dec128_exp10m1, "0.15", "0.4125375446227543021556078639302409", 0);
    failed += test_dec128_1(dec128_exp10m1, "-0.15", "-0.2920542156158620891977850578106873", 0);
    failed += test_dec128_1(dec128_exp10m1, "0.015", "0.03514216667934381727562347345431827", 0);
    failed += test_dec128_1(dec128_exp10m1, "0.333333333333333333333333333333333", "1.154434690031883721759293566519349", 0);
    printf("dec128: Testing log10\n");
    failed += test_dec128_1(dec128_log10, "2",    "0.301029995663981195213738894724493", 0); // approximate
    failed += test_dec128_1(dec128_log10, "0.5", "-0.301029995663981195213738894724493", 0); // approximate
    failed += test_dec128_1(dec128_log10, "20",   "1.301029995663981195213738894724493", 0); // approximate
    failed += test_dec128_1(dec128_log10, "200",  "2.301029995663981195213738894724493", 0); // approximate
    failed += test_dec128_1(dec128_log10, "2000", "3.301029995663981195213738894724493", 0); // approximate
    failed += test_dec128_1(dec128_log10, "1e100",   "100", 0);
    failed += test_dec128_1(dec128_log10, "1e-100", "-100", 0);
    failed += test_dec128_1(dec128_log10, "1.84e100", "100.2648178230095364645089937964247", 0);
    failed += test_dec128_1(dec128_log10, "3", "0.4771212547196624372950279032551153", 0);
    failed += test_dec128_1(dec128_log10,  "0", "-inf", 0); // error expected
    failed += test_dec128_1(dec128_log10, "-1", "nan", 0); // error expected
    
    report_failed(failed);

    return failed != 0;
}
