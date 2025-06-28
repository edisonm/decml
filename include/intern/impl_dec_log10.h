#define __IMPL_INTERN_DEC_LOG10(__dec)                                  \
    void intern_##__dec##_log10(intern_##__dec##_t *result,             \
                                const intern_##__dec##_t *x) {          \
        if (x->sign) {                                                  \
            intern_##__dec##_set_nan(result); /* log(negative) = nan */ \
            return;                                                     \
        }                                                               \
                                                                        \
        if (x->coeff==0) {                                              \
            intern_##__dec##_set_inf(result, 1); /* log(0) = -inf */    \
            return;                                                     \
        }                                                               \
                                                                        \
        intern_##__dec##_t b = *x;                                      \
                                                                        \
        normalize_intern_##__dec(&b);                                   \
                                                                        \
        int b_digits = 0;                                               \
        bits_##__dec##_t tmp = b.coeff;                                 \
                                                                        \
        while (tmp>=10) { tmp /= 10; b_digits++; }                      \
                                                                        \
        int exp = b.exponent+b_digits;                                  \
        int digits = I_##__dec##_MAX_DIGITS+3;                          \
        int tmp2 = exp;                                                 \
        while (tmp2) { tmp2 /= 10; digits--; }                          \
                                                                        \
        int coeff_digits = 9; /* 9 = floor(log10(INT64_MAX)) */         \
                                                                        \
        /* now we calculate the log10 in few iteractions, just using powi and then */ \
        /* we move the exponent to the coefficient in a clever way: */  \
        bits_##__dec##_t r_coeff = 0;                                   \
        int r_exponent = 0;                                             \
        intern_##__dec##_t r;                                           \
        while (coeff_digits != 0) {                                     \
            b.exponent = -b_digits;                                     \
            intern_##__dec##_powi(&r, &b, (int)__pow10_##__dec[coeff_digits]); \
            b_digits = 0;                                               \
            tmp = r.coeff;                                              \
            int i = 0;                                                  \
            while (r_coeff > UINT_##__dec##_MAX / __pow10_##__dec[coeff_digits-i]) { \
                i++;                                                    \
            }                                                           \
            while (tmp>=10) { tmp /= 10; b_digits++; }                  \
            r_coeff = r_coeff*__pow10_##__dec[coeff_digits-i]           \
                +(int)((r.exponent+b_digits+(int)__pow10_##__dec[i]/2)/__pow10_##__dec[i]);  \
            r_exponent -= coeff_digits-i;                               \
            digits -= coeff_digits;                                     \
            if (coeff_digits >= digits) {                               \
                coeff_digits = digits;                                  \
            }                                                           \
            b = r;                                                      \
        }                                                               \
        b.sign = 0;                                                     \
        b.coeff = r_coeff;                                              \
        b.exponent = r_exponent;                                        \
        r.sign = (exp<0)?1:0;                                           \
        r.exponent = 0;                                                 \
        r.coeff = (exp<0)?-exp:exp;                                     \
        intern_##__dec##_add(result, &r, &b);                           \
}
