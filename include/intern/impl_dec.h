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
