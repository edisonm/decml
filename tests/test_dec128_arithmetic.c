#include <stdio.h>
#include <string.h>
#include "dec128.h"
#include "test_dec128.h"

int main() {
    int failed = 0;
    printf("testing arithmetic intern_dec128\n");
    /*
    intern_dec128_t
        a = {0, 2, 0, DEC_NORMAL},
        b = {0, 25*__pow10_dec128[37]-525000, -22, DEC_NORMAL},
        c;
    intern_dec128_div(&c, &a, &b);
    __print_dec128(c); printf("\n");
    */
    failed += test_i_dec128_2(intern_dec128_add, "1", "0", "1", 0); // subtraction 1 - 0 = 0
    failed += test_i_dec128_2(intern_dec128_div, "40404040404040404040404040404040404040e-38", "95918783797571676359555147433935312724e-38",
                              "0.42123178385278161897670460589299010742", 0);
    failed += test_i_dec128_2(intern_dec128_div, "2e0", "-249999999999999999999999999999999475000e-22", "-8.0000000000000000000000000000000168e-17", 0); // subtraction 1 - 0 = 0
    printf("testing arithmetic dec128\n");
    failed += test_dec128_2(dec128_add, "1", "0", "1", 0); // subtraction 1 - 0 = 0
    failed += test_dec128_2(dec128_add, "123.45", "67.89", "191.34", 0); // addition 123.45 + 67.89 = 191.34
    failed += test_dec128_2(dec128_add, "0.09590083957724163", "0.09590083957724163", "0.19180167915448326", 0);
    failed += test_dec128_2(dec128_sub, "1000", "1", "999", 0); // subtraction 1000 - 1 = 999
    failed += test_dec128_2(dec128_sub, "-0.866025403784439", "-1.154700538379251", "0.288675134594812", 0); // subtraction
    failed += test_dec128_2(dec128_mul, "12", "3.5", "42", 0); // multiplication 12 * 3.5 = 42
    failed += test_dec128_2(dec128_mul, "18446744073709551615", "18446744073709551615", "3.402823669209384634264811192843491e38", 0);
    failed += test_dec128_2(dec128_mul, "1.8e100", "1.8e100", "3.24e200", 0);
    failed += test_dec128_2(dec128_div, "1", "-200000000000000000000000000000000020000e-55", "-50000000000000000", 0);
    failed += test_dec128_2(dec128_div, "1", "1", "1", 0);
    failed += test_dec128_2(dec128_div, "1", "81", "0.01234567901234567901234567901234568", 0);
    failed += test_dec128_2(dec128_div, "10", "4", "2.5", 0); // division 10 / 4 = 2.5
    failed += test_dec128_2(dec128_div, "1", "0", "inf", 0); // division by zero
    failed += test_dec128_2(dec128_div, "-1", "0", "-inf", 0); // division by zero
    failed += test_dec128_2(dec128_div, "0", "0", "nan", 0); // division by zero
    failed += test_dec128_2i(dec128_powi, "2", 10, "1024", 0); // 2^10 = 1024
    failed += test_dec128_2i(dec128_powi, "2", 20, "1048576", 0); // 2^20 = 1048576
    failed += test_dec128_2i(dec128_powi, "5", -3, "0.008", 0); // 5^-3 = 0.008
    failed += test_dec128_2i(dec128_powi, "10", 0, "1", 0); // 10^0 = 1
    failed += test_dec128_2i(dec128_powi, "0", 3, "0", 0); // 0^3 = 0
    failed += test_dec128_2i(dec128_powi, "1.84", __pow10_dec128[3], "6.573898729783670592147704176165426e264", 0); // 2^1 0 = 1024

    report_failed(failed);
    
    return failed != 0;
}
