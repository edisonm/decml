#include <stdio.h>
#include <string.h>
#include "dec64.h"
#include "test_dec64.h"

int main() {
    int failed = 0;
    printf("testing arithmetic dec64\n");
    failed += test_dec64_2(dec64_add, "1", "0", "1", 0); // subtraction 1 - 0 = 0
    failed += test_dec64_2(dec64_add, "123.45", "67.89", "191.34", 0); // addition 123.45 + 67.89 = 191.34
    failed += test_dec64_2(dec64_add, "0.09590083957724163", "0.09590083957724163", "0.1918016791544833", 0);
    failed += test_dec64_2(dec64_sub, "1000", "1", "999", 0); // subtraction 1000 - 1 = 999
    failed += test_dec64_2(dec64_sub, "-0.866025403784439", "-1.154700538379251", "0.288675134594812", 0); // subtraction
    failed += test_dec64_2(dec64_mul, "12", "3.5", "42", 0); // multiplication 12 * 3.5 = 42
    failed += test_dec64_2(dec64_mul, "18446744073709551615", "18446744073709551615", "3.402823669209384e38", 0);
    failed += test_dec64_2(dec64_mul, "1.8e100", "1.8e100", "3.24e200", 0);
    failed += test_dec64_2(dec64_div, "1", "1", "1", 0);
    failed += test_dec64_2(dec64_div, "1", "81", "0.01234567901234568", 0);
    failed += test_dec64_2(dec64_div, "10", "4", "2.5", 0); // division 10 / 4 = 2.5
    failed += test_dec64_2(dec64_div, "1", "0", "inf", 0); // division by zero
    failed += test_dec64_2(dec64_div, "-1", "0", "-inf", 0); // division by zero
    failed += test_dec64_2(dec64_div, "0", "0", "nan", 0); // division by zero
    failed += test_dec64_2i(dec64_powi, "2", 10, "1024", 0); // 2^10 = 1024
    failed += test_dec64_2i(dec64_powi, "2", 20, "1048576", 0); // 2^20 = 1048576
    failed += test_dec64_2i(dec64_powi, "5", -3, "0.008", 0); // 5^-3 = 0.008
    failed += test_dec64_2i(dec64_powi, "10", 0, "1", 0); // 10^0 = 1
    failed += test_dec64_2i(dec64_powi, "0", 3, "0", 0); // 0^3 = 0
    failed += test_dec64_2i(dec64_powi, "1.84", __pow10_dec64[3], "6.573898729783671e264", 0); // 2^10 = 1024

    intern_dec64_t result, base = {0, 18400000000000000000ULL, -18, DEC_NORMAL};
    intern_dec64_powi(&result, &base, __pow10_dec64[9]);

    printf("%lu %d\n", result.coeff, result.exponent);

    report_failed(failed);
    
    return failed != 0;
}
