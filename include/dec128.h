/* dec128.h - Header for decimal128 arithmetic */
#ifndef DEC128_H
#define DEC128_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "intf_dec.h"
#include "intern/dec128.h"     // path as appropriate

/*
 * IEEE 754-2008 Decimal128 BID encoding
 *
 *  [127]      Sign bit
 *  [126:124]  Combination (G0-G2)
 *  [123:110]  Exponent continuation (or part of coeff, per encoding)
 *  [109:0]    Coefficient continuation
 *
 * See IEEE 754 specs for details. For practical code, see Intel DFP, decNumber, etc.
 */
#define Q2(A,B) ((((__uint128_t)A)<<64)|B) // double quad integer
#define dec128_BITS             128
#define dec128_EXP_BITS         14
#define dec128_COEFF_BITS       113
#define dec128_HIGH_COEFF_BITS  111
#define dec128_MAX_DIGITS       34
#define dec128_SUP_COEFF        ((__uint128_t)100000000000000000ULL * 100000000000000000ULL) // 10^34
#define dec128_MAX_COEFF        ((__uint128_t)dec128_SUP_COEFF-1ULL)                       // 10^34-1
#define dec128_EXP_BIAS         6176
#define dec128_MAX_EXP          6111
#define dec128_MIN_EXP         -6143

// Masks and bit positions (adjust these according to the correct field layout for BID128)
#define dec128_SIGN_MASK        ((__uint128_t)1 << 127)
#define dec128_EXP_CONT_MASK    ((__uint128_t)0x3FFF << 113)
#define dec128_COEFF_CONT_MASK  Q2(   0x1FFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL)
#define dec128_COMB_MASK        ((__uint128_t)0x7 << 124)

// Combination field patterns for special values (see spec for real fields)
#define dec128_COMB_NAN         ((__uint128_t)0x7C << 120) // example mask—update for actual spec!
#define dec128_COMB_INF         ((__uint128_t)0x78 << 120)

#define dec128_NAN_BITS         Q2(0x7fff800000000000ULL, 0x0000000000000000ULL) // Canonical qNaN
#define dec128_INF_BITS         Q2(0x7fff000000000000ULL, 0x0000000000000000ULL) // Canonical Inf

#define dec128_INF_MASK         Q2(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL)
#define dec128_NAN_MASK         Q2(0x7FFF800000000000ULL, 0x0000000000000000ULL)

// Container for decimal128, store all bits
typedef struct {
    __uint128_t bits;
} dec128_t;

extern const dec128_t dec128_zero, dec128_one, dec128_two, dec128_nan, dec128_inf;

#define encode_dec128(sign, coeff, exp)                                 \
    ((dec128_t){ .bits =                                                \
         /* If coeff < 2^113 (low encoding): */                         \
         ((__uint128_t)((coeff) < ((__uint128_t)1 << 113)) ?            \
          /* Low encoding */                                            \
          (((__uint128_t)(sign) & 1) << 127)                          | \
          (((__uint128_t)((exp) + dec128_EXP_BIAS) & 0x3FFF) << 113)  | \
          ((__uint128_t)(coeff) & (((__uint128_t)1 << 113) - 1))        \
          :                                                             \
          /* High encoding */                                           \
          (((__uint128_t)(sign) & 1) << 127)                          | \
          /* Set G0-G2 == 0b110 (bits 126-124) for high encoding (see spec) */ \
          ((0x7ULL) << 124)                                           | \
          (((__uint128_t)((exp) + dec128_EXP_BIAS) & 0x3FFF) << 111)  | \
          /* Coefficient: lowest 111 bits only, with implicit high bit as per BID */ \
          ((__uint128_t)(coeff) & (((__uint128_t)1 << 111) - 1))        \
             )                                                          \
    })

/* --------- Bitfield extraction macros (similarly to dec64) ---------- */

// Extract sign (returns 0 or 1)
static inline int dec128_get_sign(const dec128_t *d) { return (d->bits >> 127) & 1; }
static inline void dec128_set_sign(dec128_t *d, int sign) {
    d->bits = (d->bits & ~dec128_SIGN_MASK) | ((__uint128_t)(sign & 1) << 127);
}

// The combination field distinguishes low/high coefficients (see spec)
static inline bool dec128_is_high_encoding(__uint128_t bits) {
    // G0-G2 = bits 126-124
    return ((bits >> 124) & 0x7) >= 0x6;
}

// Exponent extraction. Adjust bit manipulations as per the exact BID128 encoding.
static inline int dec128_get_exponent(const dec128_t *d) {
    __uint128_t bits = d->bits;
    int exponent;
    if (dec128_is_high_encoding(bits)) {
        exponent = (int)((bits >> 111) & 0x3FFF);  // adjust if high encoding
    } else {
        exponent = (int)((bits >> 113) & 0x3FFF);  // adjust if low encoding
    }
    return exponent - dec128_EXP_BIAS;
}
static inline void dec128_set_exponent(dec128_t *d, int exp) {
    __uint128_t bits = d->bits;
    __uint128_t biased = (__uint128_t)(exp + dec128_EXP_BIAS);
    if (dec128_is_high_encoding(bits)) {
        bits &= ~((__uint128_t)0x3FFF << 111);
        bits |= (biased & 0x3FFFULL) << 111;
    } else {
        bits &= ~((__uint128_t)0x3FFF << 113);
        bits |= (biased & 0x3FFFULL) << 113;
    }
    d->bits = bits;
}

// Coefficient extraction
static inline __uint128_t dec128_get_coefficient(const dec128_t *d) {
    __uint128_t bits = d->bits;
    if (dec128_is_high_encoding(bits)) {
        // High enc: (implicit | 2 bits) + 111 bits trailing
        __uint128_t trailing = bits & (((__uint128_t)1 << 111) - 1);
        return ((__uint128_t)4 << 111) | trailing;
    } else {
        return bits & (((__uint128_t)1 << 113) - 1);
    }
}
static inline void dec128_set_coefficient(dec128_t *d, __uint128_t coeff) {
    __uint128_t bits = d->bits;
    if (coeff > dec128_MAX_COEFF) {
        coeff = 0;
    }
    if (coeff >= ((__uint128_t)1 << 113)) {
        // High encoding, store trailing 111 bits
        __uint128_t trailing = coeff & (((__uint128_t)1 << 111) - 1);
        bits &= ~(((__uint128_t)1 << 111) - 1);
        bits |= trailing;
        bits &= ~((__uint128_t)0x7 << 124);
        bits |= (__uint128_t)0x7 << 124; // mark high enc
    } else {
        // Low encoding, 113 bits
        bits &= ~(((__uint128_t)1 << 113) - 1);
        bits |= coeff;
        bits &= ~((__uint128_t)0x7 << 124);
    }
    d->bits = bits;
}

/* Classification */
static inline bool dec128_is_nan(const dec128_t *d)    { return (d->bits & dec128_NAN_MASK) == dec128_NAN_MASK; }
static inline bool dec128_is_inf(const dec128_t *d)    { return (d->bits & dec128_INF_MASK) == dec128_INF_BITS; }
static inline bool dec128_is_finite(const dec128_t *d) { return !dec128_is_nan(d) && !dec128_is_inf(d); }

static inline void dec128_set_nan(dec128_t *d) { d->bits = dec128_NAN_BITS; }
static inline void dec128_set_inf(dec128_t *d, int sign) {
    d->bits = dec128_INF_BITS;
    if (sign) d->bits |= dec128_SIGN_MASK; else d->bits &= ~dec128_SIGN_MASK;
}

/* Arithmetic operations (with internal conversion for precision) */
void dec128_add( dec128_t *result, const dec128_t *a, const dec128_t *b);
void dec128_sub( dec128_t *result, const dec128_t *a, const dec128_t *b);
void dec128_mul( dec128_t *result, const dec128_t *a, const dec128_t *b);
void dec128_div( dec128_t *result, const dec128_t *a, const dec128_t *b);
void dec128_powi(dec128_t *result, const dec128_t *a, int exp);

// Comparator API (returns -1,0,1 or 2 as in internal)
int dec128_cmp(const dec128_t *a, const dec128_t *b);

__INTF_DEC_GREATER(dec128)
__INTF_DEC_LESS(dec128)
__INTF_DEC_EQUAL(dec128)
__INTF_DEC_LESSEQUAL(dec128)
__INTF_DEC_GREATEREQUAL(dec128)
__INTF_DEC_IS_UNORDERED(dec128)

/* Scientific functions */
__INTF_DEC_FUNC1(log10, dec128);
__INTF_DEC_FUNC1(atan,  dec128);
__INTF_DEC_FUNC1(sqrt,  dec128);
__INTF_DEC_FUNC1(asin,  dec128);
__INTF_DEC_FUNC1(acos,  dec128);
__INTF_DEC_FUNC1(sin,  dec128);
__INTF_DEC_FUNC1(cos,  dec128);
__INTF_DEC_FUNC1(abs,  dec128);
__INTF_DEC_FUNC1(exp10m1, dec128);

/* String conversion */
int dec128_set_str(dec128_t *result, const char *str);
int dec128_get_str(const dec128_t *x, char *buf, size_t bufsize);

/* Conversion to/from internal representation */
void dec128_to_internal(const dec128_t *d, intern_dec128_t *out);
void internal_to_dec128(const intern_dec128_t *in, dec128_t *d);

void normalize_intern_dec128(intern_dec128_t *v);

#define __print_dec128(x) ({                                    \
            printf("%c", dec_sign[dec128_get_sign(&(x))]);      \
            print_uint128(dec128_get_coefficient(&(x)));        \
            printf("e%d", dec128_get_exponent(&(x)));           \
        })

#endif /* DEC128_H */
