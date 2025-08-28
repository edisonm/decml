/* internal/dec128.h - Header for internal decimal128 arithmetic */
#ifndef INTERN_DEC128_H
#define INTERN_DEC128_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "intern/dec.h"
#include "intf_dec.h"

// Largest integer value with ≤38 decimal digits (max int128_t is >10^38)
#define I_dec128_MAX_DIGITS 38
#define I_dec128_BITS       128
#define intern_dec128_BITS  I_dec128_BITS

#define I_DEC128_SUP_COEFF  ((__uint128_t)10000000000000000000ULL * 10000000000000000000ULL) // 10^38
#define UINT128_MAX ((((__uint128_t)0xFFFFFFFFFFFFFFFFULL)<<64)|0xFFFFFFFFFFFFFFFFULL)

typedef struct {
    int sign;
    __uint128_t coeff;
    int exponent;
    dec_special_t special;
} intern_dec128_t;

typedef struct {
    __uint128_t hi;
    __uint128_t lo;
} uint256_t;

void print_uint128(__uint128_t n);

#define print_bits_dec128       print_uint128
#define bits_dec128_t           __uint128_t
#define bits_intern_dec128_t    __uint128_t
#define bits_ddec128_t          uint256_t
#define UINT_dec128_MAX         UINT128_MAX
#define UINT_intern_dec128_MAX  UINT128_MAX
#define intern_dec128_MAX_COEFF UINT128_MAX

extern const char dec_sign[];

#define __print_bits_dec128(x) print_uint128(x)

#define __print_intern_dec128(x) ({                                     \
            printf("%c", dec_sign[x.sign]);                             \
            print_uint128(x.coeff);                                     \
            printf("e%d%s", x.exponent, dec_special[x.special]);        \
        })

/// Constants (to be defined in a .c file)
extern const intern_dec128_t intern_dec128_zero, intern_dec128_one,
    intern_dec128_two, intern_dec128_three, intern_dec128_pi, intern_dec128_nan,
    intern_dec128_inf, intern_dec128_neginf, intern_dec128_log2_10,
    intern_dec128_log_10, intern_dec128_log10_e, intern_dec128_epsilon;

extern const intern_dec128_t pow10m1_dec128[40][9];

/// Inlines
static inline void intern_dec128_set_nan(intern_dec128_t *x) { *x = intern_dec128_nan; }
static inline void intern_dec128_set_inf(intern_dec128_t *x, int sign) {
    *x = intern_dec128_inf; x->sign = sign;
}
static inline bool intern_dec128_is_nan(const intern_dec128_t *x)  { return x->special == DEC_NAN; }
static inline bool intern_dec128_is_inf(const intern_dec128_t *x)  { return x->special == DEC_INF; }
static inline bool intern_dec128_is_finite(const intern_dec128_t *x) { return x->special == DEC_NORMAL; }

static inline __uint128_t intern_dec128_get_coefficient(const intern_dec128_t *d) {return d->coeff;}
static inline int intern_dec128_get_exponent(const intern_dec128_t *d) {return d->exponent;}
static inline int intern_dec128_get_sign(const intern_dec128_t *d) {return d->sign;}

void normalize_coeff_exp_dec128(__uint128_t *coeff, int *exponent);

/* Arithmetic operations */
void intern_dec128_powi(intern_dec128_t *result, const intern_dec128_t *base, int64_t exp);

/* Comparisons */
// Returns: -1 if a < b, 0 if a == b, 1 if a > b, 2 if unordered (NaN involved/invalid)
int intern_dec128_cmp(const intern_dec128_t *a, const intern_dec128_t *b);

__INTF_DEC_GREATER(intern_dec128)
__INTF_DEC_LESS(intern_dec128)
__INTF_DEC_EQUAL(intern_dec128)
__INTF_DEC_LESSEQUAL(intern_dec128)
__INTF_DEC_GREATEREQUAL(intern_dec128)
__INTF_DEC_IS_UNORDERED(intern_dec128)

/* Scientific functions */
__DEC_FUNC_1_ALL(__INTF, intern_dec128);
__DEC_FUNC_2_ALL(__INTF, intern_dec128);
__INTF_DEC_FUNC_1(abs, intern_dec128);

int intern_dec128_set_str(intern_dec128_t *out, const char *str);
int intern_dec128_get_str(const intern_dec128_t *d, char *buf, size_t bufsize);

#endif // INTERN_DEC128_H
