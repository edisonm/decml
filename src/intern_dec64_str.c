#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include "intern/dec64.h"
#include "intern/impl_dec_str.h"

__IMPL_INTERN_DEC_GET_STR(dec64, 32, 5)

// Helpers to parse decimal string to uint64_t coefficient (up to 19 digits)
int intern_dec64_set_str(intern_dec64_t *out, const char *str) {
    // Trim leading spaces
    while (isspace(*str)) str++;

    // Check for infinities and nan (case-insensitive, with/without '+'/'-')
    if (strcasecmp(str, "inf") == 0 || strcasecmp(str, "+inf") == 0) {
        intern_dec64_set_inf(out, 0);
        return 0;
    }
    if (strcasecmp(str, "-inf") == 0) {
        intern_dec64_set_inf(out, 1);
        return 0;
    }
    if (strcasecmp(str, "nan") == 0) {
        intern_dec64_set_nan(out);
        return 0;
    }

    // Parse string like "-123.456e7" or "1.23E-2"
    // Output:
    //   - coeff: integer coefficient (no decimal point)
    //   - exponent: adjusted exponent after shifting for decimal point

    int sign = 0;
    const char *p = str;

    // Skip whitespace
    while (isspace(*p)) p++;

    // Optional sign
    if (*p == '+') {
        p++;
    } else if (*p == '-') {
        sign = 1;
        p++;
    }

    uint64_t int_part = 0;
    uint64_t frac_part = 0;
    int int_exp = 0;
    int frac_exp = 0;
    bool decimal_seen = false;
    bool rounded = false;
    /* int expdiff = 0; */

    while (*p) {
        if (isdigit(*p)) {
            int digit = *p - '0';
            if (!decimal_seen) {
                // integer part
                if (int_part > (UINT64_MAX - digit) / 10) {
                    if (!rounded) {
                        if (digit >= 5 && int_part < UINT64_MAX) {
                            int_part++;
                        }
                        rounded = true;
                    }
                    int_exp++;
                } else {
                    int_part = int_part * 10 + digit;
                }
            } else {
                // fractional part
                { // max 19 digits for decimal64
                    if (frac_part > (UINT64_MAX - digit) / 10) {
                        // digits beyond 19 digits ignored for rounding simplicity
                        if (!rounded) {
                            if (digit >=5 && frac_part < UINT64_MAX) {
                                frac_part++;
                            }
                            rounded = true;
                        }
                    } else {
                        frac_part = frac_part * 10 + digit;
                        frac_exp++;
                    }
                }
            }
        } else if (*p == '.') {
            if (decimal_seen) return -1; // multiple decimals error
            decimal_seen = true;
        } else {
            break;
        }
        p++;
    }

    // Combine int and frac parts into coefficient
    // coefficient = int_part * 10^frac_len + frac_part
    uint64_t coeff64 = int_part;
    // Default exponent adjustment = -frac_len (decimal point shifts exponent)
    int exp = int_exp-frac_exp;

    for (int i = 0; i < frac_exp; i++) {
        if (coeff64 > UINT64_MAX / 10) {
            exp++;
            frac_part /= 10;
        } else {
            coeff64 *= 10;
        }
    }

    if (UINT64_MAX - coeff64 <= frac_part) {
        coeff64 = coeff64 / 10 + (frac_part + coeff64 % 10) / 10;
        exp++;
    } else
        coeff64 += frac_part;
    
    // Parse optional exponent part
    if (*p == 'e' || *p == 'E') {
        p++;
        int exp_sign = 1;
        if (*p == '+') {
            p++;
        } else if (*p == '-') {
            exp_sign = -1;
            p++;
        }
        int exp_val = 0;
        while (isdigit(*p)) {
            exp_val = exp_val * 10 + (*p - '0');
            p++;
        }
        exp += exp_sign * exp_val;
    }

    out->coeff = coeff64;
    out->exponent = exp;
    if (out->coeff==0) {
        out->sign = 0;
    } else
        out->sign = sign;
    out->special = DEC_NORMAL;

    return 0;
}
