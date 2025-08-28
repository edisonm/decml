#define __IMPL_CONST_INTERN_DEC(__dec, __name, __sign, __coeff, __exp, __special)      \
    const intern_##__dec##_t intern_##__dec##_##__name = {__sign, __coeff, __exp, __special}

// IEEE-754 decimal128 internal extended precision constants
#define __IMPL_CONST_INTERN_DECS(__dec) \
    __IMPL_CONST_INTERN_DEC(__dec, zero,    0, 0, 0, DEC_NORMAL);    \
    __IMPL_CONST_INTERN_DEC(__dec, one,     0, 1, 0, DEC_NORMAL);    \
    __IMPL_CONST_INTERN_DEC(__dec, two,     0, 2, 0, DEC_NORMAL);    \
    __IMPL_CONST_INTERN_DEC(__dec, three,   0, 3, 0, DEC_NORMAL);    \
    __IMPL_CONST_INTERN_DEC(__dec, nan,     0, 0, 0, DEC_NAN);       \
    __IMPL_CONST_INTERN_DEC(__dec, inf,     0, 0, 0, DEC_INF);       \
    __IMPL_CONST_INTERN_DEC(__dec, neginf,  1, 0, 0, DEC_INF);

/* Normalize internal decimal */
#define __IMPL_NORMALIZE_INTERN_DEC(__dec)                      \
    void normalize_intern_##__dec(intern_##__dec##_t *v) {      \
        normalize_coeff_exp_##__dec(&v->coeff, &v->exponent);   \
    }

#define __IMPL_NORMALIZE_COEFF_EXP_DEC(__dec)                           \
    void normalize_coeff_exp_##__dec(bits_##__dec##_t *coeff, int *exponent) { \
        int diff = 0;                                                   \
        if (*coeff == 0)                                                \
            return;                                                     \
        while (*coeff <= (UINT_##__dec##_MAX/10)/__pow10_##__dec[diff]) { \
            diff++;                                                     \
        }                                                               \
        *coeff *= __pow10_##__dec[diff];                                \
        *exponent -= diff;                                              \
    }

#define __IMPL_INTERN_DEC_floor(__dec)                          \
    void intern_##__dec##_floor(intern_##__dec##_t *result,     \
                                const intern_##__dec##_t *a) {  \
        *result = *a;                                           \
        if (!intern_##__dec##_is_finite(a)) return;              \
        int e = a->exponent;                                    \
        if (e >= 0) {                                           \
            return; /* Already integral */                      \
        }                                                       \
                                                                \
        int ne = -e;                                            \
        bits_##__dec##_t intpart;                               \
        bits_##__dec##_t remainder;                             \
        if (ne > I_##__dec##_MAX_DIGITS) {                      \
            intpart = 0;                                        \
            remainder = a->coeff;                               \
        } else {                                                \
            intpart = a->coeff / __pow10_##__dec[ne];           \
            remainder = a->coeff % __pow10_##__dec[ne];         \
        }                                                       \
                                                                \
        if (a->sign == 0) {                                     \
            /* positive: truncate */                            \
            result->coeff = intpart;                            \
            result->exponent = 0;                               \
        } else {                                                \
            /* negative: floor(-1.3) == -2 */                   \
            if (remainder != 0) intpart += 1;                   \
            result->coeff = intpart;                            \
            result->exponent = 0;                               \
        }                                                       \
    }

#define __IMPL_INTERN_DEC_ceil(__dec)                           \
    void intern_##__dec##_ceil(intern_##__dec##_t *result,      \
                               const intern_##__dec##_t *a) {   \
        *result = *a;                                           \
        if (!intern_##__dec##_is_finite(a)) return;             \
        int e = a->exponent;                                    \
        if (e >= 0) return;                                     \
                                                                \
        int ne = -e;                                            \
        bits_##__dec##_t intpart;                               \
        bits_##__dec##_t remainder;                             \
        if (ne > I_##__dec##_MAX_DIGITS) {                      \
            intpart = 0;                                        \
            remainder = a->coeff;                               \
        } else {                                                \
            intpart = a->coeff / __pow10_##__dec[ne];           \
            remainder = a->coeff % __pow10_##__dec[ne];         \
        }                                                       \
                                                                \
        if (a->sign == 0) {                                     \
            /* positive: round up if not exact */               \
            if (remainder != 0) intpart += 1;                   \
            result->coeff = intpart;                            \
            result->exponent = 0;                               \
        } else {                                                \
            /* negative: just truncate */                       \
            result->coeff = intpart;                            \
            result->exponent = 0;                               \
        }                                                       \
    }

#define __IMPL_INTERN_DEC_round(__dec)                          \
    void intern_##__dec##_round(intern_##__dec##_t *result,     \
                                const intern_##__dec##_t *a) {  \
        *result = *a;                                           \
        if (!intern_##__dec##_is_finite(a)) return;             \
        int e = a->exponent;                                    \
        if (e >= 0) return;                                     \
                                                                \
        int ne = -e;                                            \
        bits_##__dec##_t intpart;                               \
        bits_##__dec##_t remainder;                             \
        bits_##__dec##_t threshold;                             \
        if (ne > I_##__dec##_MAX_DIGITS) {                      \
            intpart = 0;                                        \
            remainder = a->coeff;                               \
            threshold = UINT_##__dec##_MAX;                     \
        } else {                                                \
            intpart = a->coeff / __pow10_##__dec[ne];           \
            remainder = a->coeff % __pow10_##__dec[ne];         \
            threshold = __pow10_##__dec[ne] / 2;                \
        }                                                       \
                                                                \
        if (remainder != 0) {                                   \
            if (remainder >= threshold) intpart += 1;           \
        }                                                       \
        result->coeff = intpart;                                \
        result->exponent = 0;                                   \
    }
