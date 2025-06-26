/* tests/test_dec64_str.c - Unit tests for dec64_set_str and dec64_get_str */

#include <stdio.h>
#include <string.h>
#include "dec64.h"

int main(void) {
    dec64_t x;
    char buf[64];

    // List of test cases: input string and expected output string
    struct {
        const char *input;
        const char *expected;
    } tests[] = {
        {"123456789012345678901234567890", "1.234567890123457e29"},
        {"123456789012345678901234567890.123456789012345678901234567890", "1.234567890123457e29"},
        {"0.0000000000000000000000000000123456789012345678901234567890", "1.234567890123457e-29"},
        {"1e100", "1e100"},
        {"1e-100", "1e-100"},
        { "123.45", "123.45" },
        { "-0.123", "-0.123" },
        { "-0.0123", "-0.0123" },
        { "-0.00123", "-0.00123" },
        { "-0.000123", "-0.000123" },
        { "-0.0000123", "-0.0000123" },
        { "-0.00000123", "-1.23e-6" },
        // No exp format up to 10-5:
        { "-0.00001234567890123456", "-0.00001234567890123456" },
        { "-0.000001234567890123456", "-1.234567890123456e-6" },
        { "0.00001234567890123456", "0.00001234567890123456" },
        { "0.000001234567890123456", "1.234567890123456e-6" },
        { "+42", "42" },
        { "0.0000000000000001", "1e-16" },
        { "9999999999999999", "9999999999999999" },
        { "-9999999999999999", "-9999999999999999" },
        { "1", "1" },
        { "1.227", "1.227" },
        { "0", "0" },
        { "-0", "0" },
        { "99999999.999999994", "99999999.99999999" },
        { "99999999.999999995", "100000000" },
        { "1.2345678901234567", "1.234567890123457" }, // Precision cutoff at 16 digits
    };

    int passed = 0;
    int total = sizeof(tests) / sizeof(tests[0]);

    printf("dec64: Testing string conversions\n");

    for (int i = 0; i < total; ++i) {
        dec64_set_str(&x, tests[i].input);
        dec64_get_str(&x, buf, sizeof(buf));

        if (strcmp(buf, tests[i].expected) == 0) {
            printf("Test %d passed: '%s'\n", i + 1, tests[i].input);
            passed++;
        } else {
            printf("Test %d failed: input='%s' expected='%s' got='%s' (0x%lXULL)\n",
                   i + 1, tests[i].input, tests[i].expected, buf, x.bits);
        }
    }

    printf("\n%d/%d tests passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}
