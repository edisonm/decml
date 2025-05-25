#include <limits.h>
#include <stdio.h>

#define __IMPL_DEC_COMPARE(__dec)                                       \
    int intern_##__dec##_cmp(const intern_##__dec##_t *ia,              \
                             const intern_##__dec##_t *ib) {            \
        /* 1. NaN propagation: unordered */                             \
        if (ia->special == DEC_NAN || ib->special == DEC_NAN)           \
            return 2; /* unordered */                                   \
                                                                        \
        /* 2. Infinity handling */                                      \
        if (ia->special == DEC_INF || ib->special == DEC_INF) {         \
            if (ia->special == DEC_INF && ib->special == DEC_INF) {     \
                /* compare signs (+inf == +inf, -inf == -inf, else sign decides) */ \
                if (ia->sign == ib->sign) return 0;                     \
                return ia->sign ? -1 : 1;                               \
            }                                                           \
            /* Any inf dominates finite */                              \
            if (ia->special == DEC_INF)                                 \
                return ia->sign ? -1 : 1; /* -inf < finite < +inf */    \
            else                                                        \
                return ib->sign ? 1 : -1; /* finite > -inf, finite < +inf */ \
        }                                                               \
                                                                        \
        /* 3. Zeroes: always equal, regardles of sign */                \
        if (ia->coeff == 0 && ib->coeff == 0)                           \
            return 0;                                                   \
                                                                        \
        /* 4. Sign */                                                   \
        if (ia->sign != ib->sign)                                       \
            return ia->sign ? -1 : 1; /* negative < positive */         \
                                                                        \
        /* 5. Both same sign, finite, nonzero: align exponents for comparison */ \
        int exp_a = ia->exponent, exp_b = ib->exponent;                 \
        bits_##__dec##_t coeff_a = ia->coeff, coeff_b = ib->coeff;    \
        int cmp;                                                        \
                                                                        \
        bits_##__dec##_t tmp;                                         \
                                                                        \
        tmp = coeff_a;                                                  \
        int digits_a = 0;                                               \
        while (tmp) { tmp /= 10 ; digits_a++; }                         \
                                                                        \
        tmp = coeff_b;                                                  \
        int digits_b = 0;                                               \
        while (tmp) { tmp /= 10 ; digits_b++; }                         \
                                                                        \
        if (digits_a > digits_b) {                                      \
            coeff_b *= __pow10_##__dec[digits_a-digits_b];            \
            exp_b -= (digits_a-digits_b);                               \
        } else if (digits_a < digits_b) {                               \
            coeff_a *= __pow10_##__dec[digits_b-digits_a];            \
            exp_a -= (digits_b-digits_a);                               \
        }                                                               \
                                                                        \
        if      (coeff_a == 0)      cmp = -1;                           \
        else if (coeff_b == 0)      cmp =  1;                           \
        else if (exp_a   < exp_b)   cmp = -1;                           \
        else if (exp_a   > exp_b)   cmp =  1;                           \
        else if (coeff_a < coeff_b) cmp = -1;                           \
        else if (coeff_a > coeff_b) cmp =  1;                           \
        else cmp = 0;                                                   \
                                                                        \
        /* If numbers are negative, flip comparison result */           \
        return ia->sign ? -cmp : cmp;                                   \
    }
