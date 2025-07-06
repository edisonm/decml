/* internal_dec64.h - Header for internal decimal64 arithmetic */

#ifndef INTERN_DEC64_H
#define INTERN_DEC64_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "intern/dec.h"
#include "intf_dec.h"

// Extended precision for internal calculations - TBD: this should not be needed
// if we use for the coefficient as maximum 2^64-1
#define I_dec64_MAX_DIGITS 19
#define I_dec64_BITS       64
#define intern_dec64_BITS  I_dec64_BITS

typedef struct {
    int sign;
    uint64_t coeff;
    int exponent;
    dec_special_t special;
} intern_dec64_t;

void print_uint64(uint64_t n);

#define print_bits_dec64 print_uint64
#define bits_dec64_t     uint64_t
#define UINT_dec64_MAX   UINT64_MAX
#define bits_ddec64_t    __uint128_t

extern const char dec_sign[];

#define __print_bits_dec64(x) printf("%lu", x)

#define __print_intern_dec64(x) printf("%c%lue%d%s", dec_sign[x.sign], x.coeff, x.exponent, dec_special[x.special])

extern const intern_dec64_t intern_dec64_zero, intern_dec64_one, intern_dec64_two,
    intern_dec64_pi, intern_dec64_nan, intern_dec64_inf, intern_dec64_neginf,
    intern_dec64_log2_10, intern_dec64_log_10, intern_dec64_log10_e, intern_dec64_epsilon;

extern const intern_dec64_t pow10m1_dec64[20][9];

static inline void intern_dec64_set_nan(intern_dec64_t *x) { *x = intern_dec64_nan; }
static inline void intern_dec64_set_inf(intern_dec64_t *x, int sign) { *x = intern_dec64_inf; x->sign = sign; }
static inline bool intern_dec64_is_nan(const intern_dec64_t *x) { return x->special == DEC_NAN; }
static inline bool intern_dec64_is_inf(const intern_dec64_t *x) { return x->special == DEC_INF; }
static inline bool intern_dec64_is_finite(const intern_dec64_t *x) { return x->special == DEC_NORMAL; }

static inline uint64_t intern_dec64_get_coefficient(const intern_dec64_t *d) {return d->coeff;}
static inline int intern_dec64_get_exponent(const intern_dec64_t *d) {return d->exponent;}
static inline int intern_dec64_get_sign(const intern_dec64_t *d) {return d->sign;}

void normalize_coeff_exp_dec64(uint64_t *coeff, int *exponent);
void normalize_intern_dec64(intern_dec64_t *v);

/* Arithmetic operations */
void intern_dec64_add(intern_dec64_t *result, const intern_dec64_t *a, const intern_dec64_t *b);
void intern_dec64_sub(intern_dec64_t *result, const intern_dec64_t *a, const intern_dec64_t *b);
void intern_dec64_mul(intern_dec64_t *result, const intern_dec64_t *a, const intern_dec64_t *b);
void intern_dec64_div(intern_dec64_t *result, const intern_dec64_t *a, const intern_dec64_t *b);
void intern_dec64_powi(intern_dec64_t *result, const intern_dec64_t *base, const int64_t exp);

/* Aritmetic comparators */
// Returns: -1 if a < b, 0 if a == b, 1 if a > b, 2 if unordered (NaN involved/invalid)
int intern_dec64_cmp(const intern_dec64_t *a, const intern_dec64_t *b);

__INTF_DEC_GREATER(intern_dec64);
__INTF_DEC_LESS(intern_dec64);
__INTF_DEC_EQUAL(intern_dec64);
__INTF_DEC_LESSEQUAL(intern_dec64);
__INTF_DEC_GREATEREQUAL(intern_dec64);
__INTF_DEC_IS_UNORDERED(intern_dec64);

/* Scientific functions */
__DEC_FUNC_1_ALL(__INTF, intern_dec64);
__INTF_DEC_FUNC_1(abs, intern_dec64);

int intern_dec64_set_str(intern_dec64_t *out, const char *str);
int intern_dec64_get_str(const intern_dec64_t *d, char *buf, size_t bufsize);

#endif // INTERN_DEC64_H
