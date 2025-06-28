#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include "intern/dec128.h"
#include "intern/impl_dec_str.h"

__IMPL_INTERN_DEC_GET_STR(dec128, 64, 6)

// Helpers to parse decimal string to __uint128_t coefficient (up to 38 digits)
int intern_dec128_set_str(intern_dec128_t *out, const char *str) {
    while (isspace(*str)) str++;
    if (strcasecmp(str, "inf") == 0 || strcasecmp(str, "+inf") == 0) {
        intern_dec128_set_inf(out, 0);
        return 0;
    }
    if (strcasecmp(str, "-inf") == 0) {
        intern_dec128_set_inf(out, 1);
        return 0;
    }
    if (strcasecmp(str, "nan") == 0) {
        intern_dec128_set_nan(out);
        return 0;
    }
    int sign = 0;
    const char *p = str;
    while (isspace(*p)) p++;
    if (*p == '+') {
        p++;
    } else if (*p == '-') {
        sign = 1;
        p++;
    }
    __uint128_t int_part = 0;
    __uint128_t frac_part = 0;
    int int_exp = 0;
    int frac_exp = 0;
    bool decimal_seen = false;
    bool rounded = false;
    while (*p) {
        if (isdigit(*p)) {
            int digit = *p - '0';
            if (!decimal_seen) {
                if (int_part > (UINT128_MAX - digit) / 10) {
                    if (!rounded) {
                        if (digit >= 5 && int_part < UINT128_MAX) {
                            int_part++;
                        }
                        rounded = true;
                    }
                    int_exp++;
                } else {
                    int_part = int_part * 10 + digit;
                }
            } else {
                // Limit to 38 digits for decimal128
                if (frac_part > (UINT128_MAX - digit) / 10) {
                    if (!rounded) {
                        if (digit >= 5 && frac_part < UINT128_MAX) {
                            frac_part++;
                        }
                        rounded = true;
                    }
                } else {
                    frac_part = frac_part * 10 + digit;
                    frac_exp++;
                }
            }
        } else if (*p == '.') {
            if (decimal_seen) return -1;
            decimal_seen = true;
        } else {
            break;
        }
        p++;
    }
    __uint128_t coeff128 = int_part;
    int exp = int_exp - frac_exp;
    for (int i = 0; i < frac_exp; i++) {
        if (coeff128 > UINT128_MAX / 10) {
            exp++;
            frac_part /= 10;
        } else {
            coeff128 *= 10;
        }
    }
    if (coeff128 > UINT128_MAX - frac_part) {
        coeff128 = coeff128 / 10 + (frac_part + coeff128 % 10) / 10;
        exp++;
    } else {
        coeff128 += frac_part;
    }
    // Parse optional exponent
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
    out->coeff = coeff128;
    out->exponent = exp;
    if (out->coeff == 0) {
        out->sign = 0;
    } else {
        out->sign = sign;
    }
    out->special = DEC_NORMAL;
    return 0;
}
