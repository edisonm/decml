#define __IMPL_DEC_FUNC1(__func, __dec)                                 \
    void __dec##_##__func(__dec##_t *result, const __dec##_t *a) {      \
        intern_##__dec##_t b, r;                                        \
        __dec##_to_internal(a, &b);                                     \
        intern_##__dec##_##__func(&r, &b);                              \
        internal_to_##__dec(&r, result);                                \
    }

#define __IMPL_DEC_FUNC2(__func, __dec)                                 \
    void __dec##_##__func(__dec##_t *result, const __dec##_t *a, const __dec##_t *b) { \
        intern_##__dec##_t x, y, r;                                     \
        __dec##_to_internal(a, &x);                                     \
        __dec##_to_internal(b, &y);                                     \
        intern_##__dec##_##__func(&r, &x, &y);                          \
        internal_to_##__dec(&r, result);                                \
    }

#define __IMPL_DEC_FUNC2i(__func, __dec)                                \
    void __dec##_##__func(__dec##_t *result, const __dec##_t *a, int b) {    \
        intern_##__dec##_t x, r;                                        \
        __dec##_to_internal(a, &x);                                     \
        intern_##__dec##_##__func(&r, &x, b);                           \
        internal_to_##__dec(&r, result);                                \
    }

#define __IMPL_DEC_FUNCi2(__func, __dec)                         \
    int __dec##_##__func(const __dec##_t *a, const __dec##_t *b) \
    {                                                            \
        intern_##__dec##_t ia, ib;                               \
        __dec##_to_internal(a, &ia);                             \
        __dec##_to_internal(b, &ib);                             \
        return intern_##__dec##_cmp(&ia, &ib);                   \
    }

/* Convert decimal BID to internal. See decXXX.h for bit extraction. */
#define __IMPL_DEC_TO_INTERNAL(__dec)                                   \
    void __dec##_to_internal(const __dec##_t *dec,                      \
                             intern_##__dec##_t *out) {                 \
        if (__dec##_is_nan(dec)) {                                      \
            out->special = DEC_NAN;                                     \
            out->sign = 0; out->coeff = 0;                              \
            out->exponent = 0;                                          \
            return;                                                     \
        } else if (__dec##_is_inf(dec)) {                               \
            out->special = DEC_INF;                                     \
            out->sign = __dec##_get_sign(dec);                          \
            out->coeff = 0;                                             \
            out->exponent = 0;                                          \
            return;                                                     \
        }                                                               \
        out->special = DEC_NORMAL;                                      \
        out->sign = __dec##_get_sign(dec);                              \
        out->coeff = __dec##_get_coefficient(dec);                      \
        if (out->coeff == 0) {                                          \
            out->exponent = 0;                                          \
        } else {                                                        \
            out->exponent = __dec##_get_exponent(dec);                  \
        }                                                               \
    }

/* Normalize internal decimal to fit into canonical Dec, with IEEE rounding. */
#define __IMPL_NORMALIZE_TO_DEC(__dec)                                  \
    static void normalize_to_##__dec(intern_##__dec##_t *v) {           \
        if (v->special != DEC_NORMAL || v->coeff == 0)                  \
            return;                                                     \
                                                                        \
        int excess = 0;                                                 \
                                                                        \
        while (v->coeff / __pow10_##__dec[excess] >= __pow10_##__dec[__dec##_MAX_DIGITS]) { \
            excess++;                                                   \
        }                                                               \
                                                                        \
        if (excess > 0) {                                               \
            bits_##__dec##_t divisor = __pow10_##__dec[excess];         \
            bits_##__dec##_t main = v->coeff / divisor;           /* kept part */ \
            bits_##__dec##_t discarded = v->coeff % divisor;      /* to analyze for rounding */ \
            /* Look at top (leftmost) discarded digit for rounding decision */ \
            bits_##__dec##_t first_discarded = discarded / __pow10_##__dec[excess-1]; \
            bits_##__dec##_t rest = discarded % __pow10_##__dec[excess-1];  /* rest of digits discarded */ \
            /* Now, apply IEEE-754 tie-to-even */                       \
            if (first_discarded < 5) {                                  \
                /* already truncated */                                 \
            } else if (first_discarded > 5) {                           \
                main++;                                                 \
            } else { /* ==5 */                                          \
                if (rest != 0) {                                        \
                    main++; /* there's more, round up */                \
                } else {                                                \
                    /* tie: round up if main is odd */                  \
                    if (main & 1) main++;                               \
                }                                                       \
            }                                                           \
            v->coeff = main;                                            \
            v->exponent += excess;                                      \
            /* Overflow from rounding? */                               \
            if (v->coeff >= __dec##_SUP_COEFF) {                        \
                v->coeff /= 10;                                         \
                v->exponent++;                                          \
            }                                                           \
        }                                                               \
                                                                        \
        /* Left-justify if too small, as before */                      \
        while (v->coeff != 0 && v->coeff < __dec##_SUP_COEFF / 10) {    \
            if (v->exponent <= -__dec##_EXP_BIAS) break;                \
            v->coeff *= 10;                                             \
            v->exponent--;                                              \
        }                                                               \
                                                                        \
        /* Clamp exponent */                                            \
        if (v->exponent > __dec##_MAX_EXP) { v->special = DEC_INF; return; } \
        if (v->exponent < __dec##_MIN_EXP) { v->coeff = 0; }            \
    }

/* Convert internal extended decimal to decXX_t,
 * performing normalization and proper special handling.
 */
#define __IMPL_INTERNAL_TO_DEC(__dec)                                   \
    void internal_to_##__dec(const intern_##__dec##_t *in, __dec##_t *dec) { \
        intern_##__dec##_t tmp = *in;  /* Work on a copy for normalization */ \
        /* Handle special cases up front */                             \
        if (tmp.special == DEC_NAN) {                                   \
            __dec##_set_nan(dec);                                       \
            return;                                                     \
        }                                                               \
        if (tmp.special == DEC_INF) {                                   \
            __dec##_set_inf(dec, tmp.sign);                             \
            return;                                                     \
        }                                                               \
                                                                        \
        /* Step 1: Normalize coefficient and exponent, with IEEE rounding */ \
        normalize_to_##__dec(&tmp);                                     \
                                                                        \
        /* Step 2: Handle overflow/underflow again, in case normalization triggers it */ \
        if (tmp.special == DEC_INF) {                                   \
            __dec##_set_inf(dec, tmp.sign);                             \
            return;                                                     \
        }                                                               \
                                                                        \
        if (tmp.coeff == 0) {                                           \
            /* TBD: Canonical representation for +0 or -0 (sign is 0 for +0 according to IEEE, keep 1 for signed zero if you wish) */ \
            *dec = __dec##_zero;                                        \
            /* dec->bits = 0; */                                        \
            return;                                                     \
        }                                                               \
                                                                        \
        /* Step 3: Encode into decXX_t */                               \
        dec->bits = 0;                                                  \
        __dec##_set_sign(dec, tmp.sign);                                \
        __dec##_set_coefficient(dec, tmp.coeff);  /* Will never exceed 16 digits after normalization */ \
        __dec##_set_exponent(dec, tmp.exponent);                        \
    }
