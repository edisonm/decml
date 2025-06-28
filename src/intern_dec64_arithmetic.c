/* libdec/src/dec64_arithmetic.c - Basic arithmetic for decimal64 */
#include "intern/dec64.h"
#include "intern/impl_dec_arithmetic.h"

static inline uint64_t hi_ddec64(__uint128_t *x) {return (uint64_t)(*x >> 64);}
static inline uint64_t lo_ddec64(__uint128_t *x) {return (uint64_t)*x;}
static inline void mul_bits_dec64_ddec64(uint64_t a, uint64_t b, __uint128_t *x) {*x = (__uint128_t)a * b;}

static inline void div_bits_ddec64_dec64(const __uint128_t *numerator,
                                         const uint64_t denominator,
                                         __uint128_t *quotient,
                                         uint64_t *remainder) {
    if (quotient) *quotient = *numerator / denominator;
    if (remainder) *remainder = (uint64_t)(*numerator % denominator);
}

/* __IMPL_DEC_BITS_DIV_POW10_ROUND(dec64) */
__IMPL_DEC_BITS_DIV_POW10_ROUND_UP(dec64)
__IMPL_INTERN_DEC_ADD( dec64)
__IMPL_INTERN_DEC_SUB( dec64)
__IMPL_INTERN_DEC_MUL( dec64)
__IMPL_INTERN_DEC_DIV( dec64)
__IMPL_INTERN_DEC_POWI(dec64)
__IMPL_INTERN_DEC_ABS( dec64)
/* 
static void intern_dec64_exp10m1_rec(const uint64_t coeff, const int exp, intern_dec64_t *y);

static void intern_dec64_exp10m1_rec(const uint64_t coeff, const int exp, intern_dec64_t *y) {
    if (coeff == 0) {
        *y = intern_dec64_zero;
        return;
    }
    if (exp == 0) {
        intern_dec64_t a = {0, 1, coeff, DEC_NORMAL};
        intern_dec64_sub(y, &a, &intern_dec64_one);
        return;
    }
    intern_dec64_t p;
    if (exp < -19) {
        // for small numbers, approximate exp10m1 via:
        // exp10m1(x) = exp(x*log(10))-1 ~= x*log(10)
        intern_dec64_exp10m1_rec(coeff/__pow10_dec64[-exp-19], -19, y);
        uint64_t res = coeff % __pow10_dec64[-exp-19];
        if (res==0)
            return;
        intern_dec64_t x = {0, res, exp, DEC_NORMAL};
        intern_dec64_mul(&p, &x, &intern_dec64_log_10);
    } else {
        intern_dec64_exp10m1_rec(coeff/10, exp+1, y);
        int digit = coeff % 10;
        if (digit==0)
            return;
        p = pow10m1_dec64[-exp-1][digit-1];
    }
    
    intern_dec64_t a, b;
    intern_dec64_add(&a,  y, &intern_dec64_one); // 1+y
    intern_dec64_mul(&b, &a, &p); // p*(1+y)
    intern_dec64_add(&a,  y, &b); // y+p*(1+y)
    *y = a;
}

void intern_dec64_exp10m1(intern_dec64_t *result, const intern_dec64_t *x) {
    static intern_dec64_t intern_dec64_fmax = {0, 0xFFFFFFFFFFFFFFFFULL, 0, DEC_NORMAL};
    static intern_dec64_t intern_dec64_feps = {0, 0x1ULL, -20, DEC_NORMAL};
    static intern_dec64_t intern_dec64_fmin = {1, I_dec64_MAX_DIGITS+1, 0, DEC_NORMAL};
    if (intern_dec64_cmp(x, &intern_dec64_fmin) <=0) {
        *result = intern_dec64_one;
        result->sign = 1;
        return;
    }
    
    if (x->sign) {
        intern_dec64_t a, b;
        intern_dec64_exp10m1_rec(x->coeff, x->exponent, result); // expm1(x)
        intern_dec64_add(&a, result, &intern_dec64_one); // 1+expm1(x)
        intern_dec64_div(&b, result, &a); // expm1(x) / (1+expm1(x))
        *result = b;
        result->sign = x->sign; // expm1(-x) == -expm1(x) / (1+expm1(x))
        return;
    }
    if (intern_dec64_cmp(x, &intern_dec64_fmax) >= 0) {
        intern_dec64_set_inf(result, 0);
        return;
    }
    if (intern_dec64_cmp(x, &intern_dec64_feps) <= 0) {
        intern_dec64_mul(result, x, &intern_dec64_log_10);
        return;
    }
    intern_dec64_exp10m1_rec(x->coeff, x->exponent, result);
}
*/
