/* dec64.h - Header for decimal64 arithmetic */

#ifndef DEC64_H
#define DEC64_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "intf_dec.h"
#include "intern/dec64.h"

/*
 * IEEE 754-2008 Decimal64 Binary Integer Decimal (BID) layout
 *
 *  64 bits total:
 *  [63]       Sign bit
 *  [62:61]    G0-G1 bits — first 2 bits of the combination field
 *  [60:59]    G2-G3 bits — next 2 bits of the combination field
 *  [58:49]    Continuation of exponent or coefficient (depending on combo bits)
 *  [48:0]     Coefficient continuation
 *
 * The encoding distinguishes between two forms:
 * - Low significand: G0-G3 in range 0–7
 * - High significand: G0-G3 == 0b1100 or 0b1101
 *
 * We store the full 64-bit word and parse it.
 */

#define dec64_BITS            64
#define dec64_EXP_BITS        10
#define dec64_COEFF_BITS      53
#define dec64_HIGH_COEFF_BITS 51
#define dec64_MAX_DIGITS      16
#define dec64_SUP_COEFF       10000000000000000ULL  // 10^16
#define dec64_MAX_COEFF       (dec64_SUP_COEFF-1)   // 10^16 - 1

#define dec64_EXP_BIAS       398
#define dec64_MAX_EXP        369
#define dec64_MIN_EXP       -398

#define dec64_COMB_MASK      0x1E00000000000000ULL  // Bits 62-58
#define dec64_SIGN_MASK      0x8000000000000000ULL
#define dec64_EXP_CONT_MASK  0x03FC000000000000ULL
#define dec64_COEFF_CONT_MASK 0x001FFFFFFFFFFFFFULL

// Combination field patterns for special values (bits 62-58)
#define dec64_COMB_NAN            ((uint64_t)0x7800000000000000ULL) // 11110xxxxx...
#define dec64_COMB_INF            ((uint64_t)0x7C00000000000000ULL) // 11111xxxxx...

// Special encodings (when exponent == 0x3FF)

// Canonical quiet NaN encoding (IEEE defines 1st bit of coefficient = 1 for quiet NaNs)
#define dec64_NAN_BITS        0x7FF8000000000000ULL  // sign = 0, exp = all 1s, quiet NaN payload
#define dec64_SNAN_BITS       0x7FF0000000000001ULL  // signaling NaN (optional)

// Canonical positive infinity
#define dec64_INF_BITS        0x7FF0000000000000ULL  // sign = 0, exp = all 1s, coeff = 0

// Masks for detection
#define dec64_INF_MASK        0x7FFFFFFFFFFFFFFFULL  // detects inf regardless of sign
#define dec64_NAN_MASK        0x7FF8000000000000ULL  // mask to match top bits of quiet NaN

typedef struct {
    uint64_t bits;
} dec64_t;

extern const dec64_t dec64_zero, dec64_one, dec64_two, dec64_nan, dec64_inf;

#define encode_dec64(sign, coeff, exp) (                                \
        ((dec64_t){ .bits =                                             \
             ((uint64_t)((sign) & 1) << 63) |                           \
             (                                                          \
                 ((uint64_t)(coeff) < (1ULL << 53))                     \
                 ? /* low encoding */                                   \
                 (((uint64_t)((exp) + dec64_EXP_BIAS) & 0x3FFULL) << 53) | \
                 ((uint64_t)(coeff) & ((1ULL << 53) - 1))               \
                 : /* high encoding */                                  \
                 (0x3ULL << 61) |                                       \
                 (((uint64_t)((exp) + dec64_EXP_BIAS) & 0x3FFULL) << 51) | \
                 ((uint64_t)(coeff) & ((1ULL << 51) - 1))               \
                 )                                                      \
        }))

/* Extract the sign bit (0 = positive, 1 = negative) */
static inline int dec64_get_sign(const dec64_t *d) {
    return (d->bits >> 63) & 1;
}

/* Set the sign bit */
static inline void dec64_set_sign(dec64_t *d, int sign) {
    d->bits = (d->bits & ~dec64_SIGN_MASK) | ((uint64_t)(sign & 1) << 63);
}

static inline bool dec64_is_high_encoding(uint64_t bits) {
    return ((bits >> 61) & 0x3) == 0x3;
}

/* Extract exponent from BID-encoded decimal64 */
static inline int dec64_get_exponent(const dec64_t *d) {
    uint64_t bits = d->bits;
    if (dec64_is_high_encoding(bits)) {
        // High encoding, exponent is bits 61-52 (after 3 leading bits)
        return (int)(((bits >> 51) & 0x3FF) - dec64_EXP_BIAS);
    } else {
        // Low encoding, exponent is bits 61-52
        return (int)(((bits >> 53) & 0x3FF) - dec64_EXP_BIAS);
    }
}

static inline void dec64_set_exponent(dec64_t *d, int exp) {
    uint64_t bits = d->bits;
    uint64_t biased = (uint64_t)(exp + dec64_EXP_BIAS);
    if (dec64_is_high_encoding(bits)) {
        bits &= ~(0x3FFULL << 51);
        bits |= (biased & 0x3FFULL) << 51;
    } else {
        bits &= ~(0x3FFULL << 53);
        bits |= (biased & 0x3FFULL) << 53;
    }
    d->bits = bits;
}

/* Extract coefficient/significand */
static inline uint64_t dec64_get_coefficient(const dec64_t *d) {
    uint64_t bits = d->bits;
    if (dec64_is_high_encoding(bits)) {
        // Implicit 3-bit prefix "100" (binary = 4)
        uint64_t trailing = bits & ((1ULL << 51) - 1);
        return (4ULL << 51) | trailing;
    } else {
        return bits & ((1ULL << 53) - 1);
    }
}

static inline void dec64_set_coefficient(dec64_t *d, uint64_t coeff) {
    uint64_t bits = d->bits;
    if (coeff > dec64_MAX_COEFF) {
        coeff = 0; // Per spec, treat overflow as 0
    }
    if (coeff >= (1ULL << 53)) {
        // High encoding
        uint64_t trailing = coeff & ((1ULL << 51) - 1);
        bits &= ~((1ULL << 51) - 1);         // Clear coefficient
        bits |= trailing;
        bits &= ~(0x3ULL << 61);             // Clear bits 62-61
        bits |= 0x3ULL << 61;                // Set bits to 0b11 for high encoding
    } else {
        // Low encoding
        bits &= ~((1ULL << 53) - 1);         // Clear coefficient
        bits |= coeff;
        bits &= ~(0x3ULL << 61);             // Ensure bits 62-61 ≠ 0b11
    }
    d->bits = bits;
}

// True if value is NaN (including sNaN)
static inline bool dec64_is_nan(const dec64_t *d) {
    // NaNs: top 5 bits = 11110 or 11111, or IEEE: exponent all 1s and coeff != 0
    return (d->bits & dec64_NAN_MASK) == dec64_NAN_MASK;
}
// Only quiet NaN?
static inline bool dec64_is_qnan(const dec64_t *d) {
    return dec64_is_nan(d); // refine if supporting sNaN later
}
// True if value is Inf
static inline bool dec64_is_inf(const dec64_t *d) {
    // Exponent all 1s, coefficient 0
    return (d->bits & dec64_INF_MASK) == dec64_INF_BITS;
}
// True if finite
static inline bool dec64_is_finite(const dec64_t *d) {
    return !dec64_is_nan(d) && !dec64_is_inf(d);
}

static inline void dec64_set_nan(dec64_t *d) {
    d->bits = dec64_NAN_BITS;
}
static inline void dec64_set_inf(dec64_t *d, int sign) {
    d->bits = dec64_INF_BITS;
    if (sign)
        d->bits |= dec64_SIGN_MASK;
    else
        d->bits &= ~dec64_SIGN_MASK;
}

/* Arithmetic operations */
void dec64_add( dec64_t *result, const dec64_t *a, const dec64_t *b);
void dec64_sub( dec64_t *result, const dec64_t *a, const dec64_t *b);
void dec64_mul( dec64_t *result, const dec64_t *a, const dec64_t *b);
void dec64_div( dec64_t *result, const dec64_t *a, const dec64_t *b);
void dec64_powi(dec64_t *result, const dec64_t *a, int exp);

/* Arithmetic comparators */
int dec64_cmp(const dec64_t *a, const dec64_t *b);

static inline int dec64_greater(const dec64_t *a, const dec64_t *b) {
    int cmp = dec64_cmp(a, b);
    return cmp != 2 && cmp > 0;
}
static inline int dec64_less(const dec64_t *a, const dec64_t *b) {
    int cmp = dec64_cmp(a, b);
    return cmp != 2 && cmp < 0;
}

static inline int dec64_equal(const dec64_t *a, const dec64_t *b) {
    int cmp = dec64_cmp(a, b);
    return cmp != 2 && cmp == 0;
}

static inline int dec64_lessequal(const dec64_t *a, const dec64_t *b) {
    int cmp = dec64_cmp(a, b);
    return cmp != 2 && cmp <= 0;
}

static inline int dec64_greaterequal(const dec64_t *a, const dec64_t *b) {
    int cmp = dec64_cmp(a, b);
    return cmp != 2 && cmp >= 0;
}
static inline int dec64_is_unordered(const dec64_t *a, const dec64_t *b) {
    return dec64_cmp(a,b) == 2;
}

/* Scientific functions */
__INTF_DEC_FUNC1(log10, dec64);
__INTF_DEC_FUNC1(atan,  dec64);
__INTF_DEC_FUNC1(sqrt,  dec64);
__INTF_DEC_FUNC1(asin,  dec64);
__INTF_DEC_FUNC1(acos,  dec64);
__INTF_DEC_FUNC1(sin,  dec64);
__INTF_DEC_FUNC1(cos,  dec64);
__INTF_DEC_FUNC1(abs,  dec64);

/* String conversion */
int dec64_set_str(dec64_t *result, const char *str);
int dec64_get_str(const dec64_t *x, char *buf, size_t bufsize);

void dec64_to_internal(const dec64_t *d, intern_dec64_t *out);
void internal_to_dec64(const intern_dec64_t *in, dec64_t *d);
void normalize_intern_dec64(intern_dec64_t *v);

#endif /* DEC64_H */
