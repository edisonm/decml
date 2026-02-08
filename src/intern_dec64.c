#include <stdio.h>
#include <inttypes.h> // For PRIu64 and such
#include "intern/dec64.h"
#include "intern/impl_dec.h"
#include "intern/impl_dec_compare.h"
#include "intern/impl_dec_atan.h"
#include "intern/impl_dec_sqrt.h"
#include "intern/impl_dec_cbrt.h"
#include "intern/impl_dec_sincos.h"
#include "intern/impl_dec_asin.h"
#include "intern/impl_dec_acos.h"
#include "intern/impl_dec_log10.h"
#include "intern/impl_dec_exp10m1.h"
/* #define USE_SPOUGE */
#ifdef USE_SPOUGE
# include "intern/impl_dec_lgamma_spouge.h"
#else
# include "intern/impl_dec_lgamma.h"
#endif
#include "intern/impl_dec_arithmetic.h"

__IMPL_CONST_INTERN_DEC(dec64, pi,       0, 3141592653589793238ULL, -I_dec64_MAX_DIGITS+1, DEC_NORMAL);
__IMPL_CONST_INTERN_DEC(dec64, log2_10,  0, 3321928094887362348ULL, -I_dec64_MAX_DIGITS+1, DEC_NORMAL);
__IMPL_CONST_INTERN_DEC(dec64, log_10,   0, 2302585092994045684ULL, -I_dec64_MAX_DIGITS+1, DEC_NORMAL);
__IMPL_CONST_INTERN_DEC(dec64, log10_e,  0, 4342944819032518277ULL, -I_dec64_MAX_DIGITS,   DEC_NORMAL);
__IMPL_CONST_INTERN_DEC(dec64, epsilon,  0,                   1ULL, -I_dec64_MAX_DIGITS,   DEC_NORMAL);

/* ---------- Lanczos tables (decimal strings parsed lazily) ---------- */
/* Form used:
xsum = a0 + Σ_{k=1..n-1} a_k/(z+k), t = z + g + 0.5.
ln Γ(z) ≈ 0.5 ln(2π) + ln(xsum) + (z+0.5) ln t − t.

According to
https://web.viu.ca/pughg/phdThesis/phdThesis.pdf

n ~= -2 -0.3*log(epsilon), which gives for 20 digits, n~=12 terms

*/


#ifndef USE_SPOUGE
extern const int LANCZOS_dec64_N;
extern const intern_dec64_t lanczos_dec64_g;
extern const intern_dec64_t lanczos_dec64_coef[];
#endif

void print_uint64(uint64_t n) {
    printf("%" PRIu64, n);
}

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

__IMPL_CONST_INTERN_DECS(dec64)
__IMPL_NORMALIZE_COEFF_EXP_DEC(dec64)
__IMPL_NORMALIZE_INTERN_DEC(dec64)
__IMPL_DEC_COMPARE(dec64)

__IMPL_INTERN_DEC_MOD_2PI(dec64)
__INTF_INTERN_DEC_EXP10M1_REC(dec64);
__IMPL_INTERN_DEC_EXP10M1_REC(dec64)
__IMPL_DEC_BITS_DIV_POW10_ROUND_UP(dec64)
#ifdef USE_SPOUGE
__IMPL_LOG_GAMMA_SPOUGE(dec64, 12)
#else
__IMPL_LANCZOS_CORE(dec64)
#endif

__DEC_FUNC_1_ALL(__IMPL_INTERN, dec64)
__DEC_FUNC_2_ALL(__IMPL_INTERN, dec64)

/* __IMPL_DEC_BITS_DIV_POW10_ROUND(dec64) */
__IMPL_INTERN_DEC_POWI(dec64)
__IMPL_INTERN_DEC_abs( dec64)
