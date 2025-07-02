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
