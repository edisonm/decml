#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset
#include <ctype.h>

// Returns rounded value after dividing val by 10^shift, with round-half-up.
// If shift==0, returns val unchanged.
#define __IMPL_DEC_BITS_DIV_POW10_ROUND(__dec)                          \
    static bits_##__dec##_t __dec##_div_pow10_round(bits_##__dec##_t val, int shift) { \
        if (shift <= 0) return val;                                     \
        bits_##__dec##_t pow10 = __pow10_##__dec[shift];                \
        bits_##__dec##_t rem = val % pow10;                             \
        bits_##__dec##_t half = pow10 / 2;                              \
        val /= pow10;                                                   \
        /* Round up if remainder >= half.*/                             \
        if (rem > half || (rem == half && (val % 2))) /* ties to even (for banker's rounding) */ \
            val++;                                                      \
        return val;                                                     \
    }

#define __IMPL_DEC_BITS_DIV_POW10_ROUND_UP(__dec)                       \
    static void __dec##_div_pow10_round_up(bits_##__dec##_t *val,       \
                                           bits_##__dec##_t rem,        \
                                           int shift) {                 \
        if (shift <= 0) return;                                         \
        bits_##__dec##_t pow10 = __pow10_##__dec[shift];                \
        bits_##__dec##_t half = pow10 / 2;                              \
        /* Round up if remainder >= half.*/                             \
        if (rem > half || (rem == half && (*val % 2))) /* ties to even (for banker's rounding) */ \
            (*val)++;                                                   \
    }

#define __IMPL_INTERN_DEC_ADD(__dec)                                    \
    void intern_##__dec##_add(intern_##__dec##_t *result, const intern_##__dec##_t *a, \
                              const intern_##__dec##_t *b) {            \
                                                                        \
        /* Handle NaN/Inf cases (IEEE 754-2008 section 7.1) */          \
        if (intern_##__dec##_is_nan(a)) {                               \
            intern_##__dec##_set_nan(result);                           \
            return;                                                     \
        }                                                               \
        if (intern_##__dec##_is_nan(b)) {                               \
            intern_##__dec##_set_nan(result);                           \
            return;                                                     \
        }                                                               \
                                                                        \
        if (intern_##__dec##_is_inf(a)) {                               \
            if (intern_##__dec##_is_inf(b) && a->sign != a->sign) {     \
                intern_##__dec##_set_nan(result); /* Inf - Inf = NaN */ \
            } else {                                                    \
                intern_##__dec##_set_inf(result, a->sign);              \
            }                                                           \
            return;                                                     \
        }                                                               \
        if (intern_##__dec##_is_inf(b)) {                               \
            intern_##__dec##_set_inf(result, b->sign);                  \
            return;                                                     \
        }                                                               \
                                                                        \
        int exp_a = a->exponent;                                        \
        int exp_b = b->exponent;                                        \
        bits_##__dec##_t crest = 0;                                     \
        bits_##__dec##_t crest_a = 0, coeff_a = a->coeff;               \
        bits_##__dec##_t crest_b = 0, coeff_b = b->coeff;               \
                                                                        \
        if (coeff_a==0 || coeff_b == 0) {                               \
            if (coeff_a != 0) {                                         \
                *result = *a;                                           \
                return;                                                 \
            };                                                          \
            if (coeff_b != 0) {                                         \
                *result = *b;                                           \
                return;                                                 \
            }                                                           \
            *result = intern_##__dec##_zero;                            \
            return;                                                     \
        }                                                               \
                                                                        \
        normalize_coeff_exp_##__dec(&coeff_a, &exp_a);                  \
        normalize_coeff_exp_##__dec(&coeff_b, &exp_b);                  \
                                                                        \
        /* printf("a0=["); __print_bits_##__dec(coeff_a); printf(","); printf("%d,", exp_a); __print_bits_##__dec(crest_a); printf("]\n"); */ \
        /* printf("b0=["); __print_bits_##__dec(coeff_b); printf(","); printf("%d,", exp_b); __print_bits_##__dec(crest_b); printf("]\n"); */ \
                                                                        \
        int shift = 0;                                                  \
        /* Align exponents */                                           \
        if (exp_a > exp_b) {                                            \
            shift = exp_a - exp_b;                                      \
            if (shift < I_##__dec##_MAX_DIGITS) { /* prevent excessive loss of precision */ \
                crest_b = coeff_b % __pow10_##__dec[shift];             \
                coeff_b /= __pow10_##__dec[shift];                      \
            } else {                                                    \
                crest_b = coeff_b;                                      \
                coeff_b = 0;                                            \
            }                                                           \
            exp_b = exp_a;                                              \
        } else if (exp_b > exp_a) {                                     \
            shift = exp_b - exp_a;                                      \
            if (shift < I_##__dec##_MAX_DIGITS) {                       \
                crest_a = coeff_a % __pow10_##__dec[shift];             \
                coeff_a /= __pow10_##__dec[shift];                      \
            } else {                                                    \
                crest_a = coeff_a;                                      \
                coeff_a = 0;                                            \
            }                                                           \
            exp_a = exp_b;                                              \
        }                                                               \
        /* printf("a1=["); __print_bits_##__dec(coeff_a); printf(","); printf("%d,", exp_a); __print_bits_##__dec(crest_a); printf("]\n"); */ \
        /* printf("b1=["); __print_bits_##__dec(coeff_b); printf(","); printf("%d,", exp_b); __print_bits_##__dec(crest_b); printf("]\n"); */ \
                                                                        \
        if (a->sign == b->sign) {                                       \
            if (coeff_a >= UINT_##__dec##_MAX - coeff_b) {              \
                result->coeff = coeff_a/10+coeff_b/10+(coeff_a%10+coeff_b%10)/10; \
                if (0 < shift && shift <= I_##__dec##_MAX_DIGITS) {     \
                    crest = ((coeff_a%10+coeff_b%10)%10)* __pow10_##__dec[shift-1] \
                        + (crest_a + crest_b+5)/10;                     \
                    if (crest >= __pow10_##__dec[shift]) {              \
                        crest -= __pow10_##__dec[shift];                \
                        result->coeff++;                                \
                    }                                                   \
                };                                                      \
                exp_a++;                                                \
            } else {                                                    \
                /* printf("a2=["); __print_bits_##__dec(coeff_a); printf(","); printf("%d]\n", exp_a); */ \
                /* printf("b2=["); __print_bits_##__dec(coeff_b); printf(","); printf("%d]\n", exp_b); */ \
                result->coeff = coeff_a + coeff_b;                      \
                crest = crest_a + crest_b;                              \
                /* printf("shift=%d, crest=%d\n", shift, crest); */     \
                /* printf("+result->coeff="); __print_bits_##__dec(result->coeff); printf("\n"); */ \
                __dec##_div_pow10_round_up(&(result->coeff), crest, shift); \
                /* printf("-result->coeff="); __print_bits_##__dec(result->coeff); printf("\n"); */ \
            }                                                           \
            result->sign = a->sign;                                     \
        } else {                                                        \
            if (coeff_a > coeff_b) {                                    \
                result->coeff = coeff_a - coeff_b;                      \
                if (shift <= I_##__dec##_MAX_DIGITS && crest_b) {       \
                    result->coeff--;                                    \
                    crest = crest_a + (__pow10_##__dec[shift] - crest_b); \
                    if (crest >= __pow10_##__dec[shift]) {              \
                        crest -= __pow10_##__dec[shift];                \
                        result->coeff++;                                \
                    }                                                   \
                }                                                       \
                __dec##_div_pow10_round_up(&(result->coeff), crest, shift); \
                result->sign = a->sign;                                 \
            } else {                                                    \
                result->coeff = coeff_b - coeff_a;                      \
                if (shift <= I_##__dec##_MAX_DIGITS && crest_a) {       \
                    result->coeff--;                                    \
                    crest = crest_b + (__pow10_##__dec[shift] - crest_a); \
                    if (crest >= __pow10_##__dec[shift]) {              \
                        crest -= __pow10_##__dec[shift];                \
                        result->coeff++;                                \
                    }                                                   \
                }                                                       \
                __dec##_div_pow10_round_up(&(result->coeff), crest, shift); \
                result->sign = b->sign;                                 \
            }                                                           \
        }                                                               \
                                                                        \
        if (result->coeff!=0) {                                         \
            result->exponent = exp_a;                                   \
        } else {                                                        \
            result->exponent = 0;                                       \
        }                                                               \
                                                                        \
        result->sign = result->coeff == 0 ? 0 : result->sign;           \
        result->special = DEC_NORMAL;                                   \
    }

#define __IMPL_INTERN_DEC_SUB(__dec)                            \
    void intern_##__dec##_sub(intern_##__dec##_t *result,       \
                              const intern_##__dec##_t *a,      \
                              const intern_##__dec##_t *b) {    \
        intern_##__dec##_t neg_b = *b;                          \
        neg_b.sign = !b->sign;                                  \
        intern_##__dec##_add(result, a, &neg_b);                \
    }

#define __IMPL_INTERN_DEC_MUL(__dec)                                    \
    void intern_##__dec##_mul(intern_##__dec##_t *result,               \
                              const intern_##__dec##_t *a,              \
                              const intern_##__dec##_t *b) {            \
        if (intern_##__dec##_is_nan(a) || intern_##__dec##_is_nan(b)) { \
            intern_##__dec##_set_nan(result);                           \
            return;                                                     \
        }                                                               \
        if (intern_##__dec##_is_inf(a) || intern_##__dec##_is_inf(b)) { \
            /* If either is zero, result is NaN */                      \
            if ((intern_##__dec##_is_inf(a) && b->coeff == 0)           \
                || (intern_##__dec##_is_inf(b) && a->coeff == 0)) {     \
                intern_##__dec##_set_nan(result);                       \
            } else {                                                    \
                intern_##__dec##_set_inf(result, a->sign ^ b->sign);    \
            }                                                           \
            return;                                                     \
        }                                                               \
                                                                        \
        bits_d##__dec##_t product;                                      \
        mul_bits_##__dec##_d##__dec(a->coeff, b->coeff, &product);      \
        result->sign = a->sign ^ b->sign;                               \
                                                                        \
        int expdiff = 0;                                                \
        int exponent = a->exponent + b->exponent;                       \
                                                                        \
        while (hi_d##__dec(&product) >= __pow10_##__dec[expdiff]) {     \
            expdiff++;                                                  \
            if (expdiff == I_##__dec##_MAX_DIGITS) {                    \
                div_bits_d##__dec##_##__dec(&product, __pow10_##__dec[expdiff], &product, NULL); \
                exponent += expdiff;                                    \
                expdiff = 0;                                            \
            }                                                           \
        }                                                               \
                                                                        \
        if (expdiff!=0) {                                               \
            div_bits_d##__dec##_##__dec(&product, __pow10_##__dec[expdiff], &product, NULL); \
            exponent += expdiff;                                        \
        }                                                               \
        result->exponent = exponent;                                    \
        result->coeff = lo_d##__dec(&product);  /* TODO: handle overflow and rounding */ \
        result->special = DEC_NORMAL;                                   \
    }

#define __IMPL_INTERN_DEC_DIV(__dec)                                    \
    void intern_##__dec##_div(intern_##__dec##_t *result,               \
                              const intern_##__dec##_t *a,              \
                              const intern_##__dec##_t *b) {            \
        if (intern_##__dec##_is_nan(a) || intern_##__dec##_is_nan(b)) { \
            intern_##__dec##_set_nan(result);                           \
            return;                                                     \
        }                                                               \
        if (intern_##__dec##_is_inf(a) && intern_##__dec##_is_inf(b)) { \
            intern_##__dec##_set_nan(result);                           \
            return;                                                     \
        }                                                               \
                                                                        \
        if (intern_##__dec##_is_inf(a)) {                               \
            int sign = a->sign ^ b->sign;                               \
            intern_##__dec##_set_inf(result, sign);                     \
            return;                                                     \
        }                                                               \
        if (intern_##__dec##_is_inf(b)) {                               \
            int sign = a->sign ^ b->sign;                               \
            *result = intern_##__dec##_zero;                            \
            result->sign = sign;                                        \
            return;                                                     \
        }                                                               \
                                                                        \
        if (b->coeff == 0) {                                            \
            if (a->coeff == 0) {                                        \
                /* 0/0 = NaN */                                         \
                intern_##__dec##_set_nan(result);                       \
            } else {                                                    \
                /* Nonzero/0 = Inf */                                   \
                int sign = a->sign ^ b->sign;                           \
                intern_##__dec##_set_inf(result, sign);                 \
            }                                                           \
            return;                                                     \
        }                                                               \
        if (a->coeff == 0) {                                            \
            *result = intern_##__dec##_zero;                            \
            return;                                                     \
        }                                                               \
                                                                        \
        int exp_a = a->exponent;                                        \
        bits_##__dec##_t coeff_a = a->coeff;                            \
                                                                        \
        int expdiff = 0;                                                \
                                                                        \
        while (coeff_a * __pow10_##__dec[expdiff] <= UINT_##__dec##_MAX / 10) { \
            expdiff++;                                                  \
        }                                                               \
        coeff_a *= __pow10_##__dec[expdiff];                            \
        exp_a -= expdiff;                                               \
        expdiff = 0;                                                    \
                                                                        \
        bits_d##__dec##_t numerator;                                    \
                                                                        \
        mul_bits_##__dec##_d##__dec(coeff_a, __pow10_##__dec[I_##__dec##_MAX_DIGITS], &numerator); /* adjust for precision */ \
                                                                        \
        result->exponent = exp_a - b->exponent - I_##__dec##_MAX_DIGITS; \
                                                                        \
        div_bits_d##__dec##_##__dec(&numerator, b->coeff, &numerator, NULL); \
                                                                        \
        while (hi_d##__dec(&numerator) >= __pow10_##__dec[expdiff]) {   \
            expdiff++;                                                  \
            if (expdiff == I_##__dec##_MAX_DIGITS) {                    \
                div_bits_d##__dec##_##__dec(&numerator, __pow10_##__dec[expdiff], &numerator, NULL); \
                result->exponent += expdiff;                            \
                expdiff = 0;                                            \
            }                                                           \
        }                                                               \
                                                                        \
        if (expdiff!=0) {                                               \
            div_bits_d##__dec##_##__dec(&numerator, __pow10_##__dec[expdiff], &numerator, NULL); \
            result->exponent += expdiff;                                \
        }                                                               \
                                                                        \
        result->coeff = lo_d##__dec(&numerator);                        \
        result->sign = a->sign ^ b->sign;                               \
        result->special = DEC_NORMAL;                                   \
    }

/* Integer power function for dec64_t, exp is uint64_t to calculate log10 */
#define __IMPL_INTERN_DEC_POWI(__dec)                                   \
    void intern_##__dec##_powi(intern_##__dec##_t *result,              \
                               const intern_##__dec##_t *base,          \
                               const int64_t exp) {                     \
        if (exp == 0) {                                                 \
            *result = intern_##__dec##_one;                             \
            return;                                                     \
        }                                                               \
                                                                        \
        if (base->coeff == 0) {                                         \
            *result = intern_##__dec##_zero;                            \
            return;                                                     \
        }                                                               \
                                                                        \
        intern_##__dec##_t b = *base;                                   \
        uint64_t e = (exp < 0) ? -exp : exp;                            \
                                                                        \
        *result = intern_##__dec##_one;                                 \
                                                                        \
        while (e > 0) {                                                 \
            if (e & 1) {                                                \
                intern_##__dec##_mul(result, result, &b);               \
            }                                                           \
            intern_##__dec##_mul(&b, &b, &b);                           \
            e >>= 1;                                                    \
        }                                                               \
                                                                        \
        if (exp < 0) {                                                  \
            intern_##__dec##_div(result, &intern_##__dec##_one, result); \
        }                                                               \
    }

#define __IMPL_INTERN_DEC_ABS(__dec)                                    \
    void intern_##__dec##_abs(intern_##__dec##_t *result,               \
                              const intern_##__dec##_t *value) {        \
        /* No effect for NaN/Inf, but safe to clear sign always */      \
        result->sign = 0;                                               \
        result->coeff = value->coeff;                                   \
        result->exponent = value->exponent;                             \
        result->special = value->special;                               \
    }
